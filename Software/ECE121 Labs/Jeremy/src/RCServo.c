#include "BOARD.h"
#include "Protocol.h"
#include "MessageIDs.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <GenericTypeDefs.h>
#include <xc.h>
#include <sys/attribs.h>

static int pulseup;
static int pulsedown;

#define PIN6 LATDbits.LATD2 //output compare pin 6

/**
 * @Function RCServo_Init(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief initializes hardware required and set it to the CENTER PULSE */
int RCServo_Init(void)
{
    T3CON = 0; //sets up timer3
    T3CON = 0x0050; // 16-bit mode, 1:32 prescale, ticks up .8us
    TMR3 = 0; // Clear timer registers for TMR3
    PR3 = 62500; //sets 16 bit period to 62500 to get 50ms interrupts
    IFS0bits.T3IF = 0; //clears interrupt flag
    IPC3bits.T3IP = 5; //sets timer3 interrupt priority
    IEC0bits.T3IE = 1; //enables timer3 interrupts,
    T3CON |= 0x8000; //starts timer 3

    TRISDbits.TRISD2 = 0; //sets pin 6 to output

    OC3CON = 0;
    OC3CONbits.OCTSEL = 1; //puts timer 3 to output compare
    OC3CONbits.OCM = 5; //dual output compare continuous pulses

    IFS0bits.OC3IF = 0;
    IPC3bits.OC3IP = 6; //sets interrupts priority to 6
    IEC0bits.OC3IE = 1;

    OC3CONbits.ON = 1; //turns on output compare 3 in pin 6
}

/**
 * @Function int RCServo_SetPulse(unsigned int inPulse)
 * @param inPulse, integer representing number of microseconds
 * @return SUCCESS or ERROR
 * @brief takes in microsecond count, converts to ticks and updates the internal variables
 * @warning This will update the timing for the next pulse, not the current one */
int RCServo_SetPulse(unsigned int inPulse)
{
    if (inPulse >= 600 && inPulse <= 2400) {
        pulseup = 500; //sets upper pulse to occur when timer reset after 500us
        OC3R = 500 * 5 / 4; //how many ticks required first for 500us
        pulsedown = pulseup + inPulse; //sets lower pulse to occur in microseconds after first pulse;
        OC3RS = pulsedown * 5 / 4; //sets OC3RS to timer ticks of microseconds if each tick is .8us
    }
    return (1);
}

/**
 * @Function int RCServo_GetPulse(void)
 * @param None
 * @return Pulse in microseconds currently set */
unsigned int RCServo_GetPulse(void)
{
    return (pulsedown - pulseup); //returns in microseconds the pulse width
}

/**
 * @Function int RCServo_GetRawTicks(void)
 * @param None
 * @return raw timer ticks required to generate current pulse. */
unsigned int RCServo_GetRawTicks(void)
{
    return ((pulsedown - pulseup) * 5 / 4); //returns pulse width in timer ticks
}

void __ISR(_OUTPUT_COMPARE_3_VECTOR) __OC3Interrupt(void)
{
    IFS0bits.OC3IF = 0;
    return;
}

void __ISR(_TIMER_3_VECTOR) Timer3IntHandler(void)
{
    IFS0bits.T3IF = 0; //clears interrupt flag
    return;
}
#ifdef RCServo_TEST

int main(void)
{
    BOARD_Init();
    Protocol_Init(); //initializes board and UART1 protocol
    LEDS_INIT();

    char testMessage[MAXPAYLOADLENGTH];
    sprintf(testMessage, "Servo Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(testMessage); //sends debug message
    
    RCServo_SetPulse(1500);
    RCServo_Init();
    while (1) { //main loop
        if (Protocol_IsMessageAvailable() && Protocol_ReadNextID() == ID_COMMAND_SERVO_PULSE) {
            unsigned char payload[MAXPAYLOADLENGTH];
            void *ptr = &payload;
            Protocol_GetPayload(ptr); //gets message payload of pulse length
            unsigned int servo = (payload[0] << 24) + (payload[1] << 16) + (payload[2] << 8) + payload[3]; //little endian conversion

            RCServo_SetPulse(servo); //sets pulse to message payload
            servo = servo / 4 * 5; //turns microsecond pulse to timer ticks
            payload[0] = (servo >> 24); // big endian conversions to return correct value
            payload[1] = ((servo << 8) >> 24);
            payload[2] = ((servo << 16) >> 24);
            payload[3] = ((servo << 24) >> 24);
            Protocol_SendMessage(4, ID_SERVO_RESPONSE, ptr); //returns length of pulse in timer ticks
        }
    }
}
#endif