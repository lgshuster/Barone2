//Section 3.2 Bus Contention
#include <xc.h>
#include <stdio.h>
#include "BOARD.h"
#include "NOP.h"
#include "LED.h"

#define PORT PORTDbits.RD8

/* Pin 2 (D8) is I/0
   Pin 1 (F3) is ODC
 */
uint8_t savedBus;   //saves previous bus signal

void verifyBus(){
    savedBus |= PORTDbits.RD8;    //stores latest input
    
    if (((savedBus & 0b00000010) == 0b00000000) && (PORTDbits.RD8 == 1)) //if previous is '0' and now is '1'
        setLEDS(0);
    if (((savedBus & 0b00000010) == 0b00000010) && (PORTDbits.RD8 == 0)) //if previous is '1' and now is '0'
        setLEDS(0);
    if (((savedBus & 0b00000010) == 0b00000000) && (PORTDbits.RD8 == 0)) //if previous is '0' and now is '0' ERROR
        setLEDS(0b10000000);
    if (((savedBus & 0b00000010) == 0b00000010) && (PORTDbits.RD8 == 1)) //if previous is '1' and now is '1' ERROR
        setLEDS(0b10000000);
    savedBus = savedBus << 1;       //set current recorded bit as previous
}

//If there is bus contention, LD7 (LED) will turn on
void main(void){
    initLEDS();
    TRISFbits.TRISF3 = 0;   //Set pin 1 as output
    ODCFbits.ODCF3 = 1;     //Set pin 1 as output drain
    
    while (1){                          //Only checks bus when latch is closed
        LATFbits.LATF3 = 1;     //pull up
        verifyBus();
        NOP_delay(0.05);
        LATFbits.LATF3 = 0;     //pull down
        verifyBus();
        NOP_delay(0.05);
    }
}