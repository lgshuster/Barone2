#include <proc/p32mx340f512h.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <xc.h>
#include "RotaryEncoder.h"
#include "FreeRunningTimer.h"
#include "MessageIDs.h"
#include "Protocol.h"
#include "BOARD.h"
#include "NOP.h"

#define CS LATDbits.LATD3    // PIN 9

//#define TEST_HARNESS_ROTARY_ENCODER

unsigned short commandFrame, packet, parityBit, data;
unsigned char message[MAXPAYLOADLENGTH];

int RotaryEncoder_Init(char interfaceMode){
    if (interfaceMode == ENCODER_BLOCKING_MODE){
        /*Designate I/O pins*/
        TRISDbits.TRISD3 = 0;   // Set CS pin to output, active low
        /*Clear registers*/
        SPI2CON = 0;
        SPI2BRG = 3; // Set baud rate 5MHz
        /*Set registers*/
        SPI2CONbits.MSTEN = 1; // Set SPI2 as master
        SPI2CONbits.MODE16 = 1; // 16-bit communication
        SPI2CONbits.SMP = 1;
        SPI2CONbits.ON = 1;
        
        CS = 1; // Set CS idle HIGH
        NOP_delay(20000);
        return SUCCESS;
    }
    else
        return ERROR;
}

unsigned short CalculateParityBit(unsigned short in){
    unsigned short p = 0;
    unsigned short i;
    for (i = 1; i < 0x8000; i++){   // skips MSB to store parity
        if (in & i) // increment p if 'in' is set
            p++;
        i = i << 1; // Shift to next bit for checking
    }
    p = p % 2;  // p is set if 'in' is even
    return p;
}

unsigned short RotaryEncoder_ReadRawAngle(void){
    data = SPI2BUF; // Clear buffer
    CS = 0;
    NOP_delay(390); // 350ns delay
    SPI2BUF = 0xFFFF;   // read rotary angle
    while (SPI2STATbits.SPIRBF == 0) {} // Wait until all 16 bits of data received by slave
    data = SPI2BUF; // Clear buffer
    CS = 1;
    NOP_delay(390);
    CS = 0;
    NOP_delay(390);
    SPI2BUF = 0xFFFF;
    while (SPI2STATbits.SPIRBF == 0) {} // Wait until all 16 bits of data received by slave
    data = SPI2BUF; // read buffer
    CS = 1;
    return data;
}

#ifdef TEST_HARNESS_ROTARY_ENCODER
void main (void){
    BOARD_Init();
    Protocol_Init();
    FreeRunningTimer_Init();
    RotaryEncoder_Init(ENCODER_BLOCKING_MODE);
    sprintf(message,"Rotary Encoder compiled on: %s %s\n", __DATE__,__TIME__);
    Protocol_SendDebugMessage(message);
    unsigned short rotaryData, address;
    int previousTime, currentTime;
    previousTime = FreeRunningTimer_GetMilliSeconds();
    NOP_delay(20000);  // 18ms delay, power on time
    while(1){
        currentTime = FreeRunningTimer_GetMilliSeconds();
        if ((currentTime - previousTime) > 10){    // 100Hz ping
            rotaryData = RotaryEncoder_ReadRawAngle();
            address = rotaryData & 0x3FFF;
//            address /= 216;
            Protocol_SendMessage(2, ID_ROTARY_ANGLE, &address); 
            previousTime = FreeRunningTimer_GetMilliSeconds();
        }
    }
    BOARD_End();
}
#endif