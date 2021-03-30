// Section 2.1 Simple Digital I/O
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
    while (1){
        setLEDS(getButtons());
    }
}