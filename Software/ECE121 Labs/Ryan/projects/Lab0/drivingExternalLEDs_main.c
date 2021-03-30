// Section 3.1 Driving External LEDs
#include <xc.h>
#include "BOARD.h"
#include "NOP.h"
#include "LED.h"

#define PIN5 LATDbits.LATD1
#define PIN6 LATDbits.LATD2
#define PIN7 PORTDbits.RD9
/* PIN 5 is red LED
 * PIN 6 is green LED
 * PIN 7 is button
 */

float red_delay, green_delay;   //delay for how long in seconds

void getButton(void){
    if (PIN7 == 1){     // Red LED 5Hz, green LED 10Hz
        PIN5 = 0;
        PIN6 = 0;   
        NOP_delay(0.05);
        PIN5 = 1;
        NOP_delay(0.05);
        PIN5 = 0;
        PIN6 = 1;
        NOP_delay(0.05);
        PIN5 = 1;
        NOP_delay(0.05);
    }
    
    else if (PIN7 == 0){    // Red LED 10Hz, green LED 5Hz
        PIN5 = 0;   
        PIN6 = 0;   
        NOP_delay(0.05);
        PIN6 = 1;
        NOP_delay(0.05);
        PIN6 = 0;
        PIN5 = 1;
        NOP_delay(0.05);
        PIN6 = 1;
        NOP_delay(0.05);
    }
    
}
void main(void){
    initLEDS();
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD2 = 0;
    while (1){
        getButton();
    }
}