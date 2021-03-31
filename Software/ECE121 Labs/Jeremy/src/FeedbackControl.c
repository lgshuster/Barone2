#include "Board.h"
#include "Protocol.h"
#include "MessageIDs.h"
#include "FeedbackControl.h"
#include "DCMotorDrive.h"
#include "FreeRunningTimer.h"
#include "RotaryEncoder.h"
/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <GenericTypeDefs.h>
#include <xc.h>
#include <sys/attribs.h>
static int kp;
static int ki;
static int kd;
static int A;
static int y;
#define IN1 LATDbits.LATD1 //pin 5 In1 pin
#define IN2 LATDbits.LATD0 //pin 3 In2 pin
#define MAXDEGREES 360
/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/
//#define FEEDBACK_MAXOUTPUT_POWER 27

/**
 * @Function FeedbackControl_Init(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief initializes the controller to the default values and (P,I,D)->(1, 0, 0)*/
int FeedbackControl_Init(void)
{
    kp = 1; //sets gains to 0
    ki = 0;
    kd = 0;
    return (SUCCESS);
}

/**
 * @Function FeedbackControl_SetProportionalGain(int newGain);
 * @param newGain, integer proportional gain
 * @return SUCCESS or ERROR
 * @brief sets the new P gain for controller */
int FeedbackControl_SetProportionalGain(int newGain)
{
    kp = newGain; //sets static variables to input
    return (SUCCESS);
}

/**
 * @Function FeedbackControl_SetIntegralGain(int newGain);
 * @param newGain, integer integral gain
 * @return SUCCESS or ERROR
 * @brief sets the new I gain for controller */
int FeedbackControl_SetIntegralGain(int newGain)
{
    ki = newGain;
    return (SUCCESS);
}

/**
 * @Function FeedbackControl_SetDerivativeGain(int newGain);
 * @param newGain, integer derivative gain
 * @return SUCCESS or ERROR
 * @brief sets the new D gain for controller */
int FeedbackControl_SetDerivativeGain(int newGain)
{
    kd = newGain;
    return (SUCCESS);
}

/**
 * @Function FeedbackControl_GetPorportionalGain(void)
 * @param None
 * @return Proportional Gain
 * @brief retrieves requested gain */
int FeedbackControl_GetProportionalGain(void)
{
    return (kp); //returns static variables
}

/**
 * @Function FeedbackControl_GetIntegralGain(void)
 * @param None
 * @return Integral Gain
 * @brief retrieves requested gain */
int FeedbackControl_GetIntegralGain(void)
{
    return (ki);
}

/**
 * @Function FeedbackControl_GetDerivativeGain(void)
 * @param None
 * @return Derivative Gain
 * @brief retrieves requested gain */
int FeedbackControl_GetDerivativeGain(void)
{
    return (kd);
}

/**
 * @Function FeedbackControl_Update(int referenceValue, int sensorValue)
 * @param referenceValue, wanted reference
 * @param sensorValue, current sensor value
 * @brief performs feedback step according to algorithm in lab manual */
int FeedbackControl_Update(int referenceValue, int sensorValue)
{
    int error = referenceValue - sensorValue; //gets error value
    A += error; //integrates current error
    int D = (sensorValue - y) * -1; //gets derivative of static and current sensor value
    y = sensorValue;
    int u = kp * error + ki * A + kd * D; //gets total u
    if (u > MAXDEGREES) {
        u = u - (u - MAXDEGREES); //limits u to +-360 degrees per second
        A -= error;
    }
    if (u < (MAXDEGREES * -1)) {
        u = u - (u + MAXDEGREES);
        A -= error;
    }
    return (u);
}

/**
 * @Function FeedbackControl_ResetController(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief resets integrator and last sensor value to zero */
int FeedbackControl_ResetController(void)
{
    A = 0; //resets static variables
    y = 0;
    return (SUCCESS);
}

//#define FEEDBACK_TEST
#ifdef FEEDBACK_TEST

int main(void)
{
    BOARD_Init();
    Protocol_Init();
    LEDS_INIT();

    char testMessage[MAXPAYLOADLENGTH];
    sprintf(testMessage, "Feedback Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(testMessage);

    FreeRunningTimer_Init();
    DCMotorDrive_Init();
    RotaryEncoder_Init(1);
    FeedbackControl_Init();

    int r;
    int kptemp;
    int kitemp;
    int kdtemp;
    int ytemp;
    int u;
    unsigned char payload[MAXPAYLOADLENGTH];
    while (1) {
        if (Protocol_IsMessageAvailable()) {
            if (Protocol_ReadNextID() == ID_FEEDBACK_SET_GAINS) { //gets gains and puts them into static variables
                Protocol_GetPayload(&payload);
                kptemp = (payload[0] << 24) + (payload[1] << 16) + (payload[2] << 8) + payload[3];
                FeedbackControl_SetProportionalGain(kptemp);
                kitemp = (payload[4] << 24) + (payload[5] << 16) + (payload[6] << 8) + payload[7];
                FeedbackControl_SetIntegralGain(kitemp);
                kdtemp = (payload[8] << 24) + (payload[9] << 16) + (payload[10] << 8) + payload[11];
                FeedbackControl_SetDerivativeGain(kdtemp);
                Protocol_SendDebugMessage("Set Gains");
                Protocol_SendMessage(0, ID_FEEDBACK_SET_GAINS_RESP, NULL);
            }
            if (Protocol_ReadNextID() == ID_FEEDBACK_REQ_GAINS) {// gets each gain, returns in 3 int message
                kptemp = FeedbackControl_GetProportionalGain();
                payload[0] = (kptemp >> 24) & 0xFF;
                payload[1] = (kptemp >> 16) & 0xFF;
                payload[2] = (kptemp >> 8) & 0xFF;
                payload[3] = kptemp & 0xFF;

                kitemp = FeedbackControl_GetIntegralGain();
                payload[4] = (kitemp >> 24) & 0xFF;
                payload[5] = (kitemp >> 16) & 0xFF;
                payload[6] = (kitemp >> 8) & 0xFF;
                payload[7] = kitemp & 0xFF;

                kdtemp = FeedbackControl_GetDerivativeGain();
                payload[8] = (kdtemp >> 24) & 0xFF;
                payload[9] = (kdtemp >> 16) & 0xFF;
                payload[10] = (kdtemp >> 8) & 0xFF;
                payload[11] = kdtemp & 0xFF;
                Protocol_SendMessage(12, ID_FEEDBACK_CUR_GAINS, &payload);
            }
            if (Protocol_ReadNextID() == ID_FEEDBACK_RESET_CONTROLLER) { //resets controller
                Protocol_GetPayload(&payload);
                FeedbackControl_ResetController();
                Protocol_SendDebugMessage("Reset Controller");
                Protocol_SendMessage(0, ID_FEEDBACK_RESET_CONTROLLER_RESP, NULL);
            }
            if (Protocol_ReadNextID() == ID_FEEDBACK_UPDATE) { //updates feedback from r and ytemp given
                Protocol_GetPayload(&payload);
                r = (payload[0] << 24) + (payload[1] << 16) + (payload[2] << 8) + payload[3];
                ytemp = (payload[4] << 24) + (payload[5] << 16) + (payload[6] << 8) + payload[7];
                u = FeedbackControl_Update(r, ytemp);
                u = Protocol_IntEndednessConversion(u);
                Protocol_SendMessage(4, ID_FEEDBACK_UPDATE_OUTPUT, &u);
            }
        }
        if (U1STAbits.OERR == 1) { //clears overflows
            U1STAbits.OERR = 0;
        }
        if (SPI2STATbits.SPIROV == 1) {
            SPI2STATCLR = 1 << 6;
        }
    }
}
#endif
