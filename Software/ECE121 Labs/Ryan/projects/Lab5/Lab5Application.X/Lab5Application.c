#include <proc/p32mx340f512h.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <xc.h>
#include "RotaryEncoder.h"
#include "FreeRunningTimer.h"
#include "MessageIDs.h"
#include "Protocol.h"
#include "BOARD.h"
#include "DCMotorDrive.h"
#include "FeedbackControl.h"
#include "NonVolatileMemory.h"
#include "ADCFilter.h"

#define IN1 LATFbits.LATF1
#define IN2 LATDbits.LATD1

#define TEST_HARNDESS_DCMOTORDRIVE

#define MAX_RATE 1000
#define ENCODER_ROLLOVER 0x3FFF
#define PWM_FREQUENCY 2000
#define GEAR_RATIO 84
#define DEGREE_CONVERT 46
#define TIME_DELTA 500
#define COMMAND_RATE_TO_ANGULAR 194 / 1000
#define ANGULAR_TO_COMMAND_RATE 1000 / 194
#define COMMAND_MODE 0
#define GAINS_ADDRESS 0
#define GAINS_LENGTH 12
#define PAGE_LENGTH 64
#define RANGE_LIMIT 194
#define POT_TO_ANGULAR 194/577
#define PIN0 0

union values {

    struct {
        int proportional;
        int integral;
        int derivative;
    };
    unsigned char gainsArray[12];
};

union feedback_update {

    struct {
        int comanded_angle;
        int sensor;
        int feedback_rate;
    };
    unsigned char FeedbackUpdateValues[8];
};

union report_feedback {

    struct {
        int current_error;
        int reference_signal;
        int sensor_signal;
        int commanded_position;
    };
    unsigned char FeedbackUpdateValues[16];
};

static unsigned char message[MAXPAYLOADLENGTH];
unsigned char null;
union values gains;
union values payload;
union feedback_update feedback_update;
union report_feedback report_feedback;
unsigned char messageID;
unsigned char commandMode = COMMAND_MODE;

void main(void) {
    unsigned char commandMode = 0;
    unsigned short currentAngle = 0;
    unsigned short previousAngle;
    unsigned int accumulatedAngle = 0;
    int totalAngle;
    int previousTime = 0;
    int reportSpeed_previousTime, controlLoop_previousTime;
    int payloadPosition;
    int angularVelocity;
    unsigned char messageID;
    short lowPassFilter[FILTERLENGTH] = {-54, -64, -82, -97, -93, -47, 66, 266, 562, 951, 1412, 1909, 2396, 2821, 3136, 3304, 3304, 3136, 2821, 2396, 1909, 1412, 951, 562, 266, 66, -47, -93, -97, -82, -64, -54};
    short potentiometerReading;
    short LPFData[FILTERLENGTH];
    int i = 0;
    int j;
    short averagePOT;
    BOARD_Init();
    Protocol_Init();
    RotaryEncoder_Init(ENCODER_INTERRUPT_MODE);
    DCMotorDrive_Init();
    FeedbackControl_Init();
    NonVolatileMemory_Init();
    ADCFilter_Init();
    FreeRunningTimer_Init();
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while ((FreeRunningTimer_GetMilliSeconds() - previousTime) < 200) {
    } // 200ms delay 
    sprintf(message, "Lab 5 application compiled on: %s %s\n", __DATE__, __TIME__);
    Protocol_SendDebugMessage(message);
    NonVolatileMemory_ReadPage(GAINS_ADDRESS, GAINS_LENGTH, gains.gainsArray); // Load PID gains for COMMAND_MODE from NVM
    payload.proportional = Protocol_IntEndednessConversion(gains.proportional);
    payload.integral = Protocol_IntEndednessConversion(gains.integral);
    payload.derivative = Protocol_IntEndednessConversion(gains.derivative);
    FeedbackControl_SetProportionalGain(gains.proportional);
    FeedbackControl_SetIntegralGain(gains.integral);
    FeedbackControl_SetDerivativeGain(gains.derivative);
    Protocol_SendMessage(12, ID_FEEDBACK_CUR_GAINS, payload.gainsArray); // Send an ID_FEEDBACK_CUR_GAINS message with current gains
    Protocol_SendMessage(1, ID_LAB5_CUR_MODE, &commandMode); // Send an ID_LAB5_CUR_MODE message with current mode
    feedback_update.comanded_angle = RotaryEncoder_ReadRawAngle(); // set commanded angle to current absolute angle
    ADCFilter_SetWeights(0, lowPassFilter);
    previousTime = FreeRunningTimer_GetMilliSeconds();
    reportSpeed_previousTime = FreeRunningTimer_GetMilliSeconds();
    controlLoop_previousTime = FreeRunningTimer_GetMilliSeconds();
    while (1) {
        if (U1STAbits.OERR) // UART 1 Protocol receiver buffer overrun
            U1STAbits.OERR = 0;
        if (Protocol_IsMessageAvailable() == TRUE) { // Check for messages
            messageID = Protocol_ReadNextID();
            /*Set gains*/
            if (messageID == ID_FEEDBACK_SET_GAINS) {
                Protocol_GetPayload(payload.gainsArray);
                gains.proportional = Protocol_IntEndednessConversion(payload.proportional);
                gains.integral = Protocol_IntEndednessConversion(payload.integral);
                gains.derivative = Protocol_IntEndednessConversion(payload.derivative);
                FeedbackControl_SetProportionalGain(gains.proportional);
                FeedbackControl_SetIntegralGain(gains.integral);
                FeedbackControl_SetDerivativeGain(gains.derivative);
                Protocol_SendDebugMessage("Set Gainz");
                FeedbackControl_ResetController(); // Resets controller after every set gains
                NonVolatileMemory_WritePage(GAINS_ADDRESS, GAINS_LENGTH, gains.gainsArray);
                /*Request gains*/
            } else if (messageID == ID_FEEDBACK_REQ_GAINS) {
                Protocol_GetPayload(&null);
                NonVolatileMemory_ReadPage(GAINS_ADDRESS, GAINS_LENGTH, gains.gainsArray);
                payload.proportional = Protocol_IntEndednessConversion(gains.proportional);
                payload.integral = Protocol_IntEndednessConversion(gains.integral);
                payload.derivative = Protocol_IntEndednessConversion(gains.derivative);
                //                Protocol_SendDebugMessage("Get Gainz");
                Protocol_SendMessage(12, ID_FEEDBACK_CUR_GAINS, payload.gainsArray);
                /*Set mode*/
            } else if (messageID == ID_LAB5_SET_MODE) {
                Protocol_GetPayload(&commandMode);
                payload.proportional = Protocol_IntEndednessConversion(gains.proportional);
                payload.integral = Protocol_IntEndednessConversion(gains.integral);
                payload.derivative = Protocol_IntEndednessConversion(gains.derivative);
                Protocol_SendMessage(12, ID_FEEDBACK_CUR_GAINS, payload.gainsArray);
                /*Request mode*/
            } else if (messageID == ID_LAB5_REQ_MODE) {
                Protocol_GetPayload(&null);
                Protocol_SendMessage(1, ID_LAB5_CUR_MODE, &commandMode);
                /*Set commanded position*/
            } else if (ID_COMMANDED_POSITION) {
                Protocol_GetPayload(&feedback_update.comanded_angle);
                feedback_update.comanded_angle = Protocol_IntEndednessConversion(feedback_update.comanded_angle);
                FeedbackControl_ResetController();
            }
        }
        /*1000Hz Calculate motor angle and speed*/
        if ((FreeRunningTimer_GetMilliSeconds() - previousTime) > 1) { // 1000Hz calculate motor speed
            currentAngle = RotaryEncoder_ReadRawAngle();
            angularVelocity = currentAngle - previousAngle;
            previousAngle = currentAngle; // Save angle as previous
            if ((angularVelocity > MAX_RATE) && IN1) { // Negative velocity
                angularVelocity -= ENCODER_ROLLOVER;
                accumulatedAngle -= ENCODER_ROLLOVER;
            }
            if ((angularVelocity < -MAX_RATE) && IN2) { // Positive velocity
                angularVelocity += ENCODER_ROLLOVER;
                accumulatedAngle += ENCODER_ROLLOVER;
            }
            totalAngle = (currentAngle + accumulatedAngle) / (GEAR_RATIO * DEGREE_CONVERT);
            angularVelocity *= TIME_DELTA; // Converts angular velocity to degrees per second
            angularVelocity /= GEAR_RATIO;
            angularVelocity /= DEGREE_CONVERT;
            sprintf(message, "Total Angle: %d Current angle: %d Previous angle: %d Accumulated angle: %d", totalAngle, currentAngle, previousAngle, accumulatedAngle);
//            Protocol_SendDebugMessage(message);
            previousTime = FreeRunningTimer_GetMilliSeconds();
        }
        if ((FreeRunningTimer_GetMilliSeconds() - controlLoop_previousTime) > 5) { //200Hz ID_REPORT_FEEDBACK
            /*ADC Filter average calculation*/
            potentiometerReading = ADCFilter_FilteredReading(PIN0);
            LPFData[i] = potentiometerReading;
            i = (i + 1) % FILTERLENGTH;
            for (j = 0; j < FILTERLENGTH; j++) {
                averagePOT += LPFData[j];
            }
            averagePOT /= FILTERLENGTH;
            sprintf(message, "Average pot: %d Total Angle: %d", averagePOT * POT_TO_ANGULAR, totalAngle);
//            Protocol_SendDebugMessage(message);
            /*PID Feedback loop*/
            if ((totalAngle > -3000) && (totalAngle < 3000)  && (averagePOT < 577) && (averagePOT > 0)) { // Reject erroneous angular velocities
                feedback_update.feedback_rate = FeedbackControl_Update(averagePOT * POT_TO_ANGULAR, totalAngle) / 820; // Run PID loop with scaling for PWM
                //                if((totalAngle > RANGE_LIMIT) && (feedback_update.feedback_rate > 0))
                //                    DCMotorDrive_SetBrake();
                //                else if ((totalAngle > -RANGE_LIMIT) && (feedback_update.feedback_rate < 0))
                //                    DCMotorDrive_SetBrake();
                //                else
                //                    DCMotorDrive_SetMotorSpeed(feedback_update.feedback_rate);
                DCMotorDrive_SetMotorSpeed(feedback_update.feedback_rate);
                if (averagePOT == 0) { // Apply break when PWM set to 0
                    DCMotorDrive_SetMotorSpeed(0);
                    DCMotorDrive_SetBrake();
                }
                /*Int endian conversion*/
                report_feedback.commanded_position = Protocol_IntEndednessConversion((int) (averagePOT * POT_TO_ANGULAR));
                report_feedback.sensor_signal = Protocol_IntEndednessConversion(totalAngle);
                report_feedback.current_error = Protocol_IntEndednessConversion((averagePOT * POT_TO_ANGULAR) - totalAngle);
                report_feedback.reference_signal = Protocol_IntEndednessConversion((int) (averagePOT * POT_TO_ANGULAR));
                /*Send REPORT FEEDBACK*/
                Protocol_SendMessage(16, ID_LAB5_REPORT, report_feedback.FeedbackUpdateValues);
            } // Reject erroneous angular velocity
            controlLoop_previousTime = FreeRunningTimer_GetMilliSeconds();
        } // 200Hz Report Feedback
    } // while (1)
    BOARD_End();
} // main()