#include "BOARD.h"
#include "Protocol.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <GenericTypeDefs.h>
#include <xc.h>
#include <sys/attribs.h>

static unsigned int microseconds;
static unsigned int milliseconds;

/**
 * @Function TIMERS_Init(void)
 * @param none
 * @return None.
 * @brief  Initializes the timer module */
void FreeRunningTimer_Init(void)
{
    T5CON = 0;
    T5CON = 0x0030; // 16-bit mode, 1:8 prescale, ticks up 1/5us
    
    TMR5 = 0;

    PR5 = 0x1388; //sets period to 5000 to get 1ms interrupts

    IFS0bits.T5IF = 0; //clears interrupt flag
    IPC5bits.T5IP = 3; //sets timer 5 interrupt priority
    IEC0bits.T5IE = 1; //enables timer interrupts,

    T5CON |= 0x8000; //starts timer 5 16 bits
}

/**
 * Function: TIMERS_GetMilliSeconds
 * @param None
 * @return the current MilliSecond Count
 */
unsigned int FreeRunningTimer_GetMilliSeconds(void)
{
    unsigned int milli = milliseconds; //returns milliseconds according to interrupt
    return (milli);
}

/**
 * Function: TIMERS_GetMicroSeconds
 * @param None
 * @return the current MicroSecond Count
 */
unsigned int FreeRunningTimer_GetMicroSeconds(void)
{
    unsigned int micro = microseconds;
    return (micro + (TMR4 % 5)); //returns microseconds according to interrupt + TMR4/5
}

void __ISR(_TIMER_5_VECTOR, ipl3auto)Timer5IntHandler(void)
{//interrupt when timer rolls ver every 1ms
    if (IFS0bits.T5IF) {
        milliseconds += 1;
        microseconds += 1000;
    }
    IFS0bits.T5IF = 0; //clears interrupt flag
    return;
}
#ifdef TIMER_TEST //use with TIMER_TEST in FreeRunningTimer.c

int main(void)
{
    BOARD_Init();
    Protocol_Init(); //initializes board and UART1 protocol
    LEDS_INIT();

    char testMessage[MAXPAYLOADLENGTH];
    sprintf(testMessage, "Timer Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(testMessage);

    FreeRunningTimer_Init();
    unsigned int target = 2000;
    while (1) { //main loop
        if (FreeRunningTimer_GetMilliSeconds() >= target) {
            target += 2000;
            int led = !(LEDS_GET()); //flashes LED on and off every 2 seconds
            LEDS_SET(led);

            char testMessage1[MAXPAYLOADLENGTH];
            sprintf(testMessage1, "Milliseconds %d", FreeRunningTimer_GetMilliSeconds());
            Protocol_SendDebugMessage(testMessage1); //returns current millisecond value

            sprintf(testMessage1, "Microseconds %d", FreeRunningTimer_GetMicroSeconds());
            Protocol_SendDebugMessage(testMessage1); //returns current microseconds value

        }
    }
}
#endif
