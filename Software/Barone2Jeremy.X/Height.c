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
static int index_front_left;
static int index_front_right;

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
//inputs: Sensor reading from front left facing ultrasonic
//returns: Average of last 10 readings from sensor

double Track_Height_Front_Left(double measurement)
{
    height[0][index_front_left] = measurement; //puts new measurement into array
    index_front_left = (index_front_left + 1) % MAX_MEASUREMENTS; //increments index
    int i;
    double avg = 0;
    for (i = 0; i < MAX_MEASUREMENTS; i++) { //returns the average height measured recently
        avg += height[0][i] * 0.707; //divides by sqrt(2) for height since 45 degree angle)
    }
    return (avg / MAX_MEASUREMENTS);
}

//inputs: Sensor reading from front right facing ultrasonic sensor
//returns: Average of last 10 readings from sensor

double Track_Height_Front_Right(double measurement)
{
    height[1][index_front_right] = measurement; //puts new measurement into array
    index_front_right = (index_front_right + 1) % MAX_MEASUREMENTS; //increments index
    int i;
    double avg = 0;
    for (i = 0; i < MAX_MEASUREMENTS; i++) { //returns the average height measured recently
        avg += height[1][i] * 0.707; //divides by sqrt(2) for height since 45 degree angle)
    }
    return (avg / MAX_MEASUREMENTS);
}

double Actual_Height(double bottom, double right, double left)
{
    if ((bottom < left) && (bottom < right)) {
        return bottom;
    } else if (left < right) {
        return left;
    } else {
        return right;
    }
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

#define TRACK_HEIGHT_TEST
#ifdef TRACK_HEIGHT_TEST
#include "BOARD.h"

int main(void)
{
    BOARD_Init();
    int i;
    srand(time(NULL));
    double rand1, rand2, rand3;
    double height_bottom, height_front_right, height_front_left, actual_height;
    while (1) {
        for (i = 0; i < 200000; i++) {
        }
        rand1 = rand() % 400; //puts random measurement into array
        rand2 = rand() % 400;
        rand3 = rand() % 400;
        height_bottom = Track_Height_Bottom(rand1);
        height_front_right = Track_Height_Front_Right(rand2);
        height_front_left = Track_Height_Front_Left(rand3);
        actual_height = Actual_Height(height_bottom, height_front_right, height_front_left);

        printf("Bottom Avg: %f   Cur: %f\n", height_bottom, rand1); //prints avg measurement
        printf("FrontR Avg: %f   Cur: %f\n", height_front_right, rand2 / .707);
        printf("FrontL Avg: %f   Cur: %f\n", height_front_left, rand3 / .707);
        printf("Actual: %f\n", actual_height);

    }
    BOARD_End();
    return (0);
}
#endif