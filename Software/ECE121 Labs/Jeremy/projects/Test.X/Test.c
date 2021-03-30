#include "BOARD.h"
#include "Protocol.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <GenericTypeDefs.h>
#include <xc.h>
#include <sys/attribs.h>

int main(void)
{
    BOARD_Init();
    Protocol_Init(); //initializes board and UART1 protocol
    LEDS_INIT();

    char testMessage[MAXPAYLOADLENGTH];
    sprintf(testMessage, "Protocol Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(testMessage);

    while (1) { //main loop
    }
}
