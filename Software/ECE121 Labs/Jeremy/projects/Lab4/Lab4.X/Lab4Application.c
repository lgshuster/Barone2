#include "Board.h"
#include "Protocol.h"
#include "FreeRunningTimer.h"
#include "MessageIDs.h"
#include "FeedbackControl.h"
#include "DCMotorDrive.h"
#include "RotaryEncoder.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <GenericTypeDefs.h>
#include <xc.h>
#include <sys/attribs.h>

#define MAXSPEED 1000
#define ENA LATDbits.LATD2 //output compare pin 6
#define IN1 LATDbits.LATD1 //pin 5 In1 pin
#define IN2 LATDbits.LATD0 //pin 3 In2 pin
#define ENCODER_ROLLOVER 0x3FFF
#define MAXDEGREES 360
#define GEARRATIO 84
#define ENCODERTICKSINDEGREES 46

int main(void)
{
    BOARD_Init();
    Protocol_Init(); //initializes modules
    LEDS_INIT();

    FreeRunningTimer_Init();
    DCMotorDrive_Init();
    RotaryEncoder_Init(1);
    FeedbackControl_Init();

    char testMessage[MAXPAYLOADLENGTH];
    sprintf(testMessage, "Lab 4 Application Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(testMessage); //sends debug  message


    unsigned char payload[MAXPAYLOADLENGTH]; //char array for sending/receiving payloads

    int kptemp;
    int kitemp;
    int kdtemp;
    unsigned int current;
    unsigned int previous;
    int rate;
    int commandedrate;
    int target1 = 10; //every 10ms, while loop, report speed
    int target2 = 5; //every 5ms, run PID loop
    int u;
    int PWM;
    int error;
    DCMotorDrive_SetMotorSpeed(0); //initializes at 0
    while (1) {
        if (FreeRunningTimer_GetMilliSeconds() >= target1) {
            target1 += 10;
            current = RotaryEncoder_ReadRawAngle(); //gets current angle reading
            rate = current - previous;
            if ((rate > MAXSPEED) && IN2) {
                rate = rate - (ENCODER_ROLLOVER); //fixes rate based off of a rollover
            }
            if ((rate < (MAXSPEED * -1)) && IN1) {
                rate = rate + (ENCODER_ROLLOVER);
            }
            previous = current;
            rate = (rate * 100) / GEARRATIO / ENCODERTICKSINDEGREES; //converts rate to degrees per second of top motor
        }
        if (FreeRunningTimer_GetMilliSeconds() >= target2) {
            target2 += 5;
            u = FeedbackControl_Update(commandedrate, rate); //gets new feedback variable
            if (u > 0){
                DCMotorDrive_SetMotorSpeed((u * 675 / 360) + 325); //limits u from 0-675, motor stalls under +-325 PWM
            } else {
                DCMotorDrive_SetMotorSpeed((u * 675 / 360) - 325); //u will be from -360 - +360, must convert to PWM ^
            }
            PWM = DCMotorControl_GetMotorSpeed(); //gets current PWM
            error = commandedrate - rate; //gets error
            if (commandedrate == 0) { //puts brake if commandedrate is 0
                DCMotorDrive_SetMotorSpeed(0);
                DCMotorDrive_SetBrake();
            }
            payload[0] = (error >> 24) & 0xFF; //returns message
            payload[1] = (error >> 16) & 0xFF;
            payload[2] = (error >> 8) & 0xFF;
            payload[3] = error & 0xFF;
            payload[4] = (rate >> 24) & 0xFF;
            payload[5] = (rate >> 16) & 0xFF;
            payload[6] = (rate >> 8) & 0xFF;
            payload[7] = rate & 0xFF;
            payload[8] = (PWM >> 24) & 0xFF;
            payload[9] = (PWM >> 16) & 0xFF;
            payload[10] = (PWM >> 8) & 0xFF;
            payload[11] = PWM & 0xFF;
            Protocol_SendMessage(12, ID_REPORT_FEEDBACK, & payload);
        }
        if (Protocol_IsMessageAvailable()) { //gets gains from message and sets them, also resets controller static variables
            if (Protocol_ReadNextID() == ID_FEEDBACK_SET_GAINS) {
                Protocol_GetPayload(&payload);
                kptemp = (payload[0] << 24) + (payload[1] << 16) + (payload[2] << 8) + payload[3];
                FeedbackControl_SetProportionalGain(kptemp);
                kitemp = (payload[4] << 24) + (payload[5] << 16) + (payload[6] << 8) + payload[7];
                FeedbackControl_SetIntegralGain(kitemp);
                kdtemp = (payload[8] << 24) + (payload[9] << 16) + (payload[10] << 8) + payload[11];
                FeedbackControl_SetDerivativeGain(kdtemp);
                FeedbackControl_ResetController();
                Protocol_SendDebugMessage("Set Gains & reset controller");
                Protocol_SendMessage(0, ID_FEEDBACK_SET_GAINS_RESP, NULL);
            }
            if (Protocol_ReadNextID() == ID_COMMANDED_RATE) { //gets commanded rate from payload
                Protocol_GetPayload(&payload);
                commandedrate = (payload[0] << 24) + (payload[1] << 16) + (payload[2] << 8) + payload[3];

            }
        }
        if (U1STAbits.OERR == 1) { //clears overflow errors from SPI or UART
            U1STAbits.OERR = 0;
        }
        if (SPI2STATbits.SPIROV == 1) {
            SPI2STATCLR = 1 << 6;
        }
    }
}
