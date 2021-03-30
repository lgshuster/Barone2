#include <xc.h>
#include <xc.h>
#include <stdio.h>
#include <string.h>
#include <sys/attribs.h>
#include "Protocol.h"
#include "FreeRunningTimer.h"
#include "BOARD.h"
//#define TEST_HARNESS_FRT

int millisecond, microsecond;
unsigned char LED = 0;

void FreeRunningTimer_Init(void){            
    /*Timer Registers*/
    T5CON = 0;
    T5CONbits.SIDL = 0;
    T5CONbits.TGATE = 0;
    T5CONbits.TCKPS = 0b011;    // Pre-scale value = 1:8
    
    PR5 = 5000;   // Every 10,000 ticks is 1 millisecond
    /*Interrupts*/
    IFS0bits.T5IF = 0;      // Clear Timer 5 flag
    IPC5bits.T5IP = 2;   // Set priority 2
    IEC0bits.T5IE = 1;      // Enable T5 interrupt
    
    T5CONbits.ON = 1;   // Begin timer
    /*Clear internal variables*/
    millisecond = 0;
    microsecond = 0;
}

unsigned int FreeRunningTimer_GetMilliSeconds(void){
    return millisecond;
}

unsigned int FreeRunningTimer_GetMicroSeconds(void){
    return TMR5 / 5;
}
/*Interrupt Handler*/
void __ISR ( _TIMER_5_VECTOR , ipl3auto ) Timer5IntHandler ( void ) {
    if (IFS0bits.T5IF == 1){
        millisecond ++;
        microsecond += 1000;
    }
    IFS0bits.T5IF = 0;  // Clear Timer5 flag
}

#ifdef TEST_HARNESS_FRT
void main(void){
    BOARD_Init();
    Protocol_Init();
    FreeRunningTimer_Init();
    char messageMilli[MAXPAYLOADLENGTH];
    while(1){
            sprintf(messageMilli,"Get millisecond function: %d\n", millisecond);
            Protocol_SendDebugMessage(messageMilli);
    }
}
#endif