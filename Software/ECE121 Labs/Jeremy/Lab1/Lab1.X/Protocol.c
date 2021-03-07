/* 
 * File:   Protocol.c
 * Author: Jeremy Germenis
 * Brief: 
 * Created on <month> <day>, <year>, <hour> <pm/am>
 * Modified on <month> <day>, <year>, <hour> <pm/am>
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/

#define PACKETBUFFERSIZE 5  // how many payloads the buffer has to store, not bytes

#define MAXPAYLOADLENGTH 128 // note that this does include the ID

#define HEAD 204
#define TAIL 185

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <GenericTypeDefs.h>
#include <xc.h>
#include <sys/attribs.h>

#include "BOARD.h"
#include "MessageIDs.h"
#include "CircBuff.h"
#include "Protocol.h"

/*******************************************************************************
 * PUBLIC DATATYPES
 ******************************************************************************/

static struct circBuffer tx; //global rx and tx buffers
static struct circBuffer rx;
static unsigned char chksm; //global variables for receive state machine
static unsigned char lngth;
static unsigned char rxreg; //global variable for uart rx register read variable
static unsigned char payload[MAXPAYLOADLENGTH]; //another global variable for receive state machine

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/

/**
 * @Function Protocol_Init(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief 
 * @author mdunne */
void Protocol_Init(void)
{
    U1STA = 0; //clears control register for UART1
    U1BRG = 21; //sets baud rate to 115200
    U1MODE = 0x8000; //enables UART1 for 8-N-1
    U1STASET = 0x1400; //Enable uart transmit and receive

    IEC0bits.U1TXIE = 1; //enables transmit interrupts
    IPC6bits.U1IP = 1; //sets transmit interrupt priority
    U1STAbits.UTXISEL1 = 0; //transmit interrupt only happens when transmit register space available
    U1STAbits.UTXISEL0 = 0;

    IEC0bits.U1RXIE = 1; //enables receive interrupts
    U1STAbits.URXISEL1 = 0; //enables receive interrupts when receive buffer is not empty
    U1STAbits.URXISEL0 = 0;
    return;
}

/**
 * @Function int Protocol_SendMessage(unsigned char len, void *Payload)
 * @param len, length of full <b>Payload</b> variable
 * @param Payload, pointer to data, will be copied in during the function
 * @return SUCCESS or ERROR
 * @brief 
 * @author mdunne */
void Protocol_SendMessage(unsigned char len, unsigned char ID, void *Payload)
{
    unsigned char *payload = Payload;
    CircBuff_Add_Data(HEAD, &tx);
    CircBuff_Add_Data(len, &tx); //puts head and length of payload into buffer

    unsigned char checksum = 0;
    CircBuff_Add_Data(ID, &tx); //puts message id into buffer and initializes checksum
    checksum = Protocol_CalcIterativeChecksum(ID, checksum);
    payload = payload + 1; //loop will start without ID in payload
    int i;
    for (i = 1; i < len; i++) { //for as long as the length is(excluding ID), insert payload into buffer
        checksum = Protocol_CalcIterativeChecksum(*payload, checksum); //also calculates checksum every iteration
        CircBuff_Add_Data(*payload, &tx);
        payload = payload + 1; //uses next address of payload to get next char
    }
    CircBuff_Add_Data(TAIL, &tx); //puts tail into buffer
    CircBuff_Add_Data(checksum, &tx); //puts pre-calculated checksum into buffer
    CircBuff_Add_Data('\r', &tx);
    CircBuff_Add_Data('\n', &tx);
    return;
}

/**
 * @Function int Protocol_SendDebugMessage(char *Message)
 * @param Message, Proper C string to send out
 * @return SUCCESS or ERROR
 * @brief Takes in a proper C-formatted string and sends it out using ID_DEBUG
 * @warning this takes an array, do <b>NOT</b> call sprintf as an argument.
 * @author mdunne */
void Protocol_SendDebugMessage(char *Message)
{
    CircBuff_Add_Data(HEAD, &tx); //starts with head

    unsigned char length = 0;
    length = strlen(Message); //gets length of string
    CircBuff_Add_Data((length + 1), &tx); //adds ID to length
  
    CircBuff_Add_Data(ID_DEBUG, &tx); //puts message id into buffer and initializes checksum
    unsigned char checksum;
    checksum = Protocol_CalcIterativeChecksum(ID_DEBUG, 0);
    int i;
    for (i = 0; i < length; i++) { //for as long as the length is, insert payloads into buffer,
        checksum = Protocol_CalcIterativeChecksum(*Message, checksum); //also calculates checksum every iteration
        CircBuff_Add_Data(*Message, &tx);
        Message = Message + 1; //uses next address of payload to get next char
    }
    CircBuff_Add_Data(TAIL, &tx); //puts tail into buffer
    CircBuff_Add_Data(checksum, &tx); //puts pre-calculated checksum into buffer
    CircBuff_Add_Data('\r', &tx);
    CircBuff_Add_Data('\n', &tx);
    return;
}

/**
 * @Function char Protocol_ShortEndednessConversion(unsigned short inVariable)
 * @param inVariable, short to convert endedness
 * @return converted short
 * @brief Converts endedness of a short. This is a bi-directional operation so only one function is needed
 * @author mdunne */
unsigned short Protocol_ShortEndednessConversion(unsigned short inVariable)
{
    unsigned short left = inVariable & 0xFF00; //gets first byte
    left = left >> 8; //turns first byte into LSB
    unsigned short right = inVariable & 0x00FF; //gets second byte
    right = right << 8; //turns second byte into MSB
    return (left + right);
}

/**
 * @Function char Protocol_IntEndednessConversion(unsigned int inVariable)
 * @param inVariable, int to convert endedness
 * @return converted short
 * @brief Converts endedness of a int. This is a bi-directional operation so only one function is needed
 * @author mdunne */
unsigned int Protocol_IntEndednessConversion(unsigned int inVariable)
{
    unsigned int left = inVariable & 0xFF000000; //gets MSB
    left = left >> 24;
    unsigned int right = inVariable & 0x000000FF; //gets LSB
    right = right << 24;

    unsigned int midleft = inVariable & 0x00FF0000; //gets 2nd MSB
    midleft = midleft >> 8;
    unsigned int midright = inVariable & 0x0000FF00; //gets 2nd LSB
    midright = midright << 8;

    return (left + right + midright + midleft);
}

/*******************************************************************************
 * PRIVATE FUNCTIONS
 * generally these functions would not be exposed but due to the learning nature of the class they
 * are to give you a theory of how to organize the code internal to the module
 ******************************************************************************/

/**
 * @Function char Protocol_CalcIterativeChecksum(unsigned char charIn, unsigned char curChecksum)
 * @param charIn, new char to add to the checksum
 * @param curChecksum, current checksum, most likely the last return of this function, can use 0 to reset
 * @return the new checksum value
 * @brief Returns the BSD checksum of the char stream given the curChecksum and the new char
 * @author mdunne */
unsigned char Protocol_CalcIterativeChecksum(unsigned char charIn, unsigned char curChecksum)
{
    unsigned char Chksm = (curChecksum >> 1) + (curChecksum << 7); //circular right shift curChecksum
    Chksm += charIn; //adds next char and bitmasks 2 bytes only
    Chksm = Chksm &= 255;
    return (Chksm);
}

/**
 * @Function void Protocol_runReceiveStateMachine(unsigned char charIn)
 * @param charIn, next character to process
 * @return None
 * @brief Runs the protocol state machine for receiving characters, it should be called from 
 * within the interrupt and process the current character
 * @author mdunne */
void Protocol_RunReceiveStateMachine(unsigned char charIn)
{
    if (charIn != HEAD) { //trashes data if not starting with head
        CircBuff_Init(&rx);
        return;
    }
    lngth = 0;
    lngth = CircBuff_Read_Data(&rx); //returns length of message into lngth global variable
    
    int i;   
    chksm = 0;
    for (i = 0; i < lngth; i++) { //puts the payload of the rx buffer into global char array
        payload[i] = CircBuff_Read_Data(&rx);
        chksm = Protocol_CalcIterativeChecksum(payload[i], chksm); //also gets checksum
    }
    
    if (CircBuff_Read_Data(&rx) != TAIL) { //trashes data if not ending in tail
        CircBuff_Init(&rx);
        return;
    } else if (CircBuff_Read_Data(&rx) != chksm) { //trashes data if checksum isn't the same as what we got from payload
        CircBuff_Init(&rx);
        return;
    } else if (CircBuff_Read_Data(&rx) != '\r') { //trashes data if it doens't end in \r, \n
        CircBuff_Init(&rx);
        return;
    } else if (CircBuff_Read_Data(&rx) != '\n') {
        CircBuff_Init(&rx);
        return;
    }
    CircBuff_Init(&rx); //clears rx register after successful message read
    
    unsigned char outmessage[MAXPAYLOADLENGTH] = {NULL}; //sets up echoed message variables
    void *outptr = &outmessage;
    if (payload[0] == ID_LEDS_SET) { //sets the LEDS if gotten ID_LEDS_SET
        LEDS_SET(payload[1]);
        return;
    }
    if (payload[0] == ID_LEDS_GET) { //returns state of LED byte if ID_LEDS_GET ID detected
        outmessage[0] = ID_LEDS_STATE;
        outmessage[1] = LEDS_GET();
        Protocol_SendMessage(2, ID_LEDS_STATE, outptr); //sends LEDS State message
        return;
    }
    if (payload[0] == ID_PING) { //performs a division by 2 if sends ID_PING
        outmessage[0] = ID_PONG;
        unsigned int ping = (payload[1] << 24) + (payload[2] << 16) + (payload[3] << 8) + payload[4]; //little endian conversion
        ping = ping / 2;
        outmessage[1] = (ping >> 24); // big endian conversions to return correct value
        outmessage[2] = ((ping << 8) >> 24);
        outmessage[3] = ((ping << 16) >> 24);
        outmessage[4] = ((ping << 24) >> 24);
        Protocol_SendMessage(5, ID_PONG, outptr);
        return;
    }
    return;
}

/**
 * @Function char PutChar(char ch)
 * @param ch, new char to add to the transmit register
 * @return SUCCESS or ERROR
 * @brief adds to circular buffer if space exists, if not returns ERROR
 * @author mdunne */
void PutChar(char ch)
{
    U1TXREG = ch; //puts char into uart transmit register
    return;
}


/**
 * This macro initializes all LEDs for use. It enables the proper pins as outputs and also turns all
 * LEDs off.
 */
#define LEDS_INIT() do {LATECLR = 0xFF; TRISECLR = 0xFF;} while (0)

/**
 * Provides a way to quickly get the status of all 8 LEDs into a uint8, where a bit is 1 if the LED
 * is on and 0 if it's not. The LEDs are ordered such that bit 7 is LED8 and bit 0 is LED0.
 */
#define LEDS_GET() (LATE & 0xFF)

/**
 * This macro sets the LEDs on according to which bits are high in the argument. Bit 0 corresponds
 * to LED0.
 * @param leds Set the LEDs to this value where 1 means on and 0 means off.
 */
#define LEDS_SET(leds) do { LATE = (leds); } while (0)

#define PROTOCOL_TEST
#ifdef PROTOCOL_TEST

int main(void)
{
    BOARD_Init();
    Protocol_Init(); //initializes board and UART1 protocol

    CircBuff_Init(&tx); //initializes transmit buffer
    CircBuff_Init(&rx); //initializes receive buffer
    LEDS_INIT();

    unsigned char array[] = "";
    sprintf(array, "Compilation Date: %s  Compilation Time: %s", __DATE__, __TIME__);
    unsigned char *arpt = array;
    Protocol_SendDebugMessage(arpt); //tests debug message, says date and time

    unsigned char ledsetpayload[MAXPAYLOADLENGTH] = {204, 5, 132, 75, 49, 74, 72, 185, 234, 13, 10}; //creates char array for payload
    int i;
    for (i = 0; i < 11; i++) {
        CircBuff_Add_Data(ledsetpayload[i], &rx); //my code doenst work unless you put something in the receive buffer first
    }
    CircBuff_Init(&rx); //initializes receive buffer again, dont know why but it doens't work without this


    while (1) { //main loop
        if ((U1STAbits.RIDLE == 1) && U1STAbits.URXDA == 1) {//forces receive interrupt if UART is idle and receive buffer isn't empty
            IFS0bits.U1RXIF = 1;
        } else if ((U1STAbits.TRMT == 1) && (CircBuff_Test_Empty(tx) == 0 && U1STAbits.RIDLE == 1 && CircBuff_Test_Empty(rx) == 1)) {
            IFS0bits.U1TXIF = 1; //forces transmit interrupt if UART is idle and transmit buffer isn't empty, while receive buffer is empty
        }
    }
    BOARD_End(); //just in case
    return (0);
}

void __ISR(_UART1_VECTOR) IntUart1Handler(void)
{
    if (IFS0bits.U1TXIF == 1 && IFS0bits.U1RXIF == 0) { //transmit interrupt only if transmit buffer not empty and receive buffer is
        if ((CircBuff_Test_Empty(tx) == 0 && U1STAbits.RIDLE == 1 && CircBuff_Test_Empty(rx) == 1)) {
            PutChar(CircBuff_Read_Data(&tx)); //puts next char into uart transmit buffer if not empty
        }
        IFS0bits.U1TXIF = 0; //clears transmit interrupt
    }
    if (IFS0bits.U1RXIF == 1 && IFS0bits.U1TXIF == 0) { //receive interrupt
        if (U1STAbits.URXDA == 1) { //if there is data in the receive buffer
            rxreg = U1RXREG; //puts received data into rx buffer
            CircBuff_Add_Data(rxreg, &rx);
            if (rxreg == '\n'){ //if last char of a message is detected, enter state machine to return message
                Protocol_RunReceiveStateMachine(CircBuff_Read_Data(&rx));
            }
            rxreg = NULL; //resets receive register variable
            while (U1STAbits.OERR && U1STAbits.URXDA == 1) { //in case of overflow, read all data and clear overflow flag
                rxreg = U1RXREG; 
                CircBuff_Add_Data(rxreg, &rx);
            }
            U1STAbits.OERR = 0;
        }
        IFS0bits.U1RXIF = 0; //clears receive interrupt
    }
    IFS0bits.U1RXIF = 0; //clears both interrupts in case of an overlap
    IFS0bits.U1TXIF = 0;
}
#endif

#ifdef PUTTY_TEST

int main(void)
{
    BOARD_Init();
    Protocol_Init(); //initializes board and UART1 protocol

    CircBuff_Init(&tx); //initializes transmit buffer
    char string[] = "Hello World!";
    int i;
    for (i = 0; i < strlen(string); i++) { //inputs string into circular transmit buffer
        CircBuff_Add_Data(string[i], &tx);
    }

    while (1) { //infinite loop
        if (U1STAbits.TRMT == 1 && CircBuff_Test_Empty(tx) == 0) {//forces transmit interrupt if UART is idle and transmit buffer isn't empty
            IFS0bits.U1TXIF = 1;
        }
    }
    BOARD_End();
    return (0);
}

void __ISR(_UART1_VECTOR) IntUart1Handler(void)
{
    if (IFS0bits.U1TXIF == 1) { //transmit interrupt
        if (CircBuff_Test_Empty(tx) == 0) {
            PutChar(CircBuff_Read_Data(&tx)); //puts next char into transmit buffer if not empty
        }
    }

    if (IFS0bits.U1RXIF == 1) { //receive interrupt
        if (U1STAbits.URXDA == 1) { //if there is data in the receive buffer
            rxreg = U1RXREG; //sets global variable to data
            PutChar(rxreg); //echos receive char
            U1STAbits.OERR = 0; //clears receive buffer and overflow error
            rxreg = NULL; //clears receive variable
        }
    }
    IFS0bits.U1TXIF = 0; //clears both interrupt flags at end of interrupt
    IFS0bits.U1RXIF = 0;
}
#endif

#ifdef CHECKSUM_TEST

int main(void)
{
    BOARD_Init();
    unsigned char checksum = 0; //testing checksum for 0x817F
    checksum = Protocol_CalcIterativeChecksum(0x81, checksum);
    checksum = Protocol_CalcIterativeChecksum(0x7F, checksum);
    printf("%x\n", checksum);

    checksum = 0; //testing checksum for 0x807F35
    checksum = Protocol_CalcIterativeChecksum(0x80, checksum);
    checksum = Protocol_CalcIterativeChecksum(0x7F, checksum);
    checksum = Protocol_CalcIterativeChecksum(0x35, checksum);
    printf("%x\n", checksum);

    checksum = 0; //testing checksum for 0x8400257D96
    checksum = Protocol_CalcIterativeChecksum(0x84, checksum);
    checksum = Protocol_CalcIterativeChecksum(0x00, checksum);
    checksum = Protocol_CalcIterativeChecksum(0x25, checksum);
    checksum = Protocol_CalcIterativeChecksum(0x7D, checksum);
    checksum = Protocol_CalcIterativeChecksum(0x96, checksum);
    printf("%x\n", checksum);

    BOARD_End();
    return (0);
}
#endif