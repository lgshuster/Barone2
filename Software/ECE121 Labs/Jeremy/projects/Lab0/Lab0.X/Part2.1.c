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
#include <stdint.h>
#include <stdlib.h>
#include <GenericTypeDefs.h>
#include <xc.h>
#include <sys/attribs.h>

#include "BOARD.h"
#include "LEDS.h"
#include "Buttons.h"

int main(void)
{
    BOARD_Init(); //Initializes Board and LED functions
    LEDS_Init();
    while (1) {
        LEDS_Set(Buttons_Get()); //sets the value of the LEDS to the values of the buttons pressed in an infinite loop
    }                           //Every one of the 4 buttons lights up 2 LEDs individually
    BOARD_End();
}