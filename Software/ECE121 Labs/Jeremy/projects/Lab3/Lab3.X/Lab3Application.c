#include "Board.h"
#include "Protocol.h"
#include "FreeRunningTimer.h"
#include "MessageIDs.h"
#include "NonVolatileMemory.h"
#include "ADCFilter.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <GenericTypeDefs.h>
#include <xc.h>
#include <sys/attribs.h>


#define SW1 PORTDbits.RD8 //switches as defines
#define SW2 PORTDbits.RD9
#define SW3 PORTDbits.RD10
#define SW4 PORTDbits.RD11

#define FILTERPAGE 365 //pages 365-372 used to store/write filter values
#define PAGELENGTH 64

void NOP1ms(void); //function for waiting between page reads at the beginning

void NOP1ms(void)
{ //nops for 5ms
    int i;
    for (i = 0; i < 20000; i++) {
    }
    return;
}

int main(void)
{
    BOARD_Init();
    Protocol_Init(); //initializes modules
    LEDS_INIT();
    NonVolatileMemory_Init();
    ADCFilter_Init();
    FreeRunningTimer_Init();

    FrequencyGenerator_Init(); //frequency generator code
    unsigned short CurFrequency;
    unsigned char freqState;

    TRISDbits.TRISD8 = 1; //initializes swtiches to be inputs
    TRISDbits.TRISD9 = 1;
    TRISDbits.TRISD10 = 1;
    TRISDbits.TRISD11 = 1;

    char testMessage[MAXPAYLOADLENGTH];
    sprintf(testMessage, "Lab 3 Application Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(testMessage); //sends debug  message

    short channel = SW1 + 2 * SW2; //variables for channel and filter to use
    short pass = SW3;
    short current = (channel << 4) | pass; //value 0-8, shows which filter is in use

    short raw; //raw and filtered values, as well as max/min for LEDs
    short filter;
    short max;
    short min;

    unsigned char payload[MAXPAYLOADLENGTH]; //char array for sending/receiving payloads
    unsigned char pagedata[64]; //data array to store page read data
    short weights[8][FILTERLENGTH]; //will store 8 arrays for stored filters from page reads
    
    int i = 0; //counting variables
    int j = 0;
    int success; //success of page reads/writes
    
    for (i = 0; i < 8; i++) { //reads 8 page addresses that hold filter values
        success = NonVolatileMemory_ReadPage(FILTERPAGE + i, PAGELENGTH, pagedata);
        if (success == 0) { //stop condition if error happened while reading
            I2C1CONbits.PEN = 1;
            while (I2C1CONbits.PEN) {
            }
        }
        for (j = 0; j < FILTERLENGTH; j++) { //puts each of 8 page reads into short array of weights
            weights[i][j] = pagedata[2 * j] + (pagedata[2 * j + 1] << 8);
            weights[i][j] = Protocol_ShortEndednessConversion(weights[i][j]);
        }
        Protocol_SendMessage(PAGELENGTH, ID_NVM_READ_PAGE_RESP, &weights[i]); //shows each filter read 0-8 from memory at start
        NOP1ms();
    }
    ADCFilter_SetWeights(channel, weights[2 * channel + pass]); //loads current filter current channel
    //    Protocol_SendMessage(PAGELENGTH, ID_NVM_READ_PAGE_RESP, &weights[2 * channel + pass]);

    int target = 10; //every 10ms, while loop
    while (1) {
        if (FreeRunningTimer_GetMilliSeconds() >= target) {
            raw = ADCFilter_RawReading(channel); //returns the raw and filtered ADC readings
            filter = ADCFilter_FilteredReading(channel);
            payload[0] = (raw & 0xFF00) >> 8;
            payload[1] = raw & 0x00FF;
            payload[2] = (filter & 0xFF00) >> 8;
            payload[3] = filter & 0x00FF;
            Protocol_SendMessage(4, ID_ADC_READING, &payload);
            if (SW4) { //absolute filtered reading on LEDS
                if (filter < 0) {
                    LEDS_SET(filter * -1);
                } else {
                    LEDS_SET(filter);
                }
            } else { //Peak to peak reading for LEDS
                if (filter > max){
                    max = filter;
                }
                if (filter < min){
                    min = filter;
                }
                LEDS_SET(max - min);
            }
            target += 10;
            channel = SW1 + 2 * SW2; //modifies channel and pass filter
            pass = SW3;
            if (((channel << 4) | pass) != current) { //if a change in channel or filter, send message
                payload[0] = (channel << 4) | pass;
                current = (channel << 4) | pass;
                ADCFilter_SetWeights(channel, weights[2 * channel + pass]); //loads new filter
                max = 0; //resets peak to peak reading
                min = 0;
                Protocol_SendMessage(1, ID_LAB3_CHANNEL_FILTER, &payload);
            }
        }
        if (Protocol_IsMessageAvailable()) {
            if (Protocol_ReadNextID() == ID_ADC_FILTER_VALUES) {
                Protocol_GetPayload(&payload); //gets payload of filter values
                success = NonVolatileMemory_WritePage(FILTERPAGE + (2 * channel + pass), PAGELENGTH, payload); //writes data at specified address
                if (success == 0) {
                    I2C1CONbits.PEN = 1;
                    while (I2C1CONbits.PEN) { //stop condition if error happened while writing
                    }
                }
                for (i = 0; i < FILTERLENGTH; i++) {
                    weights[2 * channel + pass][i] = payload[2 * i] + (payload[2 * i + 1] << 8); //makes weights into the filter given
                    weights[2 * channel + pass][i] = Protocol_ShortEndednessConversion(weights[2 * channel + pass][i]);
                }
                ADCFilter_SetWeights(channel, weights[2 * channel + pass]); //sets recently gotten filter to current channel
                max = 0; //resets peak to peak reading
                min = 0;
                Protocol_SendMessage(1, ID_ADC_FILTER_VALUES_RESP, &channel);
            }
            //frequency generator messages
            if (Protocol_ReadNextID() == ID_LAB3_SET_FREQUENCY) {
                max = 0;
                min = 0;
                Protocol_GetPayload(&CurFrequency);
                CurFrequency = Protocol_ShortEndednessConversion(CurFrequency);
                FrequencyGenerator_SetFrequency(CurFrequency);
            }
            if (Protocol_ReadNextID() == ID_LAB3_FREQUENCY_ONOFF) {
                max = 0;
                min = 0;
                Protocol_GetPayload(&freqState);
                if (freqState) {
                    FrequencyGenerator_On();
                } else {
                    FrequencyGenerator_Off();
                }
            }
        }
    }
}
