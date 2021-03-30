#include "BOARD.h"
#include "Protocol.h"
#include "FreeRunningTimer.h"
#include "MessageIDs.h"
#include "FreeRunningTimer.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <GenericTypeDefs.h>
#include <xc.h>
#include <sys/attribs.h>

#define SLAVE LATDbits.LATD8 //slave pin 2
#define READANGLEMESSAGE 0xFFFF
#define MAXMOTORSPEED 1000
static unsigned short angle;

void NOP_10us();

/*
 * @Function RCServo_Init(char interfaceMode)
 * @param interfaceMode, one of the two #defines determining the interface
 * @return SUCCESS or ERROR
 * @brief initializes hardware in appropriate mode along with the needed interrupts */

int RotaryEncoder_Init(char interfaceMode)
{
    SPI2CON = 0; //turns off and resets SPI control register

    SPI2CONbits.MSTEN = 1; //sets master mode
    SPI2CONbits.MODE16 = 1; //sets 16 bit mode
    SPI2CONbits.SMP = 1; //sets sampling at end of output


    SPI2CONbits.CKP = 0; //idle mode is low level SPI Mode 1
    SPI2CONbits.CKE = 0;

    SPI2BRG = 0x11; //sets 5mhz clock

    if (interfaceMode) {
        IFS1bits.SPI2RXIF = 0; //enables SPI2 receive interrupts
        IPC7bits.SPI2IP = 2;
        IEC1bits.SPI2RXIE = 1;
    }

    SPI2CONbits.ON = 1; //turns SPI on

    TRISDbits.TRISD8 = 0; //sets slave pin 2 to output mode
    SLAVE = 1; //slave select high pin 2

    if (interfaceMode) {//sends first readangle message
        NOP_10us(); //
        SLAVE = 0; //slave select down
        NOP_10us(); //delays for 10us
        SPI2BUF = READANGLEMESSAGE; //sends read angle message


        T2CON = 0; //sets up timer2
        T2CON = 0x0030; // 16-bit mode, 1:8 prescale, ticks up .2us
        TMR2 = 0; // Clear timer registers for TMR2
        PR2 = 10000; //sets 16 bit period to 10000 to get 2ms interrupts
        IFS0bits.T2IF = 0; //clears interrupt flag
        IPC2bits.T2IP = 5; //sets timer2 interrupt priority
        IEC0bits.T2IE = 1; //enables timer2 interrupts,
        T2CON |= 0x8000; //starts timer 2
    }
    return (0);
}

/**
 * @Function int RotaryEncoder_ReadRawAngle(void)
 * @param None
 * @return 14-bit number representing the raw encoder angle (0-16384) */
unsigned short RotaryEncoder_ReadRawAngle(void)
{
    if (IEC1bits.SPI2RXIE){ //if interrupt enabled, return static angle variable
        return(angle);
    }
    unsigned short read = SPI2BUF;
    unsigned short readangle = 0xFFFF; //reads angle from encoder, parity is 1, 1 for read
    NOP_10us(); //delays for 1us
    SLAVE = 0; //slave select down
    NOP_10us(); //delays for 10us
    SPI2BUF = readangle; //sends read angle message
    while (SPI2STATbits.SPIRBF == 0) { //allows slave to read message and respond
    }
    read = SPI2BUF;
    SLAVE = 1; //slave select high
    NOP_10us(); //delay for about 1us
    SLAVE = 0; //slave select down
    NOP_10us();
    SPI2BUF = readangle; //sends message
    while (SPI2STATbits.SPIRBF == 0) { //allows slave to read message and respond
    }
    unsigned short data = SPI2BUF; //reads angle
    SLAVE = 1;
    return (data & 0x3FFF);
}

int Parity_Check(int in)
{
    int i;
    int p = 0;
    for (i = 1; i < 0x8000; i = i << 1) {
        if (i & in) {
            p = p + 1;
        }
    }
    p = p % 2;
    return (p);
}

void NOP_10us() //counts up to simulate a NOP, takes about 10us
{
    int j;
    for (j = 0; j < 400; j++) {
    }
    return;
}

void __ISR(_SPI_2_VECTOR) __SPI2Interrupt(void)
{
    IFS1bits.SPI2RXIF = 0;
    angle = SPI2BUF; //reads angle
    angle = angle & 0x3FFF;
    SLAVE = 1; //slave select high
    return;
}

void __ISR(_TIMER_2_VECTOR) Timer2IntHandler(void)
{
    IFS0bits.T2IF = 0;
    SLAVE = 0; //slave select down
    SPI2BUF = READANGLEMESSAGE; //sends read angle message
    return;
}

//#define SPI_TEST_INTERRUPT
#ifdef SPI_TEST_INTERRUPT

int main(void)
{
    BOARD_Init();
    Protocol_Init();
    LEDS_INIT();

    char testMessage[MAXPAYLOADLENGTH];
    sprintf(testMessage, "Interrupt Encoder Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(testMessage);

    FreeRunningTimer_Init();

    RotaryEncoder_Init(1);

    unsigned char payload[MAXPAYLOADLENGTH];
    unsigned int target = 10; //sends 100hz
    unsigned short previous;
    int rate;
    while (1) {
        if (FreeRunningTimer_GetMilliSeconds() >= target) {
            target += 10;

            rate = angle - previous;
            if (rate > MAXMOTORSPEED) {
                rate = rate - 0x3FFF;
            }
            if (rate < (MAXMOTORSPEED * -1)) {
                rate = rate + 0x3FFF;
            }
            payload[1] = (rate & 0xFF); // big endian conversions to return correct value
            payload[0] = ((rate & 0xFF00) >> 8);
            Protocol_SendMessage(2, ID_REPORT_RATE, &payload); //returns angle in message

            payload[1] = (angle & 0xFF); // big endian conversions to return correct value
            payload[0] = ((angle & 0xFF00) >> 8);
            Protocol_SendMessage(2, ID_ROTARY_ANGLE, &payload); //returns angle in message
            previous = angle;
        }
    }
}

#endif
#ifdef SPI_TEST_BLOCKING

int main(void)
{
    BOARD_Init();
    Protocol_Init();

    char testMessage[MAXPAYLOADLENGTH];
    sprintf(testMessage, "Encoder Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(testMessage);

    FreeRunningTimer_Init();

    RotaryEncoder_Init(0);

    unsigned int target = 10; //sends 100hz
    while (1) {
        if (FreeRunningTimer_GetMilliSeconds() >= target) {
            target += 10;
            unsigned short data = RotaryEncoder_ReadRawAngle(); //gets raw angle
            unsigned char payload[MAXPAYLOADLENGTH]; //sets up payload char array
            void *ptr = &payload;

            payload[0] = (data >> 8); // big endian conversions to return correct value
            payload[1] = ((data << 4) >> 8);
            Protocol_SendMessage(2, ID_ROTARY_ANGLE, ptr); //returns angle in message
        }
    }
}
#endif

#ifdef ParityCheck

int main(void)
{
    BOARD_Init();

    int parity1 = Parity_Check(0x0801);
    int parity2 = Parity_Check(0x0802);
    int parity3 = Parity_Check(0x0803);



    printf("%d%d%d", parity1, parity2, parity3);

    BOARD_End();
    return (0);
}
#endif