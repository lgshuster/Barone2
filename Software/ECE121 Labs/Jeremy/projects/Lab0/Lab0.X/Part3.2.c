/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.
 */
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <GenericTypeDefs.h>
#include <xc.h>
#include <sys/attribs.h>
#include <proc/p32mx340f512h.h>

#include "BOARD.h"
#include "Buttons.h"
#include "LEDS.h"

#define PIN7 PORTDbits.RD9
#define PIN3 TRISDbits.TRISD0
#define PIN6 LATDbits.LATD2
//trisd sets the corresponding pins to output
//portd sets a pin to read inputs

void NOP_1ms();

int main(void)
{
    BOARD_Init(); //Initializes Board functions
    LEDS_Init();
    int i;
    int NOP = 100;
    int BusBroken = 0;
    
    PIN3 = 0; //sets pin 3 to output 3.3V for 5V regulator
    ODCDbits.ODCD2 = 1; //sets pin 6 to Open Drain mode
    TRISDbits.TRISD6 = 0;//sets pin 6 to output mode
    LATDbits.LATD2 = 0; //initializes output of pin 6 to 1
    TRISD |= 0x0200; //trisd bits makes PIN7 an input for bus
    
    while (1) {
        for (i = 0; i < (NOP); i++) { //.1 second delay between actions(10hz))
            NOP_1ms();
        }
        if (PIN7 == 0){ //If there is a nonzero voltage detected, set flag for potential bus broken
            BusBroken = 1;
        } else{
            BusBroken = 0;
        }
        PIN6 = !PIN6; //turn the open drain output on/off
        if (PIN7 != 0){ //If the input still reads 0 again, bus is broken
            BusBroken = 0;
        }
        LEDS_Set(BusBroken); //turns on LED8 if bus is broken
    }
    BOARD_End();
}

void NOP_1ms() //counts up to simulate a NOP, takes about 1ms
{
    int j;
    for (j = 0; j < 1162; j++) {
    }
}