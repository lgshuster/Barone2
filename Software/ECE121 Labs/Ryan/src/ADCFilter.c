#include <proc/p32mx340f512h.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <xc.h>
#include "BOARD.h"
#include "Protocol.h"
#include "MessageIDs.h"
#include "ADCFilter.h"

//#define TEST_HARNESS_ADC_FILTER
static short ADCfilter[4] [FILTERLENGTH];
static short ADCvalues[4] [FILTERLENGTH];
static short interruptIndex;

int ADCFilter_Init(void) {
    /*ADC 1 control registers*/
    AD1CON1 = 0;
    AD1CON1bits.FORM = 0; // Ints 16 bits
    AD1CON1bits.SSRC = 0b111; // Auto convert
    AD1CON1bits.ASAM = 1; // Auto sample
    /*ADC 2 control registers*/
    AD1CON2 = 0;
    AD1CON2bits.SMPI = 3; // Interrupt at completion of 4th sample
    AD1CON2bits.CSCNA = 1; // Scan inputs/ scan mode
    /*ADC 3 control register*/
    AD1CON3 = 0;
    AD1CON3bits.SAMC = 16; // Set sample time to 16 T_AD
    AD1CON3bits.ADCS = 173; // Set T_AD = 348 T_PB
    /*Input Scan Select*/
    AD1CSSLbits.CSSL2 = 1; // Pin A0, set as 1 for scan
    AD1CSSLbits.CSSL4 = 1; // Pin A1
    AD1CSSLbits.CSSL8 = 1; // Pin A2
    AD1CSSLbits.CSSL10 = 1; // Pin A3
    /*ADC Port configuration register*/
    AD1PCFGbits.PCFG2 = 0; // Set as 0 for analog input
    AD1PCFGbits.PCFG4 = 0;
    AD1PCFGbits.PCFG8 = 0;
    AD1PCFGbits.PCFG10 = 0;
    /*Tri state register configuration*/
    TRISBbits.TRISB2 = 1; // Set as 0 for input pin
    TRISBbits.TRISB4 = 1;
    TRISBbits.TRISB8 = 1;
    TRISBbits.TRISB10 = 1;
    /*ADC1 Interrupt*/
    IEC1bits.AD1IE = 0;
    IFS1bits.AD1IF = 0;
    IPC6bits.AD1IP = 1; // Priority level 1
    /*Enable Registers and Interrupt*/
    AD1CON1bits.ON = 1;
    IEC1bits.AD1IE = 1;
    /*Set filters as 1*/
    int i;
    int j;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < FILTERLENGTH; j++) {
            ADCfilter[i][j] = 0;
            ADCvalues[i][j] = 0;
        }
    }
    interruptIndex = 0;
    return SUCCESS;
}

short ADCFilter_RawReading(short pin) {
    return ((short) ADCvalues[pin][interruptIndex]);
}

short ADCFilter_FilteredReading(short pin) {
    short filteredValue = ADCFilter_ApplyFilter(ADCfilter[pin], ADCvalues[pin], interruptIndex);
    return ((short) filteredValue);
}

short ADCFilter_ApplyFilter(short filter[], short values[], short startIndex) {
    int sum;
    int i;
    short index = startIndex;
    for (i = 0; i < FILTERLENGTH; i++) {
        sum += filter[i] * values[index];
        index = (index - 1) % FILTERLENGTH;
    }
    sum = (sum >> 15) & 0xFFFF;
    unsigned char message[MAXPAYLOADLENGTH];
    return ((short) sum);
}

int ADCFilter_SetWeights(short pin, short weights[]) {
    int i;
    for (i = 0; i < FILTERLENGTH; i++) {
        ADCfilter[pin][i] = weights[i];
    }
    return SUCCESS;
}

/*Interrupt Handler: ADC*/
void __ISR(_ADC_VECTOR) ADCIntHandler(void) {
    IFS1bits.AD1IF = 0;
    ADCvalues[0] [interruptIndex] = ADC1BUF0;
    ADCvalues[1] [interruptIndex] = ADC1BUF1;
    ADCvalues[2] [interruptIndex] = ADC1BUF2;
    ADCvalues[3] [interruptIndex] = ADC1BUF3;
    interruptIndex = (interruptIndex + 1) % FILTERLENGTH;
}

#ifdef TEST_HARNESS_ADC_FILTER

union values {

    struct {
        short rawValue;
        short filteredValue;
    };
    unsigned char ADCValues[4];
};
union values get;

void main(void) {
    BOARD_Init();
    Protocol_Init();
    ADCFilter_Init();
    FreeRunningTimer_Init();
    FrequencyGenerator_Init();
    unsigned int previousTime;
    unsigned char messageID;
    unsigned char payload[MAXPAYLOADLENGTH];
    short payloadFilter[FILTERLENGTH];
    short channel;
    /*Freq. generator*/
    unsigned short CurFrequency;
    unsigned char freqState;
    int i;
    unsigned char message[MAXPAYLOADLENGTH];
    sprintf(message, "ADCFilter compiled on: %s %s\n", __DATE__, __TIME__);
    Protocol_SendDebugMessage(message);
    previousTime = FreeRunningTimer_GetMilliSeconds();
    while (1) {
        if (Protocol_IsMessageAvailable() == TRUE) {
            messageID = Protocol_ReadNextID();
            if (messageID == ID_ADC_SELECT_CHANNEL) { // Select channel
                Protocol_GetPayload(payload);
                channel = payload[0]; // Correct endianess
                Protocol_SendMessage(2, ID_ADC_SELECT_CHANNEL_RESP, &channel);
            } else if (messageID == ID_ADC_FILTER_VALUES) {
                Protocol_GetPayload(payloadFilter);
                for (i = 0; i < FILTERLENGTH; i++) { // Store filter
                    payloadFilter[i] = Protocol_ShortEndednessConversion(payloadFilter[i]);
                }
                ADCFilter_SetWeights(channel, payloadFilter);
                Protocol_SendMessage(2, ID_ADC_FILTER_VALUES_RESP, &channel);
            } else if (messageID == ID_LAB3_SET_FREQUENCY) { // Set frequency
                Protocol_GetPayload(&CurFrequency);
                CurFrequency = Protocol_ShortEndednessConversion(CurFrequency);
                FrequencyGenerator_SetFrequency(CurFrequency);
            } else if (messageID == ID_LAB3_FREQUENCY_ONOFF) { // ON/OFF frequency
                Protocol_GetPayload(&freqState);
                if (freqState) {
                    FrequencyGenerator_On();
                } else {
                    FrequencyGenerator_Off();
                }
            }
        }
        get.rawValue = ADCFilter_RawReading(channel);
        get.filteredValue = ADCFilter_FilteredReading(channel);
        if ((FreeRunningTimer_GetMilliSeconds() - previousTime) > 10) { // 100Hz ADC reading
            get.rawValue = Protocol_ShortEndednessConversion(get.rawValue);
            get.filteredValue = Protocol_ShortEndednessConversion(get.filteredValue);
            Protocol_SendMessage(4, ID_ADC_READING, &get.ADCValues);
            previousTime = FreeRunningTimer_GetMilliSeconds();
        }
    }
    BOARD_End();
}
#endif