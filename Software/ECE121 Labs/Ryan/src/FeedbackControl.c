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

//#define TEST_HARNDESS_FEEDBACKCONTROL
#define MAX_CONTROL_OUTPUT 1 << FEEDBACK_MAXOUTPUT_POWER

static int pulsePeriod;
static unsigned char message[MAXPAYLOADLENGTH];

static int P, I, D;
static int accumulator, previous_sensorValue;

int FeedbackControl_Init(void) {
    FeedbackControl_SetProportionalGain(1);
    FeedbackControl_SetIntegralGain(0);
    FeedbackControl_SetDerivativeGain(0);
    FeedbackControl_ResetController();
    return SUCCESS;
}

int FeedbackControl_SetProportionalGain(int newGain) {
    P = newGain;
    return SUCCESS;
}

int FeedbackControl_SetIntegralGain(int newGain) {
    I = newGain;
    return SUCCESS;
}

int FeedbackControl_SetDerivativeGain(int newGain) {
    D = newGain;
    return SUCCESS;
}

int FeedbackControl_GetProportionalGain(void) {
    return P;
}

int FeedbackControl_GetIntegralGain(void) {
    return I;
}

int FeedbackControl_GetDerivativeGain(void) {
    return D;
}

int FeedbackControl_Update(int referenceValue, int sensorValue) {
    int error, u, derivative;
    error = referenceValue - sensorValue; // Error
    accumulator += error; // Integrate error
    derivative = -(sensorValue - previous_sensorValue); // Derivative
    previous_sensorValue = sensorValue;
    u = (P * error) + (I * accumulator) + (D * derivative); // Compute control
    if (u > MAX_CONTROL_OUTPUT) {   // Too fast forward
        u = MAX_CONTROL_OUTPUT; // Clip control output high
        accumulator -= error;
    } else if (u < -MAX_CONTROL_OUTPUT) {   // Too fast backward
        u = -MAX_CONTROL_OUTPUT;    // Clip control output low
        accumulator -= error;
    }
    sprintf(message, "u: %d error: %d proportional: %d integral: %d derivative: %d", u, error, P * error, I * accumulator, D * derivative);
//    Protocol_SendDebugMessage(message);
    return u; //Input to set motor speed function
}

int FeedbackControl_ResetController(void) {
    accumulator = 0;
    previous_sensorValue = 0;
    return SUCCESS;
}
#ifdef TEST_HARNDESS_FEEDBACKCONTROL


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
    };
    unsigned char FeedbackUpdateValues[8];
};
union values payload;
union feedback_update feedback_update;

void main(void) {
    int feedbackOutput, null;
    BOARD_Init();
    Protocol_Init();
    FeedbackControl_Init();
    sprintf(message, "Feedback control compiled on: %s %s\n", __DATE__, __TIME__);
    Protocol_SendDebugMessage(message);
    unsigned char messageID;
    while (1) {
        if (Protocol_IsMessageAvailable() == TRUE) {
            messageID = Protocol_ReadNextID();
            if (messageID == ID_FEEDBACK_SET_GAINS) {
                Protocol_GetPayload(payload.FeedbackGains);
                FeedbackControl_SetProportionalGain(Protocol_IntEndednessConversion(payload.proportional));
                FeedbackControl_SetIntegralGain(Protocol_IntEndednessConversion(payload.integral));
                FeedbackControl_SetDerivativeGain(Protocol_IntEndednessConversion(payload.derivative));
                Protocol_SendDebugMessage("Set Gainz");
                Protocol_SendMessage(0, ID_FEEDBACK_SET_GAINS_RESP, payload.FeedbackGains); // Send set gain response
            } else if (messageID == ID_FEEDBACK_REQ_GAINS) {
                Protocol_GetPayload(&null);
                payload.proportional = Protocol_IntEndednessConversion(FeedbackControl_GetProportionalGain());
                payload.integral = Protocol_IntEndednessConversion(FeedbackControl_GetIntegralGain());
                payload.derivative = Protocol_IntEndednessConversion(FeedbackControl_GetDerivativeGain());
                Protocol_SendMessage(12, ID_FEEDBACK_CUR_GAINS, payload.FeedbackGains); // Send current gain response
            } else if (messageID == ID_FEEDBACK_RESET_CONTROLLER) {
                Protocol_GetPayload(&null);
                FeedbackControl_ResetController();
                Protocol_SendDebugMessage("Reset");
                Protocol_SendMessage(0, ID_FEEDBACK_RESET_CONTROLLER_RESP, payload.FeedbackGains); // Send reset response 
            } else if (messageID == ID_FEEDBACK_UPDATE) {
                Protocol_GetPayload(feedback_update.FeedbackUpdateValues);
                feedbackOutput = FeedbackControl_Update(Protocol_IntEndednessConversion(feedback_update.reference), Protocol_IntEndednessConversion(feedback_update.sensor));
                feedbackOutput = Protocol_IntEndednessConversion(feedbackOutput);
                Protocol_SendMessage(4, ID_FEEDBACK_UPDATE_OUTPUT, &feedbackOutput); // Send Update Output response
            }
        }
    }
    BOARD_End();
}
#endif