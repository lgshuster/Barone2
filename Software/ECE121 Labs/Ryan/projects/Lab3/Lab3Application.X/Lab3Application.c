#include <proc/p32mx340f512h.h>
#include <sys/attribs.h>
#include <stdio.h>
#include <xc.h>
#include "BOARD.h"
#include "Protocol.h"
#include "MessageIDs.h"
#include "ADCFilter.h"
#include "NonVolatileMemory.h"

#define SW1 PORTDbits.RD8
#define SW2 PORTDbits.RD9
#define SW3 PORTDbits.RD10
#define SW4 PORTDbits.RD11

#define FILTER_LENGTH_BYTE 64
#define PEAK_TO_PEAK_SAMPLE_SIZE 128

void Switch_Init(void) {
    TRISDbits.TRISD8 = 1;
    TRISDbits.TRISD9 = 1;
    TRISDbits.TRISD10 = 1;
    TRISDbits.TRISD11 = 1;
}

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
    NonVolatileMemory_Init();
    Switch_Init();
    LEDS_INIT();
    unsigned int previousTime, LEDpreviousTime;
    unsigned char messageID;
    unsigned char payload[MAXPAYLOADLENGTH];
    unsigned char sendPayload[MAXPAYLOADLENGTH];
    unsigned char data[FILTER_LENGTH_BYTE];
    short payloadFilter[FILTERLENGTH];
    unsigned char currentSwitches;
    short channel = SW1 | (SW2 << 1);
    unsigned char null = 0x00;
    int absoluteValue;
    uint8_t LEDs = 0;
    short peakToPeakArray[PEAK_TO_PEAK_SAMPLE_SIZE];
    int peakToPeakMax, peakToPeakMin, peakToPeakValue;
    int peakToPeakIndex = 0;
    /*Freq. generator*/
    unsigned short CurFrequency;
    unsigned char freqState;
    int i, j;
    unsigned char message[MAXPAYLOADLENGTH];
    sprintf(message, "Lab3Application compiled on: %s %s\n", __DATE__, __TIME__);
    Protocol_SendDebugMessage(message);
    previousTime = FreeRunningTimer_GetMilliSeconds();
    LEDpreviousTime = FreeRunningTimer_GetMilliSeconds();
    unsigned char previousSwitches = (SW2 << 5) | (SW1 << 4) | SW3;
    channel = SW1 | (SW2 << 1);
    NonVolatileMemory_ReadPage((2 * channel + SW3) * FILTER_LENGTH_BYTE, FILTER_LENGTH_BYTE, data);
    for (j = 0; j < FILTERLENGTH; j++) { // Convert char to short filter
        payloadFilter[j] = data[2 * j] + (data[((2 * j) + 1)] << 8);
        payloadFilter[j] = Protocol_ShortEndednessConversion(payloadFilter[j]);
    }
    ADCFilter_SetWeights(channel, payloadFilter);
    while (1) {
        channel = SW1 | (SW2 << 1);
        get.rawValue = ADCFilter_RawReading(channel);
        get.filteredValue = ADCFilter_FilteredReading(channel);

        if (Protocol_IsMessageAvailable() == TRUE) {
            messageID = Protocol_ReadNextID();
            if (messageID == ID_ADC_FILTER_VALUES) { // Received Filter Values from Console
                Protocol_GetPayload(payload);
                NonVolatileMemory_WritePage((2 * channel + SW3) * FILTER_LENGTH_BYTE, FILTER_LENGTH_BYTE, payload);
                for (i = 0; i < FILTERLENGTH; i++) { // Store filter
                    payloadFilter[i] = payload[2 * i] + (payload[((2 * i) + 1)] << 8);
                    payloadFilter[i] = Protocol_ShortEndednessConversion(payloadFilter[i]);
                }
                ADCFilter_SetWeights(channel, payloadFilter);
                Protocol_SendMessage(2, ID_ADC_FILTER_VALUES_RESP, &channel);
                //                sprintf(message, "Page: %d\n",(2 * channel + SW3) * FILTER_LENGTH_BYTE);
                //                Protocol_SendDebugMessage(message);
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
        if ((FreeRunningTimer_GetMilliSeconds() - LEDpreviousTime) > 100) { // 10Hz
            /*Set LEDS*/
            if (SW4 == 0) { // LEDs display Absolute Value
                absoluteValue = abs(get.filteredValue);
                LEDs = 0;
                LEDs |= absoluteValue > 1;
                LEDs |= (absoluteValue > 100) << 1;
                LEDs |= (absoluteValue > 170) << 2;
                LEDs |= (absoluteValue > 250) << 3;
                LEDs |= (absoluteValue > 360) << 4;
                LEDs |= (absoluteValue > 370) << 5;
                LEDs |= (absoluteValue > 380) << 6;
                LEDs |= (absoluteValue > 384) << 7;
                LEDS_SET(LEDs);
            } else if (SW4 == 1) { // LEDs display Peak To Peak 
                peakToPeakMin = peakToPeakArray[0];
                peakToPeakMax = peakToPeakArray[0];
                for (i = 0; i < 127; i++) {
                    if (peakToPeakArray[i] < peakToPeakMin) // Find minimum filtered value
                        peakToPeakMin = peakToPeakArray[i];
                    if (peakToPeakArray[i] > peakToPeakMax) // Find maximium filtered value
                        peakToPeakMax = peakToPeakArray[i];
                }
                peakToPeakValue = peakToPeakMax - peakToPeakMin;
                LEDs = 0;
                LEDs |= peakToPeakValue > 50;
                LEDs |= (peakToPeakValue > 150) << 1;
                LEDs |= (peakToPeakValue > 200) << 2;
                LEDs |= (peakToPeakValue > 250) << 3;
                LEDs |= (peakToPeakValue > 300) << 4;
                LEDs |= (peakToPeakValue > 350) << 5;
                LEDs |= (peakToPeakValue > 400) << 6;
                LEDs |= (peakToPeakValue > 450) << 7;
                LEDS_SET(LEDs);
            }
            LEDpreviousTime = FreeRunningTimer_GetMilliSeconds();
        }
        /*ADC Reading*/
        if ((FreeRunningTimer_GetMilliSeconds() - previousTime) > 10) { // 100Hz ADC reading
            get.rawValue = Protocol_ShortEndednessConversion(get.rawValue);
            get.filteredValue = Protocol_ShortEndednessConversion(get.filteredValue);
            Protocol_SendMessage(4, ID_ADC_READING, &get.ADCValues);
            peakToPeakArray[peakToPeakIndex++] = ADCFilter_FilteredReading(channel); // Store filtered readings in peakToPeak array
            if (peakToPeakIndex > (PEAK_TO_PEAK_SAMPLE_SIZE - 1))
                peakToPeakIndex = 0;
            /*Check switches*/
            currentSwitches = (SW2 << 5) | (SW1 << 4) | SW3;
            if (currentSwitches != previousSwitches) {
                Protocol_SendMessage(1, ID_LAB3_CHANNEL_FILTER, &currentSwitches);
                NonVolatileMemory_ReadPage((2 * channel + SW3) * FILTER_LENGTH_BYTE, FILTER_LENGTH_BYTE, data);
                for (i = 0; i < FILTERLENGTH; i++) { // Convert char to short filter
                    payloadFilter[i] = data[2 * i] + (data[((2 * i) + 1)] << 8);
                    payloadFilter[i] = Protocol_ShortEndednessConversion(payloadFilter[i]);
                }
                ADCFilter_SetWeights(channel, payloadFilter);
                previousSwitches = (SW2 << 5) | (SW1 << 4) | SW3;
            }
            previousTime = FreeRunningTimer_GetMilliSeconds();
        }
    }
    BOARD_End();
}