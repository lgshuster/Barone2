#include <proc/p32mx340f512h.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <xc.h>
#include "RotaryEncoder.h"
#include "FreeRunningTimer.h"
#include "PingSensor.h"
#include "MessageIDs.h"
#include "RCServo.h"
#include "Protocol.h"
#include "BOARD.h"
#include "NOP.h"

void main (void){
    BOARD_Init();
    Protocol_Init();
    FreeRunningTimer_Init();
    RotaryEncoder_Init(ENCODER_BLOCKING_MODE);
    PingSensor_Init();
    RCServo_Init();
    unsigned char message[MAXPAYLOADLENGTH];
    sprintf(message,"Lab 2 Application compiled on: %s %s\n", __DATE__,__TIME__);
    Protocol_SendDebugMessage(message);
    unsigned short rotaryData;
    int degrees_rotaryData;
    unsigned short pingData;
    int degrees_pingData;
    int mode;   // 0: Ping, 1: Encoder
    int Encoder_previousTime, Encoder_currentTime, Ping_previousTime, Ping_currentTime, Application_previousTime, Application_currentTime;
    Application_previousTime = FreeRunningTimer_GetMilliSeconds();
    Encoder_previousTime = FreeRunningTimer_GetMilliSeconds();
    Ping_previousTime = FreeRunningTimer_GetMilliSeconds();
    while(1){
        Application_currentTime = FreeRunningTimer_GetMilliSeconds();
        if((Application_currentTime - Application_previousTime) > 50){ // 20Hz Application
            if (mode){   // Rotary Data
                if (rotaryData < 600){  // OOB low
                    unsigned char angleReport[MAXPAYLOADLENGTH] = { (degrees_rotaryData & 0xFF000000) >> 24, (degrees_rotaryData & 0x00FF0000) >> 16,(degrees_rotaryData & 0x0000FF00) >> 8,degrees_rotaryData & 0x000000FF, 4};  // Short Endian Conversion
                    Protocol_SendMessage(5, ID_LAB2_ANGLE_REPORT,&angleReport);
                    Application_previousTime = FreeRunningTimer_GetMilliSeconds();
                }
                else if (rotaryData > 2400){    // OOB high
                    unsigned char angleReport[MAXPAYLOADLENGTH] = { (degrees_rotaryData & 0xFF000000) >> 24, (degrees_rotaryData & 0x00FF0000) >> 16,(degrees_rotaryData & 0x0000FF00) >> 8,degrees_rotaryData & 0x000000FF, 1};  // Short Endian Conversion
                    Protocol_SendMessage(5, ID_LAB2_ANGLE_REPORT,&angleReport);
                    Application_previousTime = FreeRunningTimer_GetMilliSeconds();
                }
                else{   // In range
                    RCServo_SetPulse(rotaryData);
                    unsigned char angleReport[MAXPAYLOADLENGTH] = { (degrees_rotaryData & 0xFF000000) >> 24, (degrees_rotaryData & 0x00FF0000) >> 16,(degrees_rotaryData & 0x0000FF00) >> 8,degrees_rotaryData & 0x000000FF, 2};  // Short Endian Conversion
                    Protocol_SendMessage(5, ID_LAB2_ANGLE_REPORT,&angleReport);
                    Application_previousTime = FreeRunningTimer_GetMilliSeconds();
                }
            }
            else{    // Ping Data
                if (pingData < 600){    // OOB low
                    unsigned char angleReport[MAXPAYLOADLENGTH] = { (degrees_rotaryData & 0xFF000000) >> 24, (degrees_pingData & 0x00FF0000) >> 16,(degrees_pingData & 0x0000FF00) >> 8,degrees_pingData & 0x000000FF, 4};  // Short Endian Conversion
                    Protocol_SendMessage(5, ID_LAB2_ANGLE_REPORT,&angleReport);
                    Application_previousTime = FreeRunningTimer_GetMilliSeconds();
                }
                else if (pingData > 2400){  // OOB high
                    unsigned char angleReport[MAXPAYLOADLENGTH] = { (degrees_rotaryData & 0xFF000000) >> 24, (degrees_pingData & 0x00FF0000) >> 16,(degrees_pingData & 0x0000FF00) >> 8,degrees_pingData & 0x000000FF, 1};  // Short Endian Conversion
                    Protocol_SendMessage(5, ID_LAB2_ANGLE_REPORT,&angleReport);
                    Application_previousTime = FreeRunningTimer_GetMilliSeconds();
                }
                else{   // In range
                    RCServo_SetPulse(pingData);
                    unsigned char angleReport[MAXPAYLOADLENGTH] = { (degrees_rotaryData & 0xFF000000) >> 24, (degrees_pingData & 0x00FF0000) >> 16,(degrees_pingData & 0x0000FF00) >> 8,degrees_pingData & 0x000000FF, 2};  // Short Endian Conversion
                    Protocol_SendMessage(5, ID_LAB2_ANGLE_REPORT,&angleReport);
                    Application_previousTime = FreeRunningTimer_GetMilliSeconds();
                }
            }
        }
        Encoder_currentTime = FreeRunningTimer_GetMilliSeconds();
        if ((Encoder_currentTime - Encoder_previousTime) > 10){    // 100Hz Encoder
            rotaryData = RotaryEncoder_ReadRawAngle();
            rotaryData = rotaryData & 0x3FFF;
            rotaryData = rotaryData / 8 + 550;
            degrees_rotaryData = rotaryData * 1800 / 13;
            Encoder_previousTime = FreeRunningTimer_GetMilliSeconds();
        }
        Ping_currentTime = FreeRunningTimer_GetMilliSeconds();
        if ((Ping_currentTime - Ping_previousTime) > 100){   // 100Hz ping
            pingData = PingSensor_GetDistance() * 22;
            degrees_pingData = pingData * 1800 / 13;
            Ping_previousTime = FreeRunningTimer_GetMilliSeconds();
        }
        if (Protocol_IsMessageAvailable() == TRUE){
            unsigned char payload[MAXPAYLOADLENGTH];
            Protocol_GetPayload(payload);
            mode = *payload;
        }
    }
    BOARD_End();
}