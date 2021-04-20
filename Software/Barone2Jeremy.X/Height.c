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

static double height[4][MAX_MEASUREMENTS]; //array and indedx to store measurement values
//arrays 1 and 2 are for front sensors, row 3 is for bottom sensor
static int index1;

//inputs: Sensor readings from ultrasonics(m1 = bottom, m2 = front left, m3 = front right, m4 = front)
//returns: Actual Reading

double Track_Height(double m1, double m2, double m3, double m4)
{
    height[0][index1] = m1; //puts new measurements from each sensor into array
    height[1][index1] = m2;
    height[2][index1] = m3;
    height[3][index1] = m4;
    index1 = (index1 + 1) % MAX_MEASUREMENTS; //increments index
    int i;
    double avg1, avg2, avg3, avg4;
    for (i = 0; i < MAX_MEASUREMENTS; i++) { //avg height of every row is calcualted
        avg1 += height[0][i];
        avg2 += height[1][i];
        avg3 += height[2][i];
        avg4 += height[3][i];
    }
    avg1 = avg1 / MAX_MEASUREMENTS;
    avg2 = avg2 / MAX_MEASUREMENTS;
    avg3 = avg3 / MAX_MEASUREMENTS;
    avg4 = avg4 / MAX_MEASUREMENTS;
    double h = Actual_Height(avg1, avg2, avg3, avg4);
    return (h);
}
//inputs: Avg reading from each ultrasonic(m1 = bottom, m2 = front left, m3 = front right, m4 = front)
//returns: Actual Height based on trigonometry

double Actual_Height(double m1, double m2, double m3, double m4)
{
    if ((m1 < (m2 * .707)) && (m1 < (m3 * .707))) {
        return m1; //if bottom sensor height is lower than front, going at a decline
    } 
    if (m4 < m2 || m4 < m3){
        return m1; //if front sensor is lower than the front angled sensors, obstacle in the way
    }
    if (m2 < m3) { //lastly, if going at an incline, front angled sensors should be chosen based on least height
        return (m2 * .707);
    } else {
        return (m3 * .707);
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

//#define TRACK_HEIGHT_TEST
#ifdef TRACK_HEIGHT_TEST
#include "BOARD.h"

int main(void)
{
    BOARD_Init();
    int i;
    srand(time(NULL));
    double rand1, rand2, rand3, rand4;
    double actual_height;
    while (1) {
        for (i = 0; i < 200000; i++) {
        }
        rand1 = rand() % 400; //puts random measurement into array
        rand2 = rand() % 400;
        rand3 = rand() % 400;
        rand4 = rand() % 400;
        actual_height = Actual_Height(rand1, rand2, rand3, rand4);

        printf("Bottom Cur: %f\n", rand1); //prints avg measurement
        printf("FrontL Cur: %f\n", rand2 / .707);
        printf("FrontR Cur: %f\n", rand3 / .707);
        printf("Front  Cur: %f\n", rand4);
        printf("Actual: %f\n", actual_height);

    }
    BOARD_End();
    return (0);
}
#endif