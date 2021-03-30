#include "Board.h"
#include "Protocol.h"
#include "MessageIDs.h"
#include "DCMotorDrive.h"
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

#define ENA LATDbits.LATD2 //output compare pin 6
#define IN1 LATDbits.LATD1 //pin 5 In1 pin
#define IN2 LATDbits.LATD0 //pin 3 In2 pin
#define ENCODER_ROLLOVER 0x3FFF
#define MAXSPEED 1000
#define GEARRATIO 84
#define ENCODERTICKSINDEGREES 46

/**
 * @Function DCMotorDrive_Init(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief initializes timer3 to 2Khz and set up the pins
 * @warning you will need 3 pins to correctly drive the motor  */
int DCMotorDrive_Init(void)
{
    T3CON = 0; //sets up timer3
    T3CON = 0x0060; // 16-bit mode, 1:32 prescale, ticks up .8us
    TMR3 = 0; // Clear timer registers for TMR3
    PR3 = 625; //sets 16 bit period to 2500 to get .5ms interrupts 2khz
    IFS0bits.T3IF = 0; //clears interrupt flag
    IPC3bits.T3IP = 5; //sets timer3 interrupt priority
    IEC0bits.T3IE = 1; //enables timer3 interrupts,
    T3CON |= 0x8000; //starts timer 3

    TRISDbits.TRISD2 = 0; //sets pin 6 to output to ENA
    OC3RS = 0; //sets 0% duty cycle init
    TRISDbits.TRISD1 = 0; //sets pin 5 to output IN1
    IN1 = 1; //initializes forward motion
    TRISDbits.TRISD0 = 0; //sets pin 3 to output IN2
    IN2 = 0;

    OC3CON = 0; //no OC3 interrupt needed for PWM
    OC3CONbits.OCTSEL = 1; //puts timer 3 to output compare
    OC3CONbits.OCM = 6; //PWM mode

    OC3CONbits.ON = 1; //turns on output compare 3 in pin 6
    return (SUCCESS);
}

/**
 * @Function DCMotorDrive_SetMotorSpeed(int newMotorSpeed)
 * @param newMotorSpeed, in units of Duty Cycle (+/- 1000)
 * @return SUCCESS or ERROR
 * @brief Sets the new duty cycle for the motor, 0%->0, 100%->1000 */
int DCMotorDrive_SetMotorSpeed(int newMotorSpeed)
{
    int speed;
    if (newMotorSpeed < 0) { //gets absolute value of speed
        speed = newMotorSpeed * -1;
    } else {
        speed = newMotorSpeed;
    }
    if (newMotorSpeed < 0) { //changes direction of current if negative or positive
        IN1 = 0;
        IN2 = 1;
    }
    if (newMotorSpeed > 0) {
        IN1 = 1;
        IN2 = 0;
    }
    OC3RS = (PR3 * speed) / 1000; //Puts new duty cycle in output compare in percentage
    return (SUCCESS); //based off of period of timer 3(out of 625)
}

/**
 * @Function DCMotorControl_GetMotorSpeed(void)
 * @param None
 * @return duty cycle of motor 
 * @brief returns speed in units of Duty Cycle (+/- 1000) */
int DCMotorControl_GetMotorSpeed(void)
{
    int duty = ((OC3R * 1000) / PR3); //gets duty cycle in percentage
    if (IN2 == 1) { 
        duty = duty * -1; //turns negative if speed is negative
    }
    return (duty);
}

/**
 * @Function DCMotorDrive_SetBrake(void)
 * @param None
 * @return SUCCESS or FAILURE
 * @brief set the brake on the motor for faster stop */
int DCMotorDrive_SetBrake(void)
{
    IN1 = 0; //sets dynamic braking
    IN2 = 0;
    return (SUCCESS);
}

void __ISR(_TIMER_3_VECTOR) Timer3IntHandler(void)
{
    IFS0bits.T3IF = 0; //clears interrupt flag
    return;
}
//#define MOTOR_TEST
#ifdef MOTOR_TEST

int main(void)
{
    BOARD_Init();
    Protocol_Init();
    LEDS_INIT();

    char testMessage[MAXPAYLOADLENGTH];
    sprintf(testMessage, "DC Motor Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(testMessage); //sends debug message

    FreeRunningTimer_Init(); //initializes modules
    DCMotorDrive_Init();
    RotaryEncoder_Init(1);

    unsigned int current;
    unsigned int previous;
    unsigned char payload[MAXPAYLOADLENGTH];
    unsigned int target = 10; //sends 100hz
    int motorspeed;
    int rate;
    while (1) {
        if (Protocol_IsMessageAvailable()) {
            if (Protocol_ReadNextID() == ID_COMMAND_OPEN_MOTOR_SPEED) {
                Protocol_GetPayload(&motorspeed); 
                motorspeed = Protocol_IntEndednessConversion(motorspeed); //gets new motorspeed in correct form
                if (motorspeed == 0) { //sets brake if speed is set to 0
                    DCMotorDrive_SetBrake();
                }
                DCMotorDrive_SetMotorSpeed(motorspeed); //sets new motorspeed out of +-1000
                Protocol_SendMessage(0, ID_COMMAND_OPEN_MOTOR_SPEED, NULL);

            }
        }
        if (FreeRunningTimer_GetMilliSeconds() >= target) {
            target += 10;
            current = RotaryEncoder_ReadRawAngle();
            rate = current - previous; //gets rate from previous and current angle
            if ((rate > MAXSPEED) && IN2) {
                rate = rate - (ENCODER_ROLLOVER); //if a rollover, add or subtract max encoder value
            }
            if ((rate < (MAXSPEED * -1)) && IN1) {
                rate = rate + (ENCODER_ROLLOVER);
            }
            previous = current;
            rate = (rate * 100) / GEARRATIO / ENCODERTICKSINDEGREES; //converts rate to degrees per second of top motor

            rate = Protocol_IntEndednessConversion(rate);
            payload[1] = (rate & 0xFF); // big endian conversions to return correct value
            payload[0] = ((rate & 0xFF00) >> 8);
            Protocol_SendMessage(4, ID_REPORT_RATE, &rate); //returns rate in message(degrees per second)
        }
        if (U1STAbits.OERR == 1) { //clears SPI or UART overflow errors
            U1STAbits.OERR = 0;
        }
        if (SPI2STATbits.SPIROV == 1) {
            SPI2STATCLR = 1 << 6;
        }
    }
}
#endif