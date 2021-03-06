#include <proc/p32mx340f512h.h>
#include <sys/attribs.h>
#include <string.h>
#include <stdio.h>
#include "BOARD.h"
#include "Protocol.h"
#include "MessageIDs.h"
#include "RCServo.h"
//#define TEST_HARNESS_RCSERVO

unsigned int pulsePeriod, payload;
unsigned int getPulse, sendPayload;
char message[MAXPAYLOADLENGTH];
/**
 * @Function RCServo_Init(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief initializes hardware required and set it to the CENTER PULSE */
int RCServo_Init(void){
    /*T3CON registers*/
    T3CON = 0;
    T3CONbits.TCKPS = 0b101;    // Pre-scale value = 1:32
    PR3 = 62500;   // Roll over every 50 millisecond
    /*Output Capture registers*/
    OC3CONbits.OCTSEL = 1;  // Set Timer 3 as clock source
    OC3CONbits.OCM = 0b101; // PWM mode
//    OC3CONbits.OC32 = 1;
    OC3R = 0;   // Rising edge at last tick of PR3
    OC3RS = 1875;   // Falling edge every 1500 micro seconds
    /*OC3 Interrupt registers*/
    IEC0bits.OC3IE = 0;
    IPC3bits.OC3IP = 3;
    IFS0bits.OC3IF = 0;
    /*Enable Registers*/
    T3CONbits.ON = 1;
    OC3CONbits.ON = 1;
    IEC0bits.OC3IE = 1;
    /*variables*/
    payload = 1500;
    pulsePeriod = 1875;
    return SUCCESS;
}

int RCServo_SetPulse(unsigned int inPulse){
    if ((inPulse > 2400) || (inPulse < 600))
        return ERROR;
    else {
        pulsePeriod = inPulse * 5 / 4;
        return SUCCESS;
    }
}

unsigned int RCServo_GetPulse(void){
    return (OC3RS * 4 / 5); // In microseconds
}

unsigned int RCServo_GetRawTicks(void){
    return OC3RS;
}

int positionCorrect(void){
    unsigned int upperBound, lowerBound;
    lowerBound = RCServo_GetPulse() -  1;
    upperBound = RCServo_GetPulse() +  1;
        if ((payload >= lowerBound) && (payload <= upperBound))    // Current pulse does not match new pulse
            return SUCCESS;
        else
            return ERROR;
}

void __ISR ( _OUTPUT_COMPARE_3_VECTOR ) __OC3Interrupt ( void ) {
    if (OC3RS != pulsePeriod)
        OC3RS = pulsePeriod;
    IFS0bits.OC3IF = 0;
}

#ifdef TEST_HARNESS_RCSERVO
void main(){
    BOARD_Init();
    Protocol_Init();
    RCServo_Init();
    sprintf(message,"RC Servo compiled on: %s %s\n", __DATE__,__TIME__);
    Protocol_SendDebugMessage(message);
    while(1){
        if ((Protocol_IsMessageAvailable() == TRUE) && (Protocol_ReadNextID() == ID_COMMAND_SERVO_PULSE)){    // Message received
            Protocol_GetPayload(&payload);  // Get new PWN
            payload = Protocol_IntEndednessConversion(payload);   // Endian the new PWM
            if (RCServo_SetPulse(payload) == ERROR){
                sprintf(message,"ID_COMMAND_SERVO_PULSE out of bounds!", __DATE__,__TIME__);
                Protocol_SendDebugMessage(message);
                payload = RCServo_GetPulse();
            }
        }
        if (positionCorrect() == ERROR){    // Current pulse does not match new pulse
            RCServo_SetPulse(payload);
            sendPayload = Protocol_IntEndednessConversion(payload);
            Protocol_SendMessage(4, ID_SERVO_RESPONSE, &sendPayload);    // Send back new pulse
        }
    }
    BOARD_End();
}
#endif