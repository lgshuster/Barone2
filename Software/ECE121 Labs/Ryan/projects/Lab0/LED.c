#include <proc/p32mx340f512h.h>
#include <xc.h>
#include "LED.h"

void initLEDS (void){
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE3 = 0;
    TRISEbits.TRISE4 = 0;
    TRISEbits.TRISE5 = 0;
    TRISEbits.TRISE6 = 0;
    TRISEbits.TRISE7 = 0;
    
    LATEbits.LATE0 = 0;
    LATEbits.LATE1 = 0;
    LATEbits.LATE2 = 0;
    LATEbits.LATE3 = 0;
    LATEbits.LATE4 = 0;
    LATEbits.LATE5 = 0;
    LATEbits.LATE6 = 0;
    LATEbits.LATE7 = 0;
}

void setLEDS (uint8_t LEDS){
    LATEbits.LATE0 = (uint32_t) (LEDS & 0b00000001);
    LATEbits.LATE1 = (uint32_t) ((LEDS & 0b00000010) >> 1);
    LATEbits.LATE2 = (uint32_t) ((LEDS & 0b00000100) >> 2);
    LATEbits.LATE3 = (uint32_t) ((LEDS & 0b00001000) >> 3);
    LATEbits.LATE4 = (uint32_t) ((LEDS & 0b00010000) >> 4);
    LATEbits.LATE5 = (uint32_t) ((LEDS & 0b00100000) >> 5);
    LATEbits.LATE6 = (uint32_t) ((LEDS & 0b01000000) >> 6);
    LATEbits.LATE7 = (uint32_t) ((LEDS & 0b10000000) >> 7);
}

/* *****************************************************************************
 End of File
 */
