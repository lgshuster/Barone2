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
#include "LEDS.h"
#include "Buttons.h"

void NOP_1ms();

int main(void)
{
    BOARD_Init(); //Initializes Board and LED functions
    LEDS_Init();
    int i;
    int NOPS = 250; //sets 250ms of delay before LEDs increment
    char LEDS = 0x00; //starts with no LEDs lit
    while (1) { 
        for (i = 0; i < (NOPS + 1); i++) { //every 250ms loop, LEDs increment
            NOP_1ms();
            if (i % 5 == 0) {
                while (Buttons_Get() > 0) { //checks every 5ms to reset the LEDs if a button is pressed
                    LEDS = 0;
                    LEDS_Set(0x00);
                }
            }
        }
        LEDS += 1;
        LEDS_Set(LEDS);
    }
    BOARD_End();
}

void NOP_1ms() //nop loop takes approximately 1ms
{
    int j;
    for (j = 0; j < 1162; j++) {
    }
}