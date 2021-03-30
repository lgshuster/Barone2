/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
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
    int NOPS = 2000; //2000 loops of the 1ms NOP function
    while (1) { 
        for (i = 0; i < NOPS; i++) {//Uses NOP loop to wait 2s before setting any LEDS in response to the buttons
            NOP_1ms();
        }
        LEDS_Set(Buttons_Get());
    }
    BOARD_End();
}

void NOP_1ms()
{
    int j; //NOP loop counting up to 1162, which takes approximately 1ms
    for (j = 0; j < 1162; j++) {
    }
}