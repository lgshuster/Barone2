#include <proc/p32mx340f512h.h>
#include <sys/attribs.h>
#include <string.h>
#include <stdio.h>
#include "BOARD.h"
#include "Protocol.h"
#include "PingSensor.h"
#include "MessageIDs.h"
#include "NOP.h"
#define PIN8 TRISDbits.TRISD10
#define PIN4 LATFbits.LATF1
//#define TEST_HARNESS_PING_SENSOR

static unsigned short distance, timeDelta, upGoing, downGoing; // Variables to calculate distance
int fallEdge, newTime, doNotInterrupt;
unsigned int OC3Timer;
char message[MAXPAYLOADLENGTH];

unsigned char array1, array2;
unsigned char array[MAXPAYLOADLENGTH];

int PingSensor_Init(void){
    // Set pin 4 as output/trigger
    PIN8 = 1;   // Set IC3 as input
    TRISFbits.TRISF1 = 0;   // Set pin 4 as output
    /*T4CON registers*/
    T4CON = 0;
    T4CONbits.TCKPS = 0b110;    // Pre-scale value = 1:64
    PR4 = 37500;   // interrupts every 60 millisecond
    /*Timer 4 Interrupts*/
    IFS0bits.T4IF = 0;      // Clear Timer 4 flag
    IPC4bits.T4IP = 5;      // Set priority 5
    IEC0bits.T4IE = 1;      // Enable T4 interrupt
    
    /*IC3*/
    IC3CON = 0; // Clear IC3 register
    IC3BUF = 0; // Clear IC3 buffer
    IC3CONbits.ICM = 0b110; // Simple Capture Event mode - every edge
    IC3CONbits.FEDGE = 1;   // Trigger on rising edge
    IC3CONbits.ICTMR = 1;   // Timer2 is the counter source for capture
    /*IC3 Interrupt*/
    IFS0bits.IC3IF = 0;     // Clear IC3 flag
    IPC3bits.IC3IP = 4;     // Set priority 4
    IEC0bits.IC3IE = 1;     // Enable IC3 interrupt
    /*Clear register and buffer for IC3*/
    
    /*Timer 2*/
    T2CON = 0;
    T2CONbits.TCKPS = 0b011;    // Pre-scale value = 1:8
    PR2 = 60000;    // 12ms rollover (5000ticks * 12ms)
    
    /*Begin timers*/
    T4CONbits.ON = 1;
    T2CONbits.ON = 1;
    IC3CONbits.ON = 1;
}
/*Interrupt Handler*/
void __ISR ( _TIMER_4_VECTOR ) Timer4IntHandler ( void ){   // Interrupts every 60ms
    if (IFS0bits.T4IF && !doNotInterrupt){
        PIN4 = 1;
        unsigned int previousMicro = FreeRunningTimer_GetMicroSeconds();
        while ((FreeRunningTimer_GetMicroSeconds() - previousMicro) < 200){}
        PIN4 = 0;
    }
    IFS0bits.T4IF = 0;
}
void __ISR ( _INPUT_CAPTURE_3_VECTOR ) __IC3Interrupt ( void ) {    //IC3 pin rising and falling edge
    if (IFS0bits.IC3IF && !doNotInterrupt){
        if(fallEdge == 0){  // Rising edge
            OC3Timer = FreeRunningTimer_GetMilliSeconds();
            upGoing = (IC3BUF & 0xFFFF);
            fallEdge = 1;
        }
        else {  // Falling edge
            downGoing = (IC3BUF & 0xFFFF);
            fallEdge = 0;
            newTime = 1;
        }
    }
    while (IC3CONbits.ICOV){    // Check for buffer overflow
        unsigned short dump = IC3BUF;
    }
    IFS0bits.IC3IF = 0;
}

unsigned short PingSensor_GetDistance(void){
    doNotInterrupt = 1;
    if (newTime){
        timeDelta = (downGoing - upGoing) / 5;  // pre-scale accounted by /5, in micro-second
        distance = timeDelta / 58; // Distance in cm
        newTime = 0;
        doNotInterrupt = 0;
        return distance;
    }     
}

#ifdef TEST_HARNESS_PING_SENSOR
void main (void){
    BOARD_Init();
    Protocol_Init();
    PingSensor_Init();
    FreeRunningTimer_Init();
    sprintf(message,"Ping Sensor compiled on: %s %s\n", __DATE__,__TIME__);
    Protocol_SendDebugMessage(message);
    unsigned int previousTime, currentTime;
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (1){
        currentTime = FreeRunningTimer_GetMilliSeconds();
        if ((currentTime - previousTime) > 100){    // 10Hz ping
            unsigned short getDistance = PingSensor_GetDistance();
            getDistance = Protocol_ShortEndednessConversion(getDistance);
            Protocol_SendMessage(2,ID_PING_DISTANCE,&getDistance);
            previousTime = FreeRunningTimer_GetMilliSeconds();
        }
    }
    BOARD_End();
}
#endif

#ifdef TEST_HARNESS_PING_PONG
int main(void)
{
    BOARD_Init();
    Protocol_Init();
    char testMessage[MAXPAYLOADLENGTH];
    sprintf(testMessage, "Protocol Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(testMessage);
    
    short shortTestValue = 0xDEAD;
    short shortResultValue;
    int intTestValue = 0xDEADBEEF;
    int intResultValue;
    
    shortResultValue = Protocol_ShortEndednessConversion(shortTestValue);
    sprintf(testMessage, "Short Endedness Conversion: IN: 0x%X OUT: 0x%X", shortTestValue&0xFFFF, shortResultValue&0xFFFF);
    Protocol_SendDebugMessage(testMessage);
    
    
    intResultValue = Protocol_IntEndednessConversion(intTestValue);
    sprintf(testMessage, "Int Endedness Conversion: IN: 0x%X OUT: 0x%X", intTestValue, intResultValue);
    Protocol_SendDebugMessage(testMessage);
    
    
    unsigned int pingValue = 0xfff;
    while (1) {
        if (Protocol_IsMessageAvailable()) {
            if (Protocol_ReadNextID() == ID_PING) {
                // send pong in response here
                Protocol_GetPayload(&pingValue);
                pingValue = Protocol_IntEndednessConversion(pingValue);
                pingValue>>=1;
                pingValue = Protocol_IntEndednessConversion(pingValue);
                Protocol_SendMessage(4, ID_PONG, &pingValue);
            }
        }
    }
    while (1);
}
#endif