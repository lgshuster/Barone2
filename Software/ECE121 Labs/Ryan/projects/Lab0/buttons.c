#include <xc.h>
#include <stdio.h>
#include <sys/attribs.h>
#include "buttons.h"

#define BTN1 PORTFbits.RF1
#define BTN2 PORTDbits.RD5
#define BTN3 PORTDbits.RD6
#define BTN4 PORTDbits.RD7

void initButtons(void) {
    TRISD |= 0x00E0;
    TRISF |= 0x0002;
}

uint8_t getButtons(void) {
    uint8_t button = 0b00000000;
    if (BTN1 == 1){
        button |= 0b00000011;
        printf("BTN1");
    }
    if (BTN2 == 1){
        button |= 0b00001100;
        printf("BTN2");
    }
    if (BTN3 == 1){
        button |= 0b00110000;
        printf("BTN3");
    }
    if (BTN4 == 1){
        button |= 0b11000000;
        printf("BTN4");
    }
    return (button);
}
/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************

/** 
  @Function
    int ExampleInterfaceFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Remarks
    Refer to the example_file.h interface header for function usage details.
 */


/* *****************************************************************************
 End of File
 */
