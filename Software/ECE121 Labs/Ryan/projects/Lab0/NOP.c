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

void NOP_delay(float sudo_s){
    int s = (int) (sudo_s * 1000000);
    int i;
    for ( i = 0; i < s; i++){
        asm("nop");     //assembly command for non-operation
    }
}
/* *****************************************************************************
 End of File
 */
