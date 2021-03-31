/* 
 * File:   LEDS.c
 * Author: Jeremy
 *
 * Created on January 6, 2021, 6:08 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GenericTypeDefs.h>
#include "BOARD.h"
// Microchip libraries
#include <xc.h>
#include <sys/attribs.h>
// User libraries
#include "LEDS.h"
/**
 * @file
 * This library provides an interface for controlling the LEDs on the 
 * chipKit Basic I/O Shield development board.
 *
 * This file provides prototypes for functions defined in Leds.c, which is appropriate for Lab06 ONLY.
 * Future labs will use a header-only version of the file.
 */

/**
 * LEDS_INIT() initializes the LED bar by doing two things:
 *      1) Use the appropriate SFRs to set each LED pin to "output" mode.
 *      2) Use the appropriate SFRs to set each LED pin's output value to 0 (low voltage).
 * 
 * After calling LEDS_INIT(), the other functions in this file can be used to manipulate the LED bar.
 */
void LEDS_Init(void)
{
    TRISEbits.TRISE0 = 0; //Outputs mode for all 8 LEDS
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE3 = 0;
    TRISEbits.TRISE4 = 0;
    TRISEbits.TRISE5 = 0;
    TRISEbits.TRISE6 = 0;
    TRISEbits.TRISE7 = 0;

    LATEbits.LATE0 = 0; //Turns all outputs to 0
    LATEbits.LATE1 = 0;
    LATEbits.LATE2 = 0;
    LATEbits.LATE3 = 0;
    LATEbits.LATE4 = 0;
    LATEbits.LATE5 = 0;
    LATEbits.LATE6 = 0;
    LATEbits.LATE7 = 0;

}

/**
 * LEDS_SET() controls the output on the LED bar.  Input values are 8-bit patterns, where each bit 
 * describes the desired state of the corresponding LED.  So, for example, LEDS_SET(0x80) should 
 * leave the first LED on, and the last 7 LEDs off.  LEDS_SET(0x1F) should turn off the first three
 * LEDs and turn on the remaining five LEDs.
 * 
 * @param newPattern:  An 8-bit value describing the desired output on the LED bar.  
 * 
 * LEDS_SET should not change any LED pins to inputs.
 */
void LEDS_Set(char newPattern)
{
    unsigned char yo = newPattern; //Does bit masking to input each bit of newPattern into LED bits
    LATEbits.LATE0 = (unsigned int) yo >> 7; //In this way, the LED bits are actually reversed from the nexPattern bits
    LATEbits.LATE1 = ((unsigned int) yo << 1) >> 7;
    LATEbits.LATE2 = ((unsigned int) yo << 2) >> 7;
    LATEbits.LATE3 = ((unsigned int) yo << 3) >> 7;
    LATEbits.LATE4 = ((unsigned int) yo << 4) >> 7;
    LATEbits.LATE5 = ((unsigned int) yo << 5) >> 7;
    LATEbits.LATE6 = ((unsigned int) yo << 6) >> 7;
    LATEbits.LATE7 = ((unsigned int) yo << 7) >> 7;
}

/**
 * LEDS_GET() returns the current state of the LED bar.  Return values are 8-bit patterns,
 * where each bit describes the current state of the corresponding LED So, for example, if the 
 * led bar's led's are [ON OFF ON OFF   OFF ON OFF ON], LEDS_GET() should return 0xA5.
 * 
 * @return char:  An 8-bit value describing the current output on the LED bar.  
 * 
 * LEDS_GET() should not change the state of the LEDS, or any SFRs.
 */
char LEDS_Get(void)
{ //returns char form of hex value of LATE
    unsigned int test = (LATEbits.LATE7 + LATEbits.LATE6 * 2 + //Does binary math to find value of hex LATE value 
            LATEbits.LATE5 * 4 + LATEbits.LATE4 * 8 + LATEbits.LATE3 * 16 //converts to char to return
            + LATEbits.LATE2 * 32 + LATEbits.LATE1 * 64 + LATEbits.LATE0 * 128);
    unsigned char test1 = (unsigned char) test;
    return (test1);
}

