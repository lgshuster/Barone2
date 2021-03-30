#include <xc.h>
#include <stdio.h>
#include <string.h>
#include <sys/attribs.h>
#include "Protocol.h"
#include "BOARD.h"
#include "buffer.h"
#include "MessageIDs.h"

#define TX 0
#define RX 1
#define FALSE 0
#define TRUE 1
#define TEST_HARNESS_STATE_MACHINE
typedef enum {
    WAITING_FOR_HEAD,
    STORE_LENGTH,
    STORE_PAYLOAD,
    CHECK_TAIL,
    VERIFY_CHECKSUM,
    CHECK_RETURN,
    CHECK_NEW_LINE,
    PACKET_INVALID
}state;
struct RunReceive { // Used in RunReceiveStateMachine function
    unsigned char len, calculatedRXchecksum;
    unsigned char LEDState, i;
    unsigned char RXpayload[MAXPAYLOADLENGTH];  // Only stores paylaod without ID in first byte
    state RXstate;
} RunReceive;

struct IncomingPacket { // Incoming packets that are validated by StateMachine is stored in here
    unsigned int inverted;
    unsigned char len;
    unsigned char payload[MAXPAYLOADLENGTH];
    char debugMessage[MAXPAYLOADLENGTH];
} IncomingPacket;

/*Global Variables*/
static int addToTransmit;   // Used for collision avoidance

int Protocol_Init(void){
    BOARD_Init();            // Initialize BOARD.c
    initBuffer();           // Initialize TX and RX Buffer
    LEDS_INIT();              // Initialize LEDs
    U1STA = 0;               // Clear control registers for UART1
    U1MODEbits.BRGH = 0;    // Enable 16x baud clock
    U1BRG = 21;             // Baud rate to 115200 (40Mhz/(16*115200))-1 = 20.7, 
                            // round up to nearest unit = 21 for baud rate of 115K with 1.4% error (acceptable)
    U1MODEbits.PDSEL = 0b00;   // Enable UART for 8-bit data, No Parity
    U1MODEbits.STSEL = 0;   // 1 Stop bit
    U1MODEbits.ON = 1;    // Enable UART1
    U1STAbits.UTXEN = 1;    // Enable TX for UART1
    U1STAbits.URXEN = 1;    // Enable RX for UART1
    
    IPC6bits.U1IP = 1;      // Interrupt Priority level 1
    U1STAbits.UTXISEL = 0b00;   // Set Transmit Interrupt when buffer is empty
    U1STAbits.URXISEL = 0b00;   // Same for Receive Interrupt  
    IEC0bits.U1TXIE = 1;    // Enable TX Interrupt
    IEC0bits.U1RXIE = 1;    // Enable RX Interrupt
    
    RunReceive.RXstate = WAITING_FOR_HEAD;  // Initialize first state in Protocol_RunReceiveStateMachine
    /*Send ID_DEBUG message with __DATE__ & __TIME__*/
    char message[MAXPAYLOADLENGTH];
    sprintf(message,"Compiled on: %s %s\n", __DATE__,__TIME__);
    Protocol_SendDebugMessage(message);
}

int Protocol_SendMessage(unsigned char len, unsigned char ID, void *Payload){
    unsigned char checksum = 0;
    unsigned char *payloadArray = Payload;   // Define payload as chars
    /* Note: if(PutChar(char) == ERROR) checks if Enqueue worked*/
    if (PutChar(HEAD) == ERROR)
        return ERROR;
    if (PutChar(1+ len) == ERROR)   // Enqueue length = ID + length of payload
        return ERROR;
    if (PutChar(ID) == ERROR)
        return ERROR;
    checksum = Protocol_CalcIterativeChecksum(ID,checksum); // First checksum is ID since *Payload param. doesn't include it
    int i;
    for (i = 0; i < len; i++){   // Determines number of times to call PutChar depending on length param.
        checksum = Protocol_CalcIterativeChecksum(*payloadArray,checksum);  // iterate checksum
        if (PutChar(*payloadArray++) == ERROR)  // Enqueue Payload 1 byte at a time, also checks is Enqueue worked
            return ERROR;
    }
    if (PutChar(TAIL) == ERROR)     // Enqueue TAIL
        return ERROR;
    if (PutChar(checksum) == ERROR) // Enqueue final Checksum
        return ERROR;
    if (PutChar('\r') == ERROR)     // Enqueue END 1st byte
        return ERROR;
    if (PutChar('\n') == ERROR)     // Enqueue END 2nd byte
        return ERROR;
    return SUCCESS;         // Returns SUCCESS if no errors ever occurred in PutChar process
}
int Protocol_SendDebugMessage(char *Message){
    unsigned char len;
    char message[MAXPAYLOADLENGTH];
    sprintf(message, "%s",Message);
    len = (unsigned char) strlen(message);
    return Protocol_SendMessage(len + 1, ID_DEBUG, &message);
}
unsigned char Protocol_ReadNextID(void){
    
}
int Protocol_GetPayload(void* payload){
    return ((int) IncomingPacket.payload);
}
char Protocol_IsMessageAvailable(void){
    return U1STAbits.URXDA;
}
char Protocol_IsQueueFull(void){
    return fullBuffer(RX);
}
char Protocol_IsError(void){
    
}
unsigned short Protocol_ShortEndednessConversion(unsigned short inVariable){
    unsigned short x0, x1;
    x0 = (inVariable & 0xff00) >> 8;
    x1 = (inVariable & 0x00ff) << 8;
    return (x0 | x1);
    
}
unsigned int Protocol_IntEndednessConversion(unsigned int inVariable){
    unsigned int x0, x1, x2, x3, x4;
    x0 = (inVariable & 0xff000000) >> 24;
    x1 = (inVariable & 0x00ff0000) >> 8;
    x2 = (inVariable & 0x0000ff00) << 8;
    x3 = (inVariable & 0x000000ff) << 24 ;
    return (x0 | x1 | x2 | x3);
}

unsigned char Protocol_CalcIterativeChecksum(unsigned char charIn, unsigned char curChecksum){
    curChecksum = (curChecksum >> 1) + (curChecksum << 7);      // Circular rotate 1 bit to the right
    curChecksum += charIn;      // Add in new byte of payload
    return curChecksum;
}

void Protocol_RunReceiveStateMachine(unsigned char charIn){
    switch (RunReceive.RXstate){
        case WAITING_FOR_HEAD:
                if(charIn == 0xCC){  // Head received
                    RunReceive.RXstate = STORE_LENGTH;  // Next State
                }
                else{    // Check for HEAD again
                    RunReceive.RXstate = WAITING_FOR_HEAD;
                }
            break;
        case STORE_LENGTH:
            RunReceive.len = charIn;    // Store length
            RunReceive.calculatedRXchecksum = 0;    // Reset checksum before beginning new one
            RunReceive.i = 0;
            RunReceive.RXstate = STORE_PAYLOAD;  // Next State
            break;
        case STORE_PAYLOAD:
                RunReceive.RXpayload[RunReceive.i] = charIn;    // Read payload into payload pointer variable
                RunReceive.calculatedRXchecksum = Protocol_CalcIterativeChecksum(charIn, RunReceive.calculatedRXchecksum);  // Iterate checksum with Payload
                if (RunReceive.i + 1 < RunReceive.len){
                    RunReceive.i++;
                    RunReceive.RXstate = STORE_PAYLOAD;
                }
                else
                    RunReceive.RXstate = CHECK_TAIL;
            break;
        case CHECK_TAIL:
            if (charIn == 0xB9)      // TAIL received in right spot
                RunReceive.RXstate = VERIFY_CHECKSUM;   // Next State
            else{    // TAIL is invalid
                RunReceive.RXstate = PACKET_INVALID;  // Next State
            }
            break;
        case VERIFY_CHECKSUM:
            if (charIn == RunReceive.calculatedRXchecksum) //  Packet's checksum matches local calculated checksum
                RunReceive.RXstate = CHECK_RETURN;  // Next State
            else{    // checksum does not match
                RunReceive.RXstate = PACKET_INVALID;  // Next State
            }
            break;
        case CHECK_RETURN:
            if (charIn == 0x0D)    // '\r' received
                RunReceive.RXstate = CHECK_NEW_LINE;    // Next State
            else{    // '\r' not here, or any other reasons, dump packet
                RunReceive.RXstate = PACKET_INVALID;  // Next State
            }
            break;
        case CHECK_NEW_LINE:
            if (charIn == 0x0A){    // '\n' received
                
                if (RunReceive.RXpayload[0] == ID_LEDS_SET){ // Set LEDs
                    addToTransmit = TRUE;
                    LEDS_SET(RunReceive.RXpayload[1]);
                    addToTransmit = FALSE;
                }
                else if (RunReceive.RXpayload[0] == ID_LEDS_GET){   // Get LEDs
                    addToTransmit = TRUE;
                    RunReceive.LEDState = LEDS_GET();
                    addToTransmit = FALSE;
                    Protocol_SendMessage(1,ID_LEDS_STATE, &RunReceive.LEDState);
                }
                else if (RunReceive.RXpayload[0] == ID_PING){   // PING
                    /*ID_PONG*/
                    IncomingPacket.inverted = 0x00;
                    for (RunReceive.i = 1; RunReceive.i < RunReceive.len; RunReceive.i++){
                        IncomingPacket.inverted = (IncomingPacket.inverted << 8) & 0xFFFFFF00;
                        IncomingPacket.inverted |= (RunReceive.RXpayload[RunReceive.i] & 0xFF);
                    }
                    IncomingPacket.inverted = IncomingPacket.inverted >> 1; // Divide by 2
                    IncomingPacket.inverted = Protocol_IntEndednessConversion(IncomingPacket.inverted); // convert to Little-Endian
                    Protocol_SendMessage(4,ID_PONG, &IncomingPacket.inverted);
                }
                else if (RunReceive.RXpayload[0] == ID_DEBUG){
                    for (RunReceive.i = 1; RunReceive.i < RunReceive.len; RunReceive.i++)
                        IncomingPacket.debugMessage[RunReceive.i - 1] =  RunReceive.RXpayload[RunReceive.i];
                    Protocol_SendDebugMessage(IncomingPacket.debugMessage);
                }
                else{
                    IncomingPacket.len = RunReceive.len;  // Save valid Length to global variable
                    for (RunReceive.i = 0; RunReceive.i < RunReceive.len; RunReceive.i++)
                        IncomingPacket.payload[RunReceive.i] = RunReceive.RXpayload[RunReceive.i];  // Save payload to IncomingPacket.payload
            }
            
                RunReceive.len = 0;
                RunReceive.calculatedRXchecksum = 0;
                RunReceive.LEDState = 0;
                memset(RunReceive.RXpayload, 0, MAXPAYLOADLENGTH);
                RunReceive.RXstate = WAITING_FOR_HEAD;
            }
            else{    // '\n' not here, or any other reasons, dump packet 
                RunReceive.RXstate = PACKET_INVALID;  // Next State
            }
            break;
        case PACKET_INVALID:
            RunReceive.len = 0;
            RunReceive.calculatedRXchecksum = 0;
            RunReceive.LEDState = 0;
            memset(RunReceive.RXpayload, 0, MAXPAYLOADLENGTH);
            IncomingPacket.len = 0;
            memset(IncomingPacket.payload,0,MAXPAYLOADLENGTH);
            RunReceive.RXstate = WAITING_FOR_HEAD;
            break;
    }
}

int PutChar(char ch){
    if (!fullBuffer(TX)){    // Buffer is not full, can write
        addToTransmit = TRUE;   // Set addToTrasmit flag ensure data integrity
        writeBuffer((unsigned char) ch, TX); // Write character to TX Buffer
        addToTransmit = FALSE;
        if (U1STAbits.TRMT == 1)    // If TX is idle
            IFS0bits.U1TXIF = 1;    // Set TX flag
        return SUCCESS;
    } 
    else if(fullBuffer(TX)) {   //If TX circular buffer, keep calling itself
        addToTransmit = TRUE;
        PutChar((unsigned char) ch);
        addToTransmit = FALSE;
    }
    else {  // Discard data, don't do anything
        return ERROR;
    }
}

/*TEST HARNESS*/
#ifdef TEST_HARNESS_BUFFER
void main (void){
    initBuffer();
    unsigned char payload[MAXPAYLOADLENGTH] = {204, 2, 129, 170, 185, 106, 13, 10};
    int i;
    printf("Is buffer empty? %d\n",emptyBuffer(RX));
    for (i = 0; i < 8; i++)
        writeBuffer(payload[i],RX);
    printf("Is buffer empty? %d\n",emptyBuffer(RX));
    printf("Is buffer full? %d\n",fullBuffer(RX));
    printf("Size of buffer: %02x\n",sizeBuffer(RX));
    if ( readBuffer(RX) == 0xcc)
            printf("Head\n");

    for (i = 0; i < 8; i++)
        printf("READ: %d\n",readBuffer(RX));
}
#endif

#ifdef TEST_HARNESS_PUTCHAR
    void main(void){
    Protocol_Init();
    char string[255];
    sprintf(string,"Hello World!\r\n");
    int i;
    for (i = 0; i < strlen(string); i++)
        PutChar(string[i]);
    sprintf(string,"This is a test harness 1234\r\n");
    for (i = 0; i < strlen(string); i++)
        PutChar(string[i]);
    }
        
/*Interrupt Handler*/
    
void __ISR ( _UART1_VECTOR ) IntUart1Handler ( void ) {     // Interrupt Handler
    /*TX*/
        if (IFS0bits.U1TXIF){   // If the TX flag is set
            if (!emptyBuffer(TX))    // If TX circular buffer has data
                U1TXREG = readBuffer(TX);    // Load TX circular buffer char to FIFO TX register buffer
        }
        IFS0bits.U1TXIF = 0;    // Clear TX flag
    }
#endif
#ifdef TEST_HARNESS_ECHO
    char string;
    void main(void){
    Protocol_Init();
    while (1){
        if (string != '\0'){ // If  incoming data is not NULL
            int flag;
            flag = PutChar(string);    // use PutChar to send it
            if (flag == SUCCESS)    // If char is in TX circular buffer
                string = '\0';   // Clear the variable
        }
    }
    }
    
/*Interrupt Handler*/
    
void __ISR ( _UART1_VECTOR ) IntUart1Handler ( void ) {     // Interrupt Handler
    /*TX*/
        if (IFS0bits.U1TXIF){   // If the TX flag is set
            if (!emptyBuffer(TX))    // If TX circular buffer has data
                U1TXREG = readBuffer(TX);    // Load TX circular buffer char to FIFO TX register buffer
        }
    /*RX*/
        if (IFS0bits.U1RXIF){   // If it's the RX flag is set
            if (U1STAbits.URXDA)    // If RX hardware buffer has data to be read
                string = U1RXREG;
        }
        IFS0bits.U1TXIF = 0;    // Clear TX flag
        IFS0bits.U1RXIF = 0;    // Clear RX flag
    }
#endif
#ifdef TEST_HARNESS_ECHO_RXBUFFER
char string;
void main(void){
    Protocol_Init();
    while (1)
        if (!emptyBuffer(RX)) // If  incoming data is not NULL
            PutChar(readBuffer(RX));    // use PutChar to send it
}
    
/*Interrupt Handler*/
    
void __ISR ( _UART1_VECTOR ) IntUart1Handler ( void ) {     // Interrupt Handler
    /*TX*/
        if (IFS0bits.U1TXIF){   // If the TX flag is set
            if (!emptyBuffer(TX) && (U1STAbits.UTXBF == 0) && (addToTransmit == FALSE))    // If TX circular buffer has data and FIFO has space
                U1TXREG = readBuffer(TX);    // Load TX circular buffer char to FIFO TX register buffer
        } else {
            transmitCollision = TRUE;
        }
    /*RX*/
        if (IFS0bits.U1RXIF){   // If the RX flag is set
            if (U1STAbits.URXDA && !fullBuffer(RX))    // If RX hardware buffer has data to be read and circular buffer is not full
                writeBuffer(U1RXREG,RX);
        }
        IFS0bits.U1TXIF = 0;    // Clear TX flag
        IFS0bits.U1RXIF = 0;    // Clear RX flag
    }
#endif

#ifdef TEST_HARNESS_SEND_MESSAGE
void main(void){
    Protocol_Init();
    int LED_payload = 0xAA;
    Protocol_SendMessage(0x01, ID_LEDS_SET, &LED_payload); // Correct return 0xCC 02 81AA B9 6A 0D0A
    int PONG_payload = 0x0000CC58;
    Protocol_SendMessage(0x04, ID_PONG, &PONG_payload); // Correct return 0xCC 05 840000CC58 B9 86 0D0A
    
    while (1){
        IFS0bits.U1TXIF = U1STAbits.TRMT && !emptyBuffer(TX) && !addToTransmit;  // TX is idle and there is data in circular buffer
        IFS0bits.U1RXIF = !U1STAbits.RIDLE && !fullBuffer(RX);
    }
}

/*Interrupt Handler*/

void __ISR ( _UART1_VECTOR ) IntUart1Handler ( void ) {     // Interrupt Handler
    /*TX*/
        if (IFS0bits.U1TXIF){   // If the TX flag is set
            if (!emptyBuffer(TX))    // If TX circular buffer has data and FIFO has space and if not enqueue then
                U1TXREG = readBuffer(TX);    // Dequeue
        } 
        else
            transmitCollision = TRUE;   // Collision detected, go back to TX interrupt ASAP
    /*RX*/
        if (IFS0bits.U1RXIF){   // If the RX flag is set
            if (U1STAbits.URXDA && !fullBuffer(RX))    // If RX hardware buffer has data to be read and circular buffer is not full
                writeBuffer(U1RXREG,RX);
        }
        IFS0bits.U1TXIF = 0;    // Clear TX flag
        IFS0bits.U1RXIF = 0;    // Clear RX flag
    }
#endif
#ifdef TEST_HARNESS_STATE_MACHINE
void main(void){
    Protocol_Init();
    while(1){
        IFS0bits.U1TXIF = U1STAbits.TRMT && !emptyBuffer(TX) && !addToTransmit;  // TX is idle and there is data in circular buffer
        IFS0bits.U1RXIF = U1STAbits.URXDA && !fullBuffer(RX) && !addToTransmit;
    }
}
/*Interrupt Handler*/
void __ISR ( _UART1_VECTOR ) IntUart1Handler ( void ) {     // Interrupt Handler
    /*TX*/
        if (IFS0bits.U1TXIF)
            if (!emptyBuffer(TX)){
                U1TXREG = readBuffer(TX);    // Dequeue
            }
    /*RX*/
        if (IFS0bits.U1RXIF){   // If the RX flag is set
            Protocol_RunReceiveStateMachine(U1RXREG);
        }
        /*Clear flags*/
        IFS0bits.U1TXIF = 0;    // Clear TX flag
        IFS0bits.U1RXIF = 0;    // Clear RX flag
        U1STAbits.OERR = 0;
    }
#endif
#ifdef TEST_HARNESS_GET_LEDS
void main(void){
    Protocol_Init();
    
    setLEDS(0b10101010);
    printf("GetLED = %d\n",getLEDS());
}
void __ISR ( _UART1_VECTOR ) IntUart1Handler ( void ) {     // Interrupt Handler
    /*TX*/
        if (IFS0bits.U1TXIF){   // If the TX flag is set
            if (!emptyBuffer(TX) && !U1STAbits.UTXBF && (addToTransmit = FALSE))    // If TX circular buffer has data and FIFO has space and if not enqueue then
                U1TXREG = readBuffer(TX);    // Dequeue
        } 
        else
            transmitCollision = TRUE;   // Collision detected, go back to TX interrupt ASAP
    /*RX*/
        if (IFS0bits.U1RXIF){   // If the RX flag is set
            if (U1STAbits.URXDA && !fullBuffer(RX)){    // If RX hardware buffer has data to be read and circular buffer is not full
               writeBuffer(U1RXREG,RX);
            }
            Protocol_RunReceiveStateMachine(readBuffer(RX));
        }
        IFS0bits.U1TXIF = 0;    // Clear TX flag
        IFS0bits.U1RXIF = 0;    // Clear RX flag
    }
#endif

#ifdef TEST_HARNESS_ENDIAN
void main(void){
    unsigned short shortInput = 0x3412;
    unsigned short shortResult = Protocol_ShortEndednessConversion(shortInput);
    printf("Short Endedness: %x\n", shortResult);
    
    unsigned int intInput = 0x78563412;
    unsigned int intResult = Protocol_IntEndednessConversion(intInput);
    printf("Int Endedness: %x\n", intResult);
}
#endif