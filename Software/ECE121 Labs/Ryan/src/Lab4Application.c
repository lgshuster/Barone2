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

#define IN1 LATFbits.LATF1
#define IN2 LATDbits.LATD1

//#define TEST_HARNDESS_DCMOTORDRIVE

#define MAX_RATE 1000
#define ENCODER_ROLLOVER 0x3FFF
#define PWM_FREQUENCY 2000
#define GEAR_RATIO 84
#define DEGREE_CONVERT 46
#define TIME_DELTA 500
#define COMMAND_RATE_TO_ANGULAR 374 / 1000
#define ANGULAR_TO_COMMAND_RATE 1000 / 374

union values {

    struct {
        int proportional;
        int integral;
        int derivative;
    };
    unsigned char FeedbackGains[12];
};

union feedback_update {

    struct {
        int reference;
        int sensor;
        int feedback_rate;
    };
    unsigned char FeedbackUpdateValues[8];
};

union report_feedback {

    struct {
        int error;
        int current_rate;
        int PWM;
    };
    unsigned char FeedbackUpdateValues[8];
};

static unsigned char message[MAXPAYLOADLENGTH];
union values payload;
union feedback_update feedback_update;
union report_feedback report_feedback;

void main(void) {
    BOARD_Init();
    Protocol_Init();
    RotaryEncoder_Init(ENCODER_INTERRUPT_MODE);
    DCMotorDrive_Init();
    FeedbackControl_Init();
    FreeRunningTimer_Init();
    sprintf(message, "Lab 4 application compiled on: %s %s\n", __DATE__, __TIME__);
    Protocol_SendDebugMessage(message);
    unsigned short currentAngle, previousAngle;
    int previousTime, reportSpeed_previousTime, controlLoop_previousTime;
    int payloadSpeed;
    int angularVelocity;
    unsigned char messageID;
    FeedbackControl_SetProportionalGain(10);
    FeedbackControl_SetIntegralGain(1);
    while ((FreeRunningTimer_GetMilliSeconds() - previousTime) < 200) {
    } // 200ms delay 
    previousTime = FreeRunningTimer_GetMilliSeconds();
    reportSpeed_previousTime = FreeRunningTimer_GetMilliSeconds();
    controlLoop_previousTime = FreeRunningTimer_GetMilliSeconds();
    while (1) {
        if (U1STAbits.OERR) // UART 1 Protocol receiver buffer overrun
            U1STAbits.OERR = 0;
        if (Protocol_IsMessageAvailable() == TRUE) {    // Check for messages
            messageID = Protocol_ReadNextID();
            if (messageID == ID_COMMANDED_RATE) { // Command rate
                Protocol_GetPayload(&payloadSpeed);
                payloadSpeed = Protocol_IntEndednessConversion(payloadSpeed);
                feedback_update.reference = payloadSpeed;
                if (payloadSpeed > 0) { // Forward
                    IN1 = 0;
                    IN2 = 1;
                } else if (payloadSpeed < 0) { // Reverse
                    IN1 = 1;
                    IN2 = 0;
                }
            } else if (messageID == ID_FEEDBACK_SET_GAINS) {    // Set gains
                FeedbackControl_ResetController();
                Protocol_GetPayload(payload.FeedbackGains);
                FeedbackControl_SetProportionalGain(Protocol_IntEndednessConversion(payload.proportional));
                FeedbackControl_SetIntegralGain(Protocol_IntEndednessConversion(payload.integral));
                FeedbackControl_SetDerivativeGain(Protocol_IntEndednessConversion(payload.derivative));
                Protocol_SendDebugMessage("Set Gainz");
                Protocol_SendMessage(0, ID_FEEDBACK_SET_GAINS_RESP, payload.FeedbackGains); // Send set gain response
            }
        }
        /*Free running timer*/
        if ((FreeRunningTimer_GetMilliSeconds() - previousTime) > 1) { // 1000Hz calculate motor speed
            currentAngle = RotaryEncoder_ReadRawAngle();
            if (((currentAngle - previousAngle) > -3000) && ((currentAngle - previousAngle) < 3000)) {  // Discard erroneous angular velocity
                angularVelocity = currentAngle - previousAngle;
            }
            previousAngle = currentAngle; // Save angle as previous
            if ((angularVelocity > MAX_RATE) && IN1) // Negative velocity
                angularVelocity -= ENCODER_ROLLOVER;
            if ((angularVelocity < -MAX_RATE) && IN2) // Positive velocity
                angularVelocity += ENCODER_ROLLOVER;
            angularVelocity *= TIME_DELTA; // Converts angular velocity to degrees per second
            angularVelocity /= GEAR_RATIO;
            angularVelocity /= DEGREE_CONVERT;
            previousTime = FreeRunningTimer_GetMilliSeconds();
        }
        if ((FreeRunningTimer_GetMilliSeconds() - reportSpeed_previousTime) > 100) { //100Hz ID_REPORT_SPEED
            angularVelocity = Protocol_IntEndednessConversion(angularVelocity);
            Protocol_SendMessage(4, ID_REPORT_RATE, &angularVelocity);
            reportSpeed_previousTime = FreeRunningTimer_GetMilliSeconds();
        }
        if ((FreeRunningTimer_GetMilliSeconds() - controlLoop_previousTime) > 5) { //200Hz ID_REPORT_FEEDBACK
            if ((angularVelocity > -3000) && (angularVelocity < 3000)) { // Reject erroneous angular velocities
                feedback_update.feedback_rate = FeedbackControl_Update(feedback_update.reference * COMMAND_RATE_TO_ANGULAR, angularVelocity) / 5; // Run PID loop with scaling for PWM
                if ((feedback_update.feedback_rate < 1000) && (feedback_update.feedback_rate > -1000)) { // Feedback control nominal
                        DCMotorDrive_SetMotorSpeed(feedback_update.feedback_rate);
                        if (feedback_update.reference == 0) { // Apply break when PWM set to 0
                            DCMotorDrive_SetMotorSpeed(0);
                            DCMotorDrive_SetBrake();
                        }
                        /*Int endian conversion*/
                        report_feedback.current_rate = Protocol_IntEndednessConversion(angularVelocity);
                        report_feedback.PWM = Protocol_IntEndednessConversion(DCMotorControl_GetMotorSpeed());
                        report_feedback.error = Protocol_IntEndednessConversion((feedback_update.reference * COMMAND_RATE_TO_ANGULAR) - angularVelocity);
                        /*Send REPORT FEEDBACK*/
                        Protocol_SendMessage(12, ID_REPORT_FEEDBACK, report_feedback.FeedbackUpdateValues);
                    } 
                else { // Feedback control run-off, reset integrator/accumulator
                    FeedbackControl_ResetController();
                }
            } // Reject erroneous angular velocity
            controlLoop_previousTime = FreeRunningTimer_GetMilliSeconds();
        } // 200Hz Report Feedback
    } // while (1)
    BOARD_End();
} // main()