#include "NOP.h"
#include <xc.h>

#define NOPS_FOR_5MS 5000      // 5ms based on testing LED timing on my phone stopwatch app
#define NOPS_FOR_500MS 500000    // 0.5s
#define NOPS_FOR_1S 1000000    // 1s
#define NOPS_FOR_5S 5000000    // 5s

void NOP_delay_5ms(void){
    int i;
    for ( i = 0; i < NOPS_FOR_5MS; i++){
        asm("nop");     //assembly command for non-operation
    }
}

void NOP_delay(int sudo_s){
    int i;
    for ( i = 0; i < sudo_s; i++){
        asm("nop");     //assembly command for non-operation
    }
}

/* *****************************************************************************
 End of File
 */
