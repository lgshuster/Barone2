#include "Board.h"
#include "Protocol.h"
#include "MessageIDs.h"
#include "FrequencyGenerator.h"
/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <GenericTypeDefs.h>
#include <xc.h>
#include <sys/attribs.h>

#define FILTERLENGTH 32

static short values[4][FILTERLENGTH]; //2d arrays for values and filters
static short filters[4][FILTERLENGTH];
static unsigned short index;
/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

/**
 * @Function ADCFilter_Init(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief initializes ADC system along with naive filters */
int ADCFilter_Init(void)
{
    int i; //sets initial filters to 1
    int j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < FILTERLENGTH; j++) {
            filters[i][j] = 1;
        }
    }

    AD1CON1 = 0; //resets control registers
    AD1CON2 = 0;
    AD1CON3 = 0;
    
    AD1PCFG = 0xFAEB; //AN2,4,8,10 pins on A0,1,2,3 channels added to input scanner and ADC
    AD1CSSL = 0x0514;
    
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB4 = 1;
    TRISBbits.TRISB8 = 1;
    TRISBbits.TRISB10 = 1;

    AD1CON1bits.ASAM = 1; //auto sampling
    AD1CON1bits.FORM = 0b000; //16 bits unsigned int
    AD1CON1bits.SSRC = 0b111; //auto convert

    AD1CON2bits.VCFG = 0b100; //internal voltage reference AVdd+ and AVss-
    AD1CON2bits.CSCNA = 1; //scan mode
    AD1CON2bits.SMPI = 0b0011; //interrupts after each 4 measurements
    AD1CON2bits.BUFM = 0; //16 bit buffer

    AD1CON3bits.ADRC = 0; //uses board clock
    AD1CON3bits.ADCS = 0b10101101; //(173 + 1) * 2Tpb = 348TAD
    AD1CON3bits.SAMC = 0b10000; //16TAD sample time

    IFS1bits.AD1IF = 0; //enables interrupts for ADC
    IPC6bits.AD1IP = 3; //enables interrupt priority and subpriority
    IEC1bits.AD1IE = 1;
    //
    AD1CON1bits.ON = 1; //turns on ADC
    return (1);
}

/**
 * @Function ADCFilter_RawReading(short pin)
 * @param pin, which channel to return
 * @return un-filtered AD Value
 * @brief returns current reading for desired channel */
short ADCFilter_RawReading(short pin)
{
    return (values[pin][index]); //returns reading based on channel and index
}

/**
 * @Function ADCFilter_FilteredReading(short pin)
 * @param pin, which channel to return
 * @return Filtered AD Value
 * @brief returns filtered signal using weights loaded for that channel */
short ADCFilter_FilteredReading(short pin)
{
    int sum = 0;
    unsigned int i;
    unsigned short indextemp = index; //sets temporary index and value array variables
    short valuestemp[FILTERLENGTH];
    for(i=0; i < FILTERLENGTH; i++){
        valuestemp[i] = values[pin][i]; 
    }
    for (i = 0; i < FILTERLENGTH; i++) {
        sum += valuestemp[(indextemp - i) % FILTERLENGTH] * filters[pin][i]; //applies FIR filtering
    }
    sum = sum >> 15;
    short sum1 = sum & 0xFFFF; //gets result back in a short
    return (sum1);
}

/**
 * @Function short ADCFilter_ApplyFilter(short filter[], short values[], short startIndex)
 * @param filter, pointer to filter weights
 * @param values, pointer to circular buffer of values
 * @param startIndex, location of first sample so filter can be applied correctly
 * @return Filtered and Scaled Value
 * @brief returns final signal given the input arguments
 * @warning returns a short but internally calculated value should be an int */
short ADCFilter_ApplyFilter(short filter[], short values[], short startIndex); //NOT UTILIZED
//takes a pointer to the filter array, a
//pointer to the data array, and an index indicating where the start point of the data array is.

/**
 * @Function ADCFilter_SetWeights(short pin, short weights[])
 * @param pin, which channel to return
 * @param pin, array of shorts to load into the filter for the channel
 * @return SUCCESS or ERROR
 * @brief loads new filter weights for selected channel */
int ADCFilter_SetWeights(short pin, short weights[])
{
    int i;
    for (i = 0; i < FILTERLENGTH; i++) { //puts weight input into filter array of channel
        filters[pin][i] = weights[i];
    }
    return (SUCCESS);
}
//This function indicates which pin (0-3) and a pointer to an array of weights
//(signed shorts)

void __ISR(_ADC_VECTOR) ADCIntHandler(void)
{
    values[0][index] = ADC1BUF0;//gets 4 readings at a time for AD2,4,8, and 10
    values[1][index] = ADC1BUF1;
    values[2][index] = ADC1BUF2;
    values[3][index] = ADC1BUF3;
    index = (index + 1) % FILTERLENGTH; //increments index
    IFS1bits.AD1IF = 0;
    return;
}

//#define ADC_TEST
#ifdef ADC_TEST

int main(void)
{
    BOARD_Init();
    Protocol_Init(); //initializes board and UART1 protocol
    LEDS_INIT();
    ADCFilter_Init();

    char testMessage[MAXPAYLOADLENGTH];
    sprintf(testMessage, "ADC Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(testMessage); //sends debug  message

    FreeRunningTimer_Init();
    int target = 10; //100hz

    unsigned short filter; //filtered reading variable
    unsigned short raw; //raw reading variable
    unsigned char payload[MAXPAYLOADLENGTH]; //payload to be received
    unsigned short channel = 0;
    short weights[FILTERLENGTH];
    void *ptr; //void pointer to payload
    int i;

    FrequencyGenerator_Init(); //frequency generator init
    unsigned short CurFrequency;
    unsigned char freqState;

    while (1) {
        if (FreeRunningTimer_GetMilliSeconds() >= target) {
            target += 10; //repeats every 100ms
            raw = ADCFilter_RawReading(channel); //returns the raw and filtered readings
            filter = ADCFilter_FilteredReading(channel);
            payload[0] = (raw & 0xFF00) >> 8;
            payload[1] = raw & 0x00FF;
            payload[2] = (filter & 0xFF00) >> 8;
            payload[3] = filter & 0x00FF;
            ptr = &payload;
            Protocol_SendMessage(4, ID_ADC_READING, ptr); //sends message to interface
        }
        if (Protocol_IsMessageAvailable()) {
            if (Protocol_ReadNextID() == ID_ADC_SELECT_CHANNEL) {
                ptr = &payload;
                Protocol_GetPayload(ptr);
                channel = payload[0]; //sets channel variable to payload
                ptr = &channel;
                Protocol_SendMessage(1, ID_ADC_SELECT_CHANNEL_RESP, ptr);
            }
            if (Protocol_ReadNextID() == ID_ADC_FILTER_VALUES) {
                ptr = &payload;
                Protocol_GetPayload(ptr);
                for (i = 0; i < FILTERLENGTH; i++) {
                    weights[i] = payload[2 * i] + (payload[2 * i + 1] << 8); //makes weights into the filter given a channel
                    weights[i] = Protocol_ShortEndednessConversion(weights[i]); //deals with endianness
                }
                ADCFilter_SetWeights(channel, weights);
                Protocol_SendMessage(1, ID_ADC_FILTER_VALUES_RESP, &channel);
            }
            //given frequency library
            if (Protocol_ReadNextID() == ID_LAB3_SET_FREQUENCY) {
                Protocol_GetPayload(&CurFrequency);
                CurFrequency = Protocol_ShortEndednessConversion(CurFrequency);
                FrequencyGenerator_SetFrequency(CurFrequency);
            }
            if (Protocol_ReadNextID() == ID_LAB3_FREQUENCY_ONOFF) {
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
#endif