#include <proc/p32mx340f512h.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <xc.h>
#include "BOARD.h"
#include "Protocol.h"
#include "MessageIDs.h"
#include "NonVolatileMemory.h"

//#define TEST_HARNESS_NVM_WRITE_PAGE
#define CONTROL_CODE 0xA0


int previousTime, currentTime;

int NonVolatileMemory_Init(void){
    I2C1CON = 0;
    I2C1BRG = 0x00C5;    // FSCK = 100kH
    I2C1CONbits.ON = 1;
    return SUCCESS;
}

unsigned char NonVolatileMemory_ReadByte(int address){
    I2C1CONbits.SEN = 1;
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1CONbits.SEN) {   // Wait for Start
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    I2C1TRN = CONTROL_CODE & 0xFFFE;    // {Read and write operations <7:4>, chip select <3:1>, R/W = 0 <0>}
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.TRSTAT) { // Wait for empty TX buffer
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5) // Timeout after 5ms
            return ERROR;
    }
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.ACKSTAT) {    // ACK Control Bits
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    I2C1TRN = (address & 0xFF00) >> 8; //8 bit address high 
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.TRSTAT) { // Wait for empty TX buffer
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.ACKSTAT) {  // ACK high address received
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }   
    I2C1TRN = address & 0x00FF; // 8 bit address low
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.TRSTAT) {
    if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    } 
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.ACKSTAT) {  // ACK low address received
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    I2C1CONbits.RSEN = 1;   // Resend
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1CONbits.RSEN) {  // Restart sent
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    I2C1TRN = CONTROL_CODE | 1;    // {Read and write operations <7:4>, chip select <3:1>, R/W = 1 <0>}
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.TRSTAT) { // TX empty
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5) // Timeout after 5ms
            return ERROR;
    }
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.ACKSTAT) {    // ACK received
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    I2C1CONbits.RCEN = 1;   // Enable RX buffer to receive
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1CONbits.RCEN) {    // RX buffer full
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    I2C1CONbits.ACKDT = 1;
    I2C1CONbits.ACKEN = 1;
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1CONbits.ACKEN) {    // Master ACK sent
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    I2C1CONbits.PEN = 1;
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1CONbits.PEN) {    // Stop sent
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    return I2C1RCV;
}

char NonVolatileMemory_WriteByte(int address, unsigned char data){
    I2C1CONbits.SEN = 1;
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1CONbits.SEN) {   // Wait for Start
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    I2C1TRN = CONTROL_CODE & 0xFFFE;    // {Read and write operations = 0b1010 <7:4>, chip select = 0b000 <3:1>, R/W = 0 <0>}
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.TRSTAT) { // Wait for empty TX buffer
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5) // Timeout after 5ms
            return ERROR;
    }
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.ACKSTAT) {    // ACK Control Bits
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    I2C1TRN = (address & 0xFF00) >> 8; //8 bit address high 
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.TRSTAT) { // Wait for empty TX buffer
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.ACKSTAT) {  // ACK high address
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }   
    I2C1TRN = address & 0x00FF; // 8 bit address low
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.TRSTAT) {
    if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    } 
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.ACKSTAT) {  // ACK low address
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    } 
    I2C1TRN = data; // 8-bit data
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.TRSTAT) {
    if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    } 
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.ACKSTAT) {  // ACK data received
    if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    I2C1CONbits.PEN = 1;
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1CONbits.PEN) {  // Wait for Stop
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    return SUCCESS;
}

int NonVolatileMemory_ReadPage(int page, char length, unsigned char data[]){
    if (!length)    // Length is 0
        return ERROR;
    I2C1CONbits.SEN = 1;    // Start
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1CONbits.SEN) {   // Wait for Start
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    I2C1TRN = CONTROL_CODE & 0xFFFE;    // {Read and write operations <7:4>, chip select <3:1>, R/W = 0 <0>}
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.TRSTAT) { // Wait for empty TX buffer
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5) // Timeout after 5ms
            return ERROR;
    }
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.ACKSTAT) {    // ACK Control Bits
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    I2C1TRN = ((page << 6) & 0xFF00) >> 8; //8 bit address high 
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.TRSTAT) { // Wait for empty TX buffer
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.ACKSTAT) {  // ACK high address received
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }   
    I2C1TRN = (page << 6) & 0x00FF; // 8 bit address low
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.TRSTAT) {
    if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    } 
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.ACKSTAT) {  // ACK low address received
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    I2C1CONbits.RSEN = 1;   // Resend
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1CONbits.RSEN) {  // Restart sent
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    I2C1TRN = CONTROL_CODE | 1;    // {Read and write operations <7:4>, chip select <3:1>, R/W = 1 <0>}
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.TRSTAT) { // TX empty
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5) // Timeout after 5ms
            return ERROR;
    }
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.ACKSTAT) {    // ACK received
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    int i;
    for(i = 0; i < length - 1; i++){
        I2C1CONbits.RCEN = 1;   // Enable RX buffer to receive
        previousTime = FreeRunningTimer_GetMilliSeconds();
        while (I2C1CONbits.RCEN) {    // RX buffer full
            if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
                return ERROR;
        }
        I2C1CONbits.ACKDT = 0;  // Master sends ACK
        I2C1CONbits.ACKEN = 1;
        previousTime = FreeRunningTimer_GetMilliSeconds();
        while (I2C1CONbits.ACKEN) {    // Master ACK sent
            if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
                return ERROR;
        }
        data[length] = I2C1RCV;  // Store data in i-th element
    }
    I2C1CONbits.RCEN = 1;   // Enable RX buffer to receive
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1CONbits.RCEN) {    // RX buffer full
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
        }
    I2C1CONbits.ACKDT = 1;  // Master sends NACK
    I2C1CONbits.ACKEN = 1;
    data[i] = I2C1RCV;  // Store data in i-th element
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1CONbits.ACKEN) {    // Master NACK sent
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    I2C1CONbits.PEN = 1;
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1CONbits.PEN) {    // Stop sent
            if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 10)
                return ERROR;
    }
    return SUCCESS;
}
int NonVolatileMemory_WritePage(int page, char length, unsigned char data[]){
    if (!length)    // length is 0
        return ERROR;
    I2C1CONbits.SEN = 1;
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1CONbits.SEN) {   // Wait for Start
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    I2C1TRN = CONTROL_CODE & 0xFFFE;    // {Read and write operations = 0b1010 <7:4>, chip select = 0b000 <3:1>, R/W = 0 <0>}
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.TRSTAT) { // Wait for empty TX buffer
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5) // Timeout after 5ms
            return ERROR;
    }
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.ACKSTAT) {    // ACK Control Bits
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    I2C1TRN = ((page << 6) & 0xFF00) >> 8; //8 bit address high 
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.TRSTAT) { // Wait for empty TX buffer
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.ACKSTAT) {  // ACK high address
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }   
    I2C1TRN = (page << 6) & 0x00FF; // 8 bit address low
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.TRSTAT) {
    if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    } 
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1STATbits.ACKSTAT) {  // ACK low address
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
    }
    int i;
    for (i = 0; i < length; i++){
        I2C1TRN = data[i]; // Send data
        previousTime = FreeRunningTimer_GetMilliSeconds();
        while (I2C1STATbits.TRSTAT) {
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
                return ERROR;
        } 
        previousTime = FreeRunningTimer_GetMilliSeconds();
        while (I2C1STATbits.ACKSTAT) {  // receive ACK
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
                return ERROR;
        }
    }
    I2C1CONbits.PEN = 1;
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (I2C1CONbits.PEN) {  // Wait for Stop
        if((FreeRunningTimer_GetMilliSeconds() - previousTime) > 5)
            return ERROR;
        }
    return SUCCESS;
}
/******************************************************************************/
/*                                Test Harness                                */
/******************************************************************************/
#ifdef TEST_HARNESS_NVM_INIT
#define TEST_ADDRESS 0x3333
void main(void){
    BOARD_Init();
    Protocol_Init();
    FreeRunningTimer_Init();
    NonVolatileMemory_Init();
    while(1){
        I2C1CONbits.SEN = 1;
        while (I2C1CONbits.SEN) {}
        I2C1TRN = (TEST_ADDRESS << 1 ) & 0xFFFE;
        while (I2C2STATbits.TBF) {}
    }
    BOARD_End();
}
#endif

#ifdef TEST_HARNESS_NVM_WRITE_BYTE
void main(void){
    BOARD_Init();
    Protocol_Init();
    FreeRunningTimer_Init();
    NonVolatileMemory_Init();
    unsigned char message[MAXPAYLOADLENGTH];
    sprintf(message,"NVM_WRITE_BYTE compiled on: %s %s\n", __DATE__,__TIME__);
    Protocol_SendDebugMessage(message);
    unsigned char payload[MAXPAYLOADLENGTH];
    unsigned char messageID;
    char status;
    while(1){
        if(Protocol_IsMessageAvailable() == TRUE){
            messageID = Protocol_ReadNextID();
            Protocol_GetPayload(payload);
            if (messageID == ID_NVM_WRITE_BYTE){
                int address = (payload[2] << 8) | payload[3];
                unsigned char data = payload[4];
                status = NonVolatileMemory_WriteByte(address, data);
                if (status == SUCCESS){
                    unsigned char null = 0x00;
                    Protocol_SendMessage(1, ID_NVM_WRITE_BYTE_ACK, &null);
                }
            }
        }
    }
    BOARD_End();
}
#endif

#ifdef TEST_HARNESS_NVM_READ_BYTE
void main(void){
    BOARD_Init();
    Protocol_Init();
    FreeRunningTimer_Init();
    NonVolatileMemory_Init();
    unsigned char message[MAXPAYLOADLENGTH];
    sprintf(message,"NVM_WRITE_BYTE compiled on: %s %s\n", __DATE__,__TIME__);
    Protocol_SendDebugMessage(message);
    unsigned char payload[MAXPAYLOADLENGTH];
    unsigned char messageID;
    char status;
    while(1){
        if(Protocol_IsMessageAvailable() == TRUE){
            messageID = Protocol_ReadNextID();
            Protocol_GetPayload(payload);
            if (messageID == ID_NVM_WRITE_BYTE){
                int address = (payload[2] << 8) | payload[3];
                unsigned char data = payload[4];
                status = NonVolatileMemory_WriteByte(address, data);
                if (status == SUCCESS){
                    unsigned char null = 0x00;
                    Protocol_SendMessage(1, ID_NVM_WRITE_BYTE_ACK, &null);
                }
            }
            else if (messageID == ID_NVM_READ_BYTE){
                int address = (payload[2] << 8) | payload[3];
                unsigned char dataRead = NonVolatileMemory_ReadByte(address);
                if (status == SUCCESS){
                    Protocol_SendMessage(1, ID_NVM_READ_BYTE_RESP, &dataRead);
                }
            }
        }
    }
    BOARD_End();
}
#endif

#ifdef TEST_HARNESS_NVM_READ_PAGE
#define MAXDATALENGTH 64
void main(void){
    BOARD_Init();
    Protocol_Init();
    FreeRunningTimer_Init();
    NonVolatileMemory_Init();
    unsigned char message[MAXPAYLOADLENGTH];
    sprintf(message,"NVM_READ_PAGE compiled on: %s %s\n", __DATE__,__TIME__);
    Protocol_SendDebugMessage(message);
    unsigned char payload[MAXPAYLOADLENGTH];
    unsigned char messageID;
    char status;
    while(1){
        if(Protocol_IsMessageAvailable() == TRUE){
            messageID = Protocol_ReadNextID();
            Protocol_GetPayload(payload);
            if (messageID == ID_NVM_WRITE_BYTE){
                int address = (payload[2] << 8) | payload[3];
                unsigned char data = payload[4];
                status = NonVolatileMemory_WriteByte(address, data);
                if (status == SUCCESS){
                    unsigned char null = 0x00;
                    Protocol_SendMessage(1, ID_NVM_WRITE_BYTE_ACK, &null);
                }
            }
            else if (messageID == ID_NVM_READ_BYTE){
                int address = (payload[2] << 8) | payload[3];
                unsigned char dataRead = NonVolatileMemory_ReadByte(address);
                Protocol_SendMessage(1, ID_NVM_READ_BYTE_RESP, &dataRead);
            }
            else if (messageID == ID_NVM_READ_PAGE){
                int address = (payload[2] << 8) | payload[3];
                unsigned char dataRead[MAXDATALENGTH];
                int status = NonVolatileMemory_ReadPage(address, MAXDATALENGTH, dataRead);
                if (status == SUCCESS)
                    Protocol_SendMessage(MAXDATALENGTH, ID_NVM_READ_PAGE_RESP, dataRead);
            }
        }
    }
    BOARD_End();
}
#endif
#ifdef TEST_HARNESS_NVM_WRITE_PAGE
#define MAXDATALENGTH 64
void main(void){
    BOARD_Init();
    Protocol_Init();
    FreeRunningTimer_Init();
    NonVolatileMemory_Init();
    unsigned char message[MAXPAYLOADLENGTH];
    sprintf(message,"NVM_READ_PAGE compiled on: %s %s\n", __DATE__,__TIME__);
    Protocol_SendDebugMessage(message);
    unsigned char payload[MAXPAYLOADLENGTH];
    unsigned char dataWrite[MAXDATALENGTH];
    unsigned char messageID;
    char status;
    while(1){
        if(Protocol_IsMessageAvailable() == TRUE){
            messageID = Protocol_ReadNextID();
            Protocol_GetPayload(payload);
            if (messageID == ID_NVM_WRITE_BYTE){
                int address = (payload[2] << 8) | payload[3];
                unsigned char data = payload[4];
                status = NonVolatileMemory_WriteByte(address, data);
                if (status == SUCCESS){
                    unsigned char null = 0x00;
                    Protocol_SendMessage(1, ID_NVM_WRITE_BYTE_ACK, &null);
                }
            }
            else if (messageID == ID_NVM_READ_BYTE){
                int address = (payload[2] << 8) | payload[3];
                unsigned char dataRead = NonVolatileMemory_ReadByte(address);
                Protocol_SendMessage(1, ID_NVM_READ_BYTE_RESP, &dataRead);
            }
            else if (messageID == ID_NVM_READ_PAGE){
                int address = (payload[2] << 8) | payload[3];
                unsigned char dataRead[MAXDATALENGTH];
                int status = NonVolatileMemory_ReadPage(address, MAXDATALENGTH, dataRead);
                if (status == SUCCESS)
                    Protocol_SendMessage(MAXDATALENGTH, ID_NVM_READ_PAGE_RESP, dataRead);
            }
            else if (messageID == ID_NVM_WRITE_PAGE){
                int address = (payload[2] << 8) | payload[3];
                int i;
                int payloadSize = sizeof(payload);
                for (i = 0; i < payloadSize; i++){
                    dataWrite[i] = payload[i + 4];
                }
                int status = NonVolatileMemory_WritePage(address, MAXDATALENGTH, dataWrite);
                if (status == SUCCESS)
                    Protocol_SendMessage(MAXDATALENGTH, ID_NVM_WRITE_PAGE_ACK, dataWrite);
            }
        }
    }
    BOARD_End();
}
#endif