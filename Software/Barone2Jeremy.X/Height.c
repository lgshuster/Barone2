#include "BOARD.h"
#include "Height.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <GenericTypeDefs.h>
#include <xc.h>
#include <sys/attribs.h>
#include <math.h>
#include <inttypes.h>

#define MAX_MEASUREMENTS 10 //changes size of array

static double measurements[MAX_MEASUREMENTS]; //array and indedx to store measurement values
static int index_m;

double Track_Height(double measurement)
{
    measurements[index_m] = measurement; //puts new measurement into array
    index_m = (index_m + 1) % MAX_MEASUREMENTS; //increments index
    int i;
    double max = 0;
    for (i = 0; i < MAX_MEASUREMENTS; i++) //returns the maximum height measured recently
        if (measurements[i] > max) {
            max = measurements[i];
        }
    return (max);
}

int Update_Values(double scalar)
{
    int i;
    for (i = 0; i < MAX_MEASUREMENTS; i++) { //multiplies each value in array by scalar
        measurements[i] = measurements[i] * scalar;
    }
    return (SUCCESS);
}

//#define TRACK_HEIGHT_TEST
#ifdef TRACK_HEIGHT_TEST

int main(void)
{
    BOARD_Init();
    int i;
    srand(time(NULL));
    double rand_m;
    while (1) {
        for (i = 0; i < 200000; i++) {
        }
        rand_m = rand() % 400; //puts random measurement into array
        printf("Max: %f   Cur: %f\n", Track_Height(rand_m), rand_m); //prints max measurement
    }
    BOARD_End();
    return (0);
}
#endif