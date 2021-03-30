#include "Board.h"
#include "Protocol.h"
#include "FreeRunningTimer.h"
#include "MessageIDs.h"
/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <GenericTypeDefs.h>
#include <xc.h>
#include <sys/attribs.h>

#define DEV_ADDR 0b1010000
/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/
static unsigned char bytedata; //variables for reading byte and page data
static unsigned char pagedata[64];

/**
 * @Function NonVolatileMemory_Init(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief initializes I2C for usage */
int NonVolatileMemory_Init(void)
{
    I2C1CON = 0;
    I2C1BRG = 0x00C5; //sets baud rate for 100khz baud given 40mhz clock
    I2C1CONbits.ON = 1; //turns on I2C

    I2C1CONbits.PEN = 1;
    while (I2C1CONbits.PEN) { //stop condition to initialize at the beginning
    }

    return (1);
}

/**
 * @Function NonVolatileMemory_ReadByte(int address)
 * @param address, device address to read from
 * @return value at said address
 * @brief reads one byte from device
 * @warning Default value for this EEPROM is 0xFF */
unsigned char NonVolatileMemory_ReadByte(int address)
{
    I2C1CONbits.SEN = 1;
    while (I2C1CONbits.SEN) { //start condition
    }

    I2C1TRN = (DEV_ADDR << 1); //sends address of device + write bit at end
    while (I2C1STATbits.TRSTAT) {//waits for transmission to finish
    }
    while (I2C1STATbits.ACKSTAT) { //if ACK not received, return error
        return (ERROR);
    }

    I2C1TRN = ((address >> 8) & 0xFF); //sends high byte of address
    while (I2C1STATbits.TRSTAT) {//waits for transmission to finish
    }
    while (I2C1STATbits.ACKSTAT) { //if ACK not received, return error
        return (ERROR);
    }

    I2C1TRN = address & 0xFF; //sends low byte of address
    while (I2C1STATbits.TRSTAT) {//waits for transmission to finish
    }
    while (I2C1STATbits.ACKSTAT) { //if ACK not received, return error
        return (ERROR);
    }

    I2C1CONbits.RSEN = 1;
    while (I2C1CONbits.RSEN) { //start condition again to read
    }

    I2C1TRN = (DEV_ADDR << 1) | 1; //sends address of device + read bit at end
    while (I2C1STATbits.TRSTAT) {//waits for transmission to finish
    }
    while (I2C1STATbits.ACKSTAT) { //if ACK not received, return error
        return (ERROR);
    }

    I2C1CONbits.RCEN = 1; //enables receiving, waits until whole data received
    while (I2C1CONbits.RCEN) {
    }

    I2C1CONbits.ACKDT = 1; //will send nack after data received
    I2C1CONbits.ACKEN = 1; //sends and waits for nack to be received
    while (I2C1CONbits.ACKEN) {
    }

    I2C1CONbits.PEN = 1;
    while (I2C1CONbits.PEN) { //stop condition
    }

    bytedata = I2C1RCV; //gets received data into variable
    return (bytedata);
}

/**
 * @Function char NonVolatileMemory_WriteByte(int address, unsigned char data)
 * @param address, device address to write to
 * @param data, value to write at said address
 * @return SUCCESS or ERROR
 * @brief writes one byte to device */
char NonVolatileMemory_WriteByte(int address, unsigned char data)
{
    I2C1CONbits.SEN = 1;
    while (I2C1CONbits.SEN) { //start condition
    }

    I2C1TRN = (DEV_ADDR << 1); //sends address of device + write bit at end
    while (I2C1STATbits.TRSTAT) {//waits for transmission to finish
    }
    while (I2C1STATbits.ACKSTAT) { //if ACK not received, return error
        return (ERROR);
    }

    I2C1TRN = ((address >> 8) & 0xFF); //sends high byte of address
    while (I2C1STATbits.TRSTAT) {//waits for transmission to finish
    }
    while (I2C1STATbits.ACKSTAT) { //if ACK not received, return error
        return (ERROR);
    }

    I2C1TRN = address & 0xFF; //sends low byte of address
    while (I2C1STATbits.TRSTAT) {//waits for transmission to finish
    }
    while (I2C1STATbits.ACKSTAT) { //if ACK not received, return error
        return (ERROR);
    }

    I2C1TRN = data; //sends byte data to write to address
    while (I2C1STATbits.TRSTAT) {//waits for transmission to finish
    }
    while (I2C1STATbits.ACKSTAT) { //if ACK not received, return error
        return (ERROR);
    }

    I2C1CONbits.PEN = 1;
    while (I2C1CONbits.PEN == 1) { //stop condition
    }

    return (SUCCESS);
}

/**
 * @Function int NonVolatileMemory_ReadPage(int page, char length, unsigned char data[])
 * @param page, page value to read from
 * @param length, value between 1 and 64 bytes to read
 * @param data, array to store values into
 * @return SUCCESS or ERROR
 * @brief reads bytes in page mode, up to 64 at once
 * @warning Default value for this EEPROM is 0xFF */
int NonVolatileMemory_ReadPage(int page, char length, unsigned char data[])
{
    I2C1CONbits.SEN = 1;
    while (I2C1CONbits.SEN) { //start condition
    }

    I2C1TRN = (DEV_ADDR << 1); //sends address of device + write bit at end
    while (I2C1STATbits.TRSTAT) {//waits for transmission to finish
    }
    while (I2C1STATbits.ACKSTAT) { //if ACK not received, return error
        return (ERROR);
    }

    int addr = page << 6;
    I2C1TRN = ((addr >> 8) & 0xFF); //sends high byte of address
    while (I2C1STATbits.TRSTAT) {//waits for transmission to finish
    }
    while (I2C1STATbits.ACKSTAT) { //if ACK not received, return error
        return (ERROR);
    }

    I2C1TRN = addr & 0xFF; //sends low byte of address
    while (I2C1STATbits.TRSTAT) {//waits for transmission to finish
    }
    while (I2C1STATbits.ACKSTAT) { //if ACK not received, return error
        return (ERROR);
    }

    I2C1CONbits.RSEN = 1;
    while (I2C1CONbits.RSEN) { //start condition again to read
    }

    I2C1TRN = (DEV_ADDR << 1) | 1; //sends address of device + read bit at end
    while (I2C1STATbits.TRSTAT) {//waits for transmission to finish
    }
    while (I2C1STATbits.ACKSTAT) { //if ACK not received, return error
        return (ERROR);
    }

    I2C1CONbits.ACKDT = 0; //sends ACK instead to slave
    int i;
    for (i = 0; i < length; i++) {//for 64 bytes of data

        I2C1CONbits.RCEN = 1; //enables receiving, waits until whole data received
        while (I2C1CONbits.RCEN) {
        }
        data[i] = I2C1RCV;
        while (I2C1STATbits.RBF) {
        }
        if(i == (length-1)){
            I2C1CONbits.ACKDT = 1;
        }
        I2C1CONbits.ACKEN = 1; //sends and waits for ack to be received
        while (I2C1CONbits.ACKEN) {
        }
    }

    I2C1CONbits.PEN = 1;
    while (I2C1CONbits.PEN) { //stop condition
    }

    return (SUCCESS);
}

/**
 * @Function char int NonVolatileMemory_WritePage(int page, char length, unsigned char data[])
 * @param address, device address to write to
 * @param data, value to write at said address
 * @return SUCCESS or ERROR
 * @brief writes one byte to device */
int NonVolatileMemory_WritePage(int page, char length, unsigned char data[])
{
    I2C1CONbits.SEN = 1;
    while (I2C1CONbits.SEN) { //start condition
    }

    I2C1TRN = (DEV_ADDR << 1); //sends address of device + write bit at end
    while (I2C1STATbits.TRSTAT) {//waits for transmission to finish
    }
    while (I2C1STATbits.ACKSTAT) { //if ACK not received, return error
        return (ERROR);
    }

    int addr = page * 64;
    I2C1TRN = ((addr >> 8) & 0xFF); //sends high byte of address
    while (I2C1STATbits.TRSTAT) {//waits for transmission to finish
    }
    while (I2C1STATbits.ACKSTAT) { //if ACK not received, return error
        return (ERROR);
    }

    I2C1TRN = addr & 0xFF; //sends low byte of address
    while (I2C1STATbits.TRSTAT) {//waits for transmission to finish
    }
    while (I2C1STATbits.ACKSTAT) { //if ACK not received, return error
        return (ERROR);
    }

    int i;
    for (i = 0; i < length; i++) { //for 64 bytes of data
        I2C1TRN = data[i]; //sends byte data to write to address
        while (I2C1STATbits.TRSTAT) {//waits for transmission to finish
        }
        while (I2C1STATbits.ACKSTAT) { //if ACK not received, return error
            return (ERROR);
        }
    }

    I2C1CONbits.PEN = 1;
    while (I2C1CONbits.PEN == 1) { //stop condition
    }

    return (SUCCESS);
}

//#define NVM_TEST
#ifdef NVM_TEST

int main(void)
{
    BOARD_Init();
    Protocol_Init(); //initializes modules
    LEDS_INIT();
    NonVolatileMemory_Init();

    char testMessage[MAXPAYLOADLENGTH];
    sprintf(testMessage, "NVM Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(testMessage); //sends debug  message

    unsigned int addr; //address for reading
    unsigned char addw[MAXPAYLOADLENGTH]; //address for writing
    void *ptr; //void ptr to input into protocol functions
    int success; //success conditions of read/write functions
    unsigned int writeadd; //address to write
    unsigned char writedata; //byte data to write
    unsigned char writepagedata[64]; //data to write into
    int i = 0;
    while (1) { //main loop
        if (Protocol_IsMessageAvailable()) {
            if (Protocol_ReadNextID() == ID_NVM_READ_BYTE) {
                ptr = &addr;
                Protocol_GetPayload(ptr);
                addr = Protocol_IntEndednessConversion(addr);

                success = NonVolatileMemory_ReadByte(addr); //reads value at address, returns value
                if (success == 0) { //stop condition if error happened while reading
                    I2C1CONbits.PEN = 1;
                    while (I2C1CONbits.PEN) {
                    }
                }
                ptr = &bytedata;
                Protocol_SendMessage(1, ID_NVM_READ_BYTE_RESP, ptr);
            }
            if (Protocol_ReadNextID() == ID_NVM_READ_PAGE) {
                ptr = &addr;
                Protocol_GetPayload(ptr);
                addr = Protocol_IntEndednessConversion(addr);
                success = NonVolatileMemory_ReadPage(addr, 64, pagedata); //reads value at address, returns value
                if (success == 0) { //stop condition if error happened while reading
                    I2C1CONbits.PEN = 1;
                    while (I2C1CONbits.PEN) {
                    }
                }
                ptr = &pagedata;
                Protocol_SendMessage(64, ID_NVM_READ_PAGE_RESP, ptr);
            }
            if (Protocol_ReadNextID() == ID_NVM_WRITE_BYTE) {
                ptr = &addw;
                Protocol_GetPayload(ptr);
                writeadd = (addw[0] << 24) + (addw[1] << 16) + (addw[2] << 8) + addw[3]; //gets address and data for byte write
                writedata = addw[4];
                success = NonVolatileMemory_WriteByte(writeadd, writedata); //writes data at specified address
                if (success == 0) {
                    I2C1CONbits.PEN = 1;
                    while (I2C1CONbits.PEN) { //stop condition if error happened while writing
                    }
                }
                Protocol_SendMessage(0, ID_NVM_WRITE_BYTE_ACK, NULL);
            }
            if (Protocol_ReadNextID() == ID_NVM_WRITE_PAGE) {
                ptr = &addw;
                Protocol_GetPayload(ptr);
                writeadd = (addw[0] << 24) + (addw[1] << 16) + (addw[2] << 8) + addw[3]; //gets address of page to read

                for (i = 0; i < 64; i++) {
                    writepagedata[i] = addw[i + 4]; //gets rid of address in payload
                }
                success = NonVolatileMemory_WritePage(writeadd, 64, writepagedata); //writes data at specified address
                if (success == 1) {
                    Protocol_SendMessage(0, ID_NVM_WRITE_PAGE_ACK, NULL); //sends acknowledgement if write success
                } else {
                    I2C1CONbits.PEN = 1;
                    while (I2C1CONbits.PEN) { //stop condition if error happened while writing
                    }
                    Protocol_SendDebugMessage("Write Page Error");
                }
            }
            if (I2C1STATbits.IWCOL == 1) {
                I2C1CONbits.PEN = 1;
                while (I2C1CONbits.PEN) { //stop condition if write collision on buffer
                }
                I2C1STATbits.IWCOL = 0;
                Protocol_SendDebugMessage("Collision");
            }
        }
    }
}
#endif