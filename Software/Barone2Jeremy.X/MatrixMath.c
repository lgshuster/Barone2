#include "MatrixMath.h"
#include "BOARD.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <GenericTypeDefs.h>
#include <xc.h>
#include <sys/attribs.h>
#include <math.h>
#include <inttypes.h>

#define DOUBLE_LENGTH 8
#define MAX_MATRIX_SIZE 24

int Matrix_Multiply(void *m1, int x1, int y1, void *m2, int x2, int y2, void *m3, int *x3, int *y3)
{
    int M1xLength = 0, M2yLength = 0;
    if ((x1 != y2) || (x2 != y1)) {
        return (ERROR);
    }
    double *matrix1 = (double *) m1; //turns void pointers back into double array pointers
    double *matrix2 = (double *) m2; //only 1 dimensional to simplify turning back into 2D later
    double *matrix3 = (double *) m3;
    int i, j, k;
    for (i = 0; i < (y1 * x2); i++) { //puts new matrix values into 1D array for matrix 3
        matrix3[i] = 0;
        for (j = 0; j < y2; j++) {
            matrix3[i] += matrix1[j + ((i / y1) * x1)] * matrix2[j * x2 + (i % x2)];
            //            printf("%f %f %f\n", matrix3[i], matrix1[j + ((i / y1) * x1)], matrix2[j * x2 + (i % x2)]);
        }
    }
    *x3 = y1; //puts boundaries of new matrix 3 into address of variable passed
    *y3 = x2;
    return (SUCCESS);
}

#define MATRIX_MULTIPLY_TEST
#ifdef MATRIX_MULTIPLY_TEST

int main(void)
{
    BOARD_Init();
    double matrix1[2][3] = {//defines 1st matrix, edit for testing
        {0, 1, 2},
        {3, 4, 5}
    };
    int columns1 = (sizeof (matrix1[0])) / DOUBLE_LENGTH; //finds size of matrix
    int rows1 = (sizeof (matrix1)) / columns1 / DOUBLE_LENGTH;

    double matrix2[3][2] = {//defines 2nd matrix, edit for testing
        {9, 10},
        {12, 13},
        {15, 16}
    };
    int columns2 = (sizeof (matrix2[0])) / DOUBLE_LENGTH; //finds size of 2nd matrix
    int rows2 = (sizeof (matrix2)) / columns2 / DOUBLE_LENGTH;

    //    printf("%d %d\n%d %d\n", columns1, rows1, columns2, rows2);

    double matrix3temp[MAX_MATRIX_SIZE * MAX_MATRIX_SIZE]; //defines third matrix, 1D for sizing
    int columns3; //empty variables to input size of third matrix
    int rows3;
    
    int success = Matrix_Multiply(&matrix1, columns1, rows1, &matrix2, columns2, rows2, &matrix3temp, &columns3, &rows3);
    //    printf("\n%d", success);
    
    double matrix3[rows3][columns3];
    int i, j;
    for (i = 0; i < rows3; i++) { //puts 1D values for matrix 3 into new 2D matrix 3
        for (j = 0; j < columns3; j++) {
            matrix3[i][j] = matrix3temp[i * rows3 + j];
            printf("%f ", matrix3[i][j]);
        }
        printf("\n");
    }
    //in the end values are put into matrix3, whose size is dependent on matrix 1 and 2
    BOARD_End();
    return (0);
}
#endif