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

static double height[3][MAX_MEASUREMENTS]; //array and indedx to store measurement values
//arrays 1 and 2 are for front sensors, row 3 is for bottom sensor
static int index_bottom;
static int index_front;

//inputs: Sensor reading from bottom ultrasonic
//returns: Average of last 10 readings

double Track_Height_Bottom(double measurement)
{
    height[2][index_bottom] = measurement; //puts new measurement into array
    index_bottom = (index_bottom + 1) % MAX_MEASUREMENTS; //increments index
    int i;
    double avg = 0;
    for (i = 0; i < MAX_MEASUREMENTS; i++) { //returns the average height measured recently
        avg += height[2][i];
    }
    return (avg / MAX_MEASUREMENTS);
}
//inputs: Sensor reading from first and second front facing ultrasonic
//returns: Average of last 10 readings from both sensors

double Track_Height_Front(double measurement1, double measurement2)
{
    height[0][index_front] = measurement1; //puts new measurement into array
    height[1][index_front] = measurement2; //puts new measurement into array
    index_front = (index_front + 1) % MAX_MEASUREMENTS; //increments index
    int i;
    double avg = 0;
    for (i = 0; i < MAX_MEASUREMENTS; i++) { //returns the average height measured recently
        avg += height[0][i] * 0.707; //divides by sqrt(2) for height since 45 degree angle)
        avg += height[1][i] * 0.707;
    }
    return (avg / MAX_MEASUREMENTS / 2);
}


//inputs: scalar double value to change entire array by
//returns: 1 when complete

int Update_Values(double scalar, int row)
{
    int i;
    for (i = 0; i < MAX_MEASUREMENTS; i++) { //multiplies each value in array by scalar
        height[row][i] = height[row][i] * scalar;
    }
    return (1);
}

//#define TRACK_HEIGHT_TEST
#ifdef TRACK_HEIGHT_TEST
#include "BOARD.h"

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
        printf("Bottom Avg: %f   Cur: %f\n", Track_Height_Bottom(rand_m), rand_m); //prints avg measurement
        printf("Front  Avg: %f   Cur: %f\n", Track_Height_Front(rand_m / .707, rand_m / .707), rand_m / .707);
    }
    BOARD_End();
    return (0);
}
#endif