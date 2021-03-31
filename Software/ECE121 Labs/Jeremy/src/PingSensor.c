#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <GenericTypeDefs.h>
#include <xc.h>
#include <sys/attribs.h>

#include "Protocol.h"
#include "BOARD.h"
#include "MessageIDs.h"
#include "FreeRunningTimer.h"

#define PIN5 LATDbits.LATD1

static unsigned int falling;
static unsigned short timeup;
static unsigned short timetot;

/**
 * @Function PingSensor_Init(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief initializes hardware for PingSensor with the needed interrupts */
int PingSensor_Init(void)
{
    T4CON = 0; //sets up timer4
    T4CON = 0x0060; // 16-bit mode, 1:64 prescale, ticks up 8/5us
    TMR4 = 0; // Clear timer registers for TMR4
    PR4 = 37500; //sets 16 bit period to 37500 to get 60ms interrupts
    IFS0bits.T4IF = 0; //clears interrupt flag
    IPC4bits.T4IP = 5; //sets timer4 interrupt priority
    IEC0bits.T4IE = 1; //enables timer4 interrupts,
    T4CON |= 0x8000; //starts timer 4

    TRISDbits.TRISD10 = 1; //sets pin8 as input for IC3
    TRISDbits.TRISD1 = 0; //sets pin5 as output

    T2CON = 0; //sets up timer2
    T2CON = 0x0030; // 16-bit mode, 1:8 prescale, ticks up every .2us
    TMR2 = 0; // Clear timer registers for TMR2
    PR2 = 0xEA60; //sets 16 bit period to 60000 to get 12ms rollovers

    IC3CON = 0;
    IC3CONbits.FEDGE = 1; //first event is rising edge
    IC3CONbits.ICTMR = 1; //sets to timer2
    IC3CONbits.ICM = 6; //capture event on every edge after first event
    IC3CONbits.ICI = 0; //interrupt on every edge

    IFS0bits.IC3IF = 0; //clears IC3 interrupt flag
    IPC3bits.IC3IP = 4; //sets high IC3 interrupt priority
    IEC0bits.IC3IE = 1; //enables IC3 interrupts

    T2CON |= 0x8000; //starts timer 2
    IC3CONbits.ON = 1; //turns on input capture into pin 8
    return (1);
}

/**
 * @Function int PingSensor_GetDistance(void)
 * @param None
 * @return Unsigned Short corresponding to distance in millimeters */
unsigned short PingSensor_GetDistance(void)
{
    unsigned short distance = timetot / 5 * 343 / 2 / 1000; //gets distance in millimeters
    return (distance);
}

void __ISR(_INPUT_CAPTURE_3_VECTOR) __IC3Interrupt(void)
{
    if (falling == 0) { //read first time on rising edge
        timeup = IC3BUF;
        falling = 1;
    } else {
        timetot = IC3BUF - timeup; //gets distance from previous time and falling edge time
        falling = 0;
    }
    IFS0bits.IC3IF = 0;
    return;
}

void __ISR(_TIMER_4_VECTOR) Timer4IntHandler(void)
{
    if (PR4 == 37500){
        PIN5 = 1; //changes period to about 80us to set slave select low again
        PR4 = 50;
    }else{
        PIN5 = 0; //puts period back to normal to get appropriate delay before next pulse
        PR4 = 37500;
    }
    IFS0bits.T4IF = 0; //clears interrupt flag
    return;
}

#ifdef PING_TEST //use with PING_PROTOCOL_TEST in Protocol.c

int main(void)
{
    BOARD_Init();
    Protocol_Init(); //initializes board and UART1 protocol
    LEDS_INIT();

    char testMessage[MAXPAYLOADLENGTH];
    sprintf(testMessage, "Ping Sensor Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(testMessage);

    unsigned int target = 100;
    PingSensor_Init();
    FreeRunningTimer_Init();
    while (1) { //main loop
        if (FreeRunningTimer_GetMilliSeconds() >= target) {
            target += 100;
            unsigned short temp = PingSensor_GetDistance();
            unsigned char dis1 = temp >> 8;
            unsigned char dis2 = (temp << 8) >> 8; //returns short distance into character array
            unsigned char array1[MAXPAYLOADLENGTH] = {dis1, dis2};
            void *arpt1 = array1;
            Protocol_SendMessage(2, ID_PING_DISTANCE, arpt1); //sends ping distance message
        }
    }
}
#endif