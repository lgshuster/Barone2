#include "Protocol.h"
#include "BOARD.h"
#include "MessageIDs.h"
#include "FreeRunningTimer.h"
#include "PingSensor.h"
#include "RCServo.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <GenericTypeDefs.h>
#include <xc.h>
#include <sys/attribs.h>
static unsigned char change;

#define LAB2_TEST
#ifdef LAB2_TEST
int main(void)
{
    BOARD_Init();
    Protocol_Init(); //initializes board and UART1 protocol
    LEDS_INIT();

    char testMessage[MAXPAYLOADLENGTH];
    sprintf(testMessage, "Lab 2 Application Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(testMessage); //sends debug send message

    PingSensor_Init(); //initializes ping sensor
    RCServo_SetPulse(1500); //initializes the servo position
    RCServo_Init(); 
    RotaryEncoder_Init(0);//initializes encoder

    unsigned int target = 50; //every .1s, loop
    FreeRunningTimer_Init();
    while (1) { //main loop
        if (FreeRunningTimer_GetMilliSeconds() >= target) { //20hz loops
            target += 50;
            if (change == 0) {
                RCServo_SetPulse(PingSensor_GetDistance()*2 + 600); //sets servo to adjusted ping sensor reading range of about 100cm
            } else {
                RCServo_SetPulse(RotaryEncoder_ReadRawAngle()/9 + 600); //sets servo to encoder reading, adjusted for full rotation of servo
            }
        }
        if (Protocol_IsMessageAvailable()) {
            if (Protocol_ReadNextID() == ID_LAB2_INPUT_SELECT) {
                unsigned char payload[MAXPAYLOADLENGTH]; //if Lab2 input select message received
                void *ptr = &payload;
                Protocol_GetPayload(ptr); //changes the static variable to switch between SPI and ping sensor
                change = payload[0];
            }
        }
        unsigned short reading = RotaryEncoder_ReadRawAngle(); //gets reading of encoder
        unsigned char payload[MAXPAYLOADLENGTH];
        void *ptr = &payload; //initializes payload char array

        reading = reading /45; //turns degrees into 360
        unsigned int milli = reading;
        milli = milli * 1000; //gets millidegrees of reading into an int
        payload[0] = (milli >> 24); // big endian conversions to return correct value
        payload[1] = ((milli << 8) >> 24);
        payload[2] = ((milli << 16) >> 24);
        payload[3] = ((milli << 24) >> 24);
        payload[4] = 2;
        Protocol_SendMessage(5, ID_LAB2_ANGLE_REPORT, ptr); //returns lab 2 angle report message
        
    }
}
#endif