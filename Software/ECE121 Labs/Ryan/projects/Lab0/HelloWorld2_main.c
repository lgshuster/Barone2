//Section 2.3 ImprovedHelloWorld!
#include "BOARD.h"
#include "LED.h"
#include "buttons.h"
#include "NOP.h"
#include <xc.h>
#include <stdio.h>

void main(void)
{
    initLEDS();
    initButtons();
    uint8_t LEDS = 0;
    int LEDEnd = 0;
    while (1){
        LEDS |= 0b10000000;
        if (getButtons() != 0){     // If any buttons are pressed, reset LEDS
            LEDS = 0;
            LEDEnd = 0;
        }
        else if ((LEDS == 0b11111111) && (LEDEnd == 0) )     // If all LEDS are on, flag for next loop to turn off all LEDS
            LEDEnd = 1;
        else if (LEDEnd == 1){      // Turn off LEDS when previous loop's LEDS are all on
            LEDEnd = 0;
            LEDS = 0;
        }
        setLEDS(LEDS);
        NOP_delay_5ms();
        LEDS = LEDS >> 1;
    }
}