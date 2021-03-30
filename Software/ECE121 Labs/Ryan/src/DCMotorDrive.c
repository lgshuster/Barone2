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

#define IN1 LATFbits.LATF1
#define IN2 LATDbits.LATD1

//#define TEST_HARNDESS_DCMOTORDRIVE

#define MAX_RATE 1000
#define ENCODER_ROLLOVER 0x3FFF
#define PWM_FREQUENCY 2000
#define GEAR_RATIO 84
#define DEGREE_CONVERT 46
#define TIME_DELTA 500

static int pulsePeriod;
static unsigned char message[MAXPAYLOADLENGTH];

int DCMotorDrive_Init(void) {
    /*T3CON registers*/
    T3CON = 0;
    T3CONbits.TCKPS = 0b011; // Pre-scale value = 1:8
    PR3 = 5000000 / PWM_FREQUENCY; // PWM frequency rollover OC1
    //    sprintf(message, "Roll over: %d\n", PR3);
    //    Protocol_SendDebugMessage(message);
    /*Output Capture registers*/
    OC1CONbits.OCTSEL = 1; // Set Timer 3 as clock source
    OC1CONbits.OCM = 0b110; // PWM mode, no fault
    //    OC1CONbits.OC32 = 1;    //32-bit OC buffer mode
    OC1RS = 0; // Falling edge 
    /*T3 Interrupt*/
    IFS0bits.T3IF = 0;
    IPC3bits.T3IP = 3; //T3 level 3 priority
    /*variables*/
    pulsePeriod = 0;
    /*Set IN1 IN2 IO port*/
    TRISFbits.TRISF1 = 0;
    TRISDbits.TRISD1 = 0;
    IN1 = 0;
    IN2 = 1;
    /*Enable Registers*/
    T3CONbits.ON = 1;
    OC1CONbits.ON = 1;
    IEC0bits.T3IE = 1;
}

int DCMotorDrive_SetMotorSpeed(int newMotorSpeed) {
    //    sprintf(message,"Set motor speed: %d\n", abs(newMotorSpeed));
    //    Protocol_SendDebugMessage(message);
    if ((abs(newMotorSpeed) > MAXMOTORSPEED) || (abs(newMotorSpeed) < 0)) {
        return ERROR;
    } else {
        if (newMotorSpeed > 0) { // Forward
            IN1 = 0;
            IN2 = 1;
        } else if (newMotorSpeed < 0) { // Reverse
            IN1 = 1;
            IN2 = 0;
        }
        pulsePeriod = abs(newMotorSpeed) * 5;
        return SUCCESS;
    }
}

int DCMotorControl_GetMotorSpeed(void) {
    return (OC1R * 1000 / (PR3 * 2));
}

int DCMotorDrive_SetBrake(void){
    IN1 = 0;
    IN2 = 0;
    return SUCCESS;
}

/*Interrupt handler*/
void __ISR(_TIMER_3_VECTOR) Timer3IntHandler(void) {
    IFS0bits.T3IF = 0;
    OC1RS = pulsePeriod;
}

#ifdef TEST_HARNDESS_DCMOTORDRIVE

void main(void) {
    BOARD_Init();
    Protocol_Init();
    RotaryEncoder_Init(ENCODER_INTERRUPT_MODE);
    DCMotorDrive_Init();
    FreeRunningTimer_Init();
    sprintf(message, "DC Motor Dive compiled on: %s %s\n", __DATE__, __TIME__);
    Protocol_SendDebugMessage(message);
    unsigned short parityBit_currentAngle, currentAngle, previousAngle;
    int previousTime, reportSpeed_previousTime;
    int payloadSpeed, payloadSpeed_response;
    int angularVelocity;
    unsigned char messageID;
    while ((FreeRunningTimer_GetMilliSeconds() - previousTime) < 200) {
    } // 200ms delay 
    previousTime = FreeRunningTimer_GetMilliSeconds();
    reportSpeed_previousTime = FreeRunningTimer_GetMilliSeconds();
    while (1) {
        if (U1STAbits.OERR) // UART 1 Protocol receiver buffer overrun
            U1STAbits.OERR = 0;
        if (Protocol_IsMessageAvailable() == TRUE) {
            messageID = Protocol_ReadNextID();
            if (messageID == ID_COMMAND_OPEN_MOTOR_SPEED) {
                Protocol_GetPayload(&payloadSpeed);
                payloadSpeed = Protocol_IntEndednessConversion(payloadSpeed);
                if (payloadSpeed > 0) { // Forward
                    IN1 = 0;
                    IN2 = 1;
                } else if (payloadSpeed < 0) { // Reverse
                    IN1 = 1;
                    IN2 = 0;
                }
                DCMotorDrive_SetMotorSpeed(payloadSpeed);
                payloadSpeed_response = Protocol_IntEndednessConversion(payloadSpeed);
                Protocol_SendMessage(4, ID_COMMAND_OPEN_MOTOR_SPEED_RESP, &payloadSpeed_response);
            }
        }
        if ((FreeRunningTimer_GetMilliSeconds() - previousTime) > 1) { // 10000Hz velocity calculation
            currentAngle = RotaryEncoder_ReadRawAngle();
            angularVelocity = currentAngle - previousAngle; // Velocity
            previousAngle = currentAngle; // Save angle as previous
            if ((angularVelocity > MAX_RATE) && IN1) // Negative velocity
                angularVelocity -= ENCODER_ROLLOVER;
            if ((angularVelocity < -MAX_RATE) && IN2) // Positive velocity
                angularVelocity += ENCODER_ROLLOVER;
            angularVelocity *= TIME_DELTA;
            angularVelocity /= GEAR_RATIO;
            angularVelocity /= DEGREE_CONVERT;
            previousTime = FreeRunningTimer_GetMilliSeconds();
        }
        if ((FreeRunningTimer_GetMilliSeconds() - reportSpeed_previousTime) > 100) { //100Hz ID_REPORT_SPEED
            angularVelocity = Protocol_IntEndednessConversion(angularVelocity);
            Protocol_SendMessage(4, ID_REPORT_RATE, &angularVelocity);
            reportSpeed_previousTime = FreeRunningTimer_GetMilliSeconds();
        }
    }
    BOARD_End();
}
#endif