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

#include "BOARD.h"
#include "Buttons.h"

#define PIN5 TRISDbits.TRISD1
#define PIN6 TRISDbits.TRISD2
#define PIN7 PORTDbits.RD9
#define PIN3 TRISDbits.TRISD0
//trisd sets the corresponding pins to output
//portd sets a pin to read inputs

void NOP_1ms();

int main(void)
{
    BOARD_Init(); //Initializes Board functions
    int i;
    int NOP1 = 1000; //sets each NOP loop to approximately 1/2hz and 1/4hz
    int NOP2 = 2000;
    PIN5 = 0; //initializes the pins to be outputs
    PIN6 = 0;
    PIN3 = 0;
    TRISD |= 0x0200; //trisd bits makes PIN7 an input
    while (1) {
        for (i = 0; i < (NOP2); i++) {;;;
            if (i % NOP1 == 0) { //Every half of the 1/4hz loop, the 1/2 hz LED is flashed
                if (PIN7 > 0) {
                    PIN5 = !PIN5;
                } else {
                    PIN6 = !PIN6;
                }
            }
            NOP_1ms();
        }
        if (PIN7 > 0) { //after 1/4hz NOP loop, change the value of the output pin to flash LED
            PIN6 = !PIN6;
        } else {
            PIN5 = !PIN5;
        }
    }
    BOARD_End();
}

void NOP_1ms()
{
    int j;
    for (j = 0; j < 1162; j++) {
    }
}