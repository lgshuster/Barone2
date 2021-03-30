#include <proc/p32mx340f512h.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <xc.h>
#include "RotaryEncoder.h"
#include "FreeRunningTimer.h"
#include "MessageIDs.h"
#include "Protocol.h"
#include "BOARD.h"

#define CS LATDbits.LATD3    // PIN 9
#define ANGLECOM 0x3FFF

//#define TEST_HARNESS_LAB_4
#define MAX_RATE 1000
#define ENCODER_ROLLOVER 0x3FFF

static unsigned short commandFrame, packet, parityBit, data;
static unsigned char message[MAXPAYLOADLENGTH];
static unsigned short SPI_bufferData;
static int SPIMode;

unsigned short ParityBitConvert(unsigned short in) {
    unsigned short p = 0;
    unsigned short i;
    for (i = 1; i < 0x8000; i++) { // skips MSB to store parity
        if (in & i) // increment p if 'in' is set
            p++;
        i = i << 1; // Shift to next bit for checking
    }
    p = p % 2; // p is set if 'in' is even
    return ((p << 15) | in);
}

int RotaryEncoder_Init(char interfaceMode) {
    if (interfaceMode == ENCODER_BLOCKING_MODE) {
        /*Designate I/O pins*/
        TRISDbits.TRISD3 = 0; // Set CS pin to output, active low
        /*Clear registers*/
        SPI2CON = 0;
        SPI2BRG = 3; // Set baud rate 5MHz
        /*Set registers*/
        SPI2CONbits.MSTEN = 1; // Set SPI2 as master
        SPI2CONbits.MODE16 = 1; // 16-bit communication
        SPI2CONbits.SMP = 1; // Input data sampled at the end of output data time
        SPI2CONbits.ON = 1;

        CS = 1; // Set CS idle HIGH
        SPIMode = ENCODER_BLOCKING_MODE;
        sprintf(message, "Rotary Encoder blocking mode\n");
        Protocol_SendDebugMessage(message);
        return SUCCESS;
    }
    if (interfaceMode == ENCODER_INTERRUPT_MODE) {
        TRISDbits.TRISD3 = 0; // Set CS pin to output, active low
        /*SPI 2 control register */
        SPI2CON = 0;
        SPI2BRG = 3; // Set baud rate 5MHz
        SPI2CONbits.MSTEN = 1; // Set SPI2 as master
        SPI2CONbits.MODE16 = 1; // 16-bit communication
        SPI2CONbits.SMP = 1;
        /*Timer 2 register*/
        T2CON = 0;
        T2CONbits.TCKPS = 0b011; // Pre-scale value = 1:8
        PR2 = 10000; // 2ms rollover
        /*T2 interrupt*/
        IFS0bits.T2IF = 0;
        IPC2bits.T2IP = 1; //T2 Level 1 priority
        /*SPI2 RX interrupt*/
        IFS1bits.SPI2RXIF = 0;
        IPC7bits.SPI2IP = 2; //SPI2RX Level 2 priority
        /*Turn on SPI2 register and T2 interrupt*/
        IEC0bits.T2IE = 1;
        IEC1bits.SPI2RXIE = 1;
        SPI2CONbits.ON = 1;
        T2CONbits.ON = 1;
        RotaryEncoder_ReadRawAngle();
        sprintf(message, "Rotary Encoder interrupt mode\n");
        Protocol_SendDebugMessage(message);
        return SUCCESS;
    } else
        return ERROR;
}

unsigned short RotaryEncoder_ReadRawAngle(void) {
    if (IEC1bits.SPI2RXIE) {
        return SPI_bufferData;
    }
    int rotaryEncoder_previousTime = FreeRunningTimer_GetMicroSeconds();
    data = SPI2BUF; // Clear buffer
    CS = 0;
    while ((FreeRunningTimer_GetMicroSeconds() - rotaryEncoder_previousTime) < 350) {
    }
    rotaryEncoder_previousTime = FreeRunningTimer_GetMicroSeconds();
    SPI2BUF = 0xFFFF; // read rotary angle
    while (SPI2STATbits.SPIRBF == 0) {
    } // Wait until all 16 bits of data received by slave
    data = SPI2BUF; // Clear buffer
    CS = 1;
    while ((FreeRunningTimer_GetMicroSeconds() - rotaryEncoder_previousTime) < 350) {
    }
    rotaryEncoder_previousTime = FreeRunningTimer_GetMicroSeconds();
    CS = 0;
    while ((FreeRunningTimer_GetMicroSeconds() - rotaryEncoder_previousTime) < 350) {
    }
    rotaryEncoder_previousTime = FreeRunningTimer_GetMicroSeconds();
    SPI2BUF = 0xFFFF;
    while (SPI2STATbits.SPIRBF == 0) {
    } // Wait until all 16 bits of data received by slave
    data = SPI2BUF; // read buffer
    CS = 1;
    return data;
}

/*Interrupt handlers*/
void __ISR(_TIMER_2_VECTOR) Timer2IntHandler(void) {
    IFS0bits.T2IF = 0;
    CS = 0;
    SPI2BUF = ParityBitConvert(ANGLECOM | 0x4000);
}

void __ISR(_SPI_2_VECTOR) __SPI2Interrupt(void) {
    IFS1bits.SPI2RXIF = 0;
    SPI_bufferData = SPI2BUF & 0x3FFF; // Read lower 14-bits of encoder
    CS = 1;
}

#ifdef TEST_HARNESS_LAB_4

void main(void) {
    BOARD_Init();
    Protocol_Init();
    RotaryEncoder_Init(ENCODER_INTERRUPT_MODE);
    LEDS_INIT();
    FreeRunningTimer_Init();
    sprintf(message, "Rotary Encoder Lab 4 compiled on: %s %s\n", __DATE__, __TIME__);
    Protocol_SendDebugMessage(message);
    unsigned short parityBit_currentAngle, currentAngle, previousAngle, angularVelocity;
    int previousTime, currentTime;
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while ((FreeRunningTimer_GetMilliSeconds() - previousTime) < 200) {
    } // 200ms delay 
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (1) {
        currentTime = FreeRunningTimer_GetMilliSeconds();
        if ((currentTime - previousTime) > 10) { // 100Hz ping
            currentAngle = RotaryEncoder_ReadRawAngle();
            parityBit_currentAngle = ParityBitConvert(currentAngle); // Add parity bit to payload
            Protocol_SendMessage(2, ID_ROTARY_ANGLE, &parityBit_currentAngle);
            angularVelocity = currentAngle - previousAngle; // Velocity
            previousAngle = currentAngle; // Save angle as previous
            if (angularVelocity > MAX_RATE) // Negative velocity
                angularVelocity -= ENCODER_ROLLOVER;
            else if (angularVelocity < -MAX_RATE) // Positive velocity
                angularVelocity += ENCODER_ROLLOVER;
            //            sprintf(message,"Angular Velocity: %x\n", angularVelocity);
            //            Protocol_SendDebugMessage(message);
            previousTime = FreeRunningTimer_GetMilliSeconds();
        }
    }
    BOARD_End();
}
#endif