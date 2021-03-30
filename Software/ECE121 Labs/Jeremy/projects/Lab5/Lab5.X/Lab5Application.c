#include "Board.h"
#include "Protocol.h"
#include "FreeRunningTimer.h"
#include "MessageIDs.h"
#include "RotaryEncoder.h"
#include "DCMotorDrive.h"
#include "ADCFilter.h"
#include "NonVolatileMemory.h"
#include "FeedbackControl.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <GenericTypeDefs.h>
#include <xc.h>
#include <sys/attribs.h>

#define GEARRATIO 84
#define ENCODERTICKSINDEGREES 46 //used to get current position
#define ENCODER_ROLLOVER 0x3FFF
#define MAXSPEED 1000
#define IN1 LATDbits.LATD1 //pin 5 In1 pin
#define IN2 LATDbits.LATD0 //pin 3 In2 pin
#define GAINSPAGE 300 //address for gains page

static int accumulated_angle;

int main(void)
{
    BOARD_Init();
    Protocol_Init(); //initializes modules
    LEDS_INIT();
    RotaryEncoder_Init(1);
    DCMotorDrive_Init();
    NonVolatileMemory_Init();
    FeedbackControl_Init();
    ADCFilter_Init();

    char testMessage[MAXPAYLOADLENGTH];
    sprintf(testMessage, "Lab 5 Application Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(testMessage); //sends debug  message

    unsigned char payload[MAXPAYLOADLENGTH]; //char array for sending/receiving payloads
    short LPF[FILTERLENGTH] = {-54, -64, -82, -97, -93, -47, 66, 266, 562, 951, 1412, 1909, 2396, 2821, 3136, 3304, 3304, 3136, 2821, 2396, 1909, 1412, 951, 562, 266, 66, -47, -93, -97, -82, -64, -54};
    //Low Pass Filter for use with Potentiometer
    ADCFilter_SetWeights(0, LPF); //sets hard coded low pass filter
    unsigned short pot; //0 channel used for potentiometer reading

    char gainstemp[64]; //gets previous gains from NVM
    int gain;
    NonVolatileMemory_ReadPage(GAINSPAGE, 64, gainstemp); //reads page with gains on it, first 12 chars used for 3 ints
    gain = (gainstemp[0] << 24) + (gainstemp[1] << 16) + (gainstemp[2] << 8) + gainstemp[3]; //puts appropriate gains into functions
    FeedbackControl_SetProportionalGain(gain);
    gain = (gainstemp[4] << 24) + (gainstemp[5] << 16) + (gainstemp[6] << 8) + gainstemp[7];
    FeedbackControl_SetIntegralGain(gain);
    gain = (gainstemp[8] << 24) + (gainstemp[9] << 16) + (gainstemp[10] << 8) + gainstemp[11];
    FeedbackControl_SetDerivativeGain(gain);

    Protocol_SendMessage(12, ID_FEEDBACK_CUR_GAINS, &gainstemp); //sends initial mode and gains message
    gainstemp[0] = 0;
    Protocol_SendMessage(1, ID_LAB5_CUR_MODE, &gainstemp);

    unsigned int current; //variables used for absolute angle calculation
    unsigned int previous;
    int rate;
    int total;
    int totaltemp;
    int target1 = 10;
    int target2 = 20;
    int commandedposition;
    int u;
    int error;

    FreeRunningTimer_Init(); //initializes timer and motor speed
    DCMotorDrive_SetMotorSpeed(0);
    while (1) {
        if (FreeRunningTimer_GetMilliSeconds() >= target1) {
            target1 += 10;
            current = RotaryEncoder_ReadRawAngle();
            rate = current - previous; //gets current rate and angle
            previous = current;
            if ((rate > MAXSPEED) && IN2) {
                rate = rate - (ENCODER_ROLLOVER); //fixes rate based off of a rollover
                accumulated_angle -= ENCODER_ROLLOVER; //also gets accumulated angle of motor
            }
            if ((rate < (MAXSPEED * -1)) && IN1) {
                rate = rate + (ENCODER_ROLLOVER);
                accumulated_angle += ENCODER_ROLLOVER;
            }
            total = current + accumulated_angle;
            total = total / GEARRATIO / ENCODERTICKSINDEGREES; //final variable for encoder abs angle, in 360 degrees
            totaltemp = Protocol_IntEndednessConversion(total);
            pot = ADCFilter_FilteredReading(0); //gets potentiometer reading
            commandedposition = ((pot - 512) * 5) / 17; //converts pot reading from -150 to 150 degrees commanded position
            error = commandedposition - total;
            u = FeedbackControl_Update(commandedposition, total); //gets feedback control output to motor speed
            if (u > 0) {
                DCMotorDrive_SetMotorSpeed((u * 675 / 360) + 325); //limits u from 0-675, motor stalls under +-325 PWM
            } else {
                DCMotorDrive_SetMotorSpeed((u * 675 / 360) - 325); //u will be from -360 - +360, must convert to PWM ^
            }
            if (commandedposition == total) { //puts brake if in position
                DCMotorDrive_SetMotorSpeed(0);
                DCMotorDrive_SetBrake();
            }
            payload[0] = (error >> 24) & 0xFF; //returns message
            payload[1] = (error >> 16) & 0xFF;
            payload[2] = (error >> 8) & 0xFF;
            payload[3] = error & 0xFF;
            payload[4] = (commandedposition >> 24) & 0xFF; //returns message
            payload[5] = (commandedposition >> 16) & 0xFF;
            payload[6] = (commandedposition >> 8) & 0xFF;
            payload[7] = commandedposition & 0xFF;
            payload[8] = (total >> 24) & 0xFF; //returns message
            payload[9] = (total >> 16) & 0xFF;
            payload[10] = (total >> 8) & 0xFF;
            payload[11] = total & 0xFF;
            payload[12] = (commandedposition >> 24) & 0xFF; //returns message for Lab 5 report
            payload[13] = (commandedposition >> 16) & 0xFF;
            payload[14] = (commandedposition >> 8) & 0xFF;
            payload[15] = commandedposition & 0xFF;
            if (FreeRunningTimer_GetMilliSeconds() >= target2) {
                target2 += 20;
                Protocol_SendMessage(16, ID_LAB5_REPORT, &payload);
            }
        }
        if (Protocol_IsMessageAvailable()) { //gets gains from message and sets them, also resets controller static variables
            if (Protocol_ReadNextID() == ID_FEEDBACK_SET_GAINS) {
                Protocol_GetPayload(&payload);
                NonVolatileMemory_WritePage(GAINSPAGE, 64, payload); //writes NVM page with new gains
                gain = (payload[0] << 24) + (payload[1] << 16) + (payload[2] << 8) + payload[3]; //sets new gains from endedness payload
                FeedbackControl_SetProportionalGain(gain);
                gain = (payload[4] << 24) + (payload[5] << 16) + (payload[6] << 8) + payload[7];
                FeedbackControl_SetIntegralGain(gain);
                gain = (payload[8] << 24) + (payload[9] << 16) + (payload[10] << 8) + payload[11];
                FeedbackControl_SetDerivativeGain(gain);
                FeedbackControl_ResetController();
                Protocol_SendDebugMessage("Set Gains & reset controller"); //resets controller 
                Protocol_SendMessage(0, ID_FEEDBACK_SET_GAINS_RESP, NULL);
            }
            if (Protocol_ReadNextID() == ID_FEEDBACK_REQ_GAINS) {
                Protocol_GetPayload(&payload);
                gain = FeedbackControl_GetProportionalGain();
                payload[0] = (gain >> 24) & 0xFF; //returns message with gains from each of the 3 parts
                payload[1] = (gain >> 16) & 0xFF;
                payload[2] = (gain >> 8) & 0xFF;
                payload[3] = gain & 0xFF;
                gain = FeedbackControl_GetIntegralGain();
                payload[4] = (gain >> 24) & 0xFF;
                payload[5] = (gain >> 16) & 0xFF;
                payload[6] = (gain >> 8) & 0xFF;
                payload[7] = gain & 0xFF;
                gain = FeedbackControl_GetDerivativeGain();
                payload[8] = (gain >> 24) & 0xFF;
                payload[9] = (gain >> 16) & 0xFF;
                payload[10] = (gain >> 8) & 0xFF;
                payload[11] = gain & 0xFF;
                Protocol_SendMessage(12, ID_FEEDBACK_CUR_GAINS, &payload);
            }
            if (Protocol_ReadNextID() == ID_COMMANDED_POSITION) { //not utilized
                Protocol_GetPayload(&payload);
            }
            if (Protocol_ReadNextID() == ID_LAB5_REQ_MODE) { //returns 0 for always command mode
                Protocol_GetPayload(&payload);
                payload[0] = 0;
                Protocol_SendMessage(1, ID_LAB5_CUR_MODE, &payload);
            }
            if (Protocol_ReadNextID() == ID_LAB5_SET_MODE) { //not utilized
                Protocol_GetPayload(&payload);
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
