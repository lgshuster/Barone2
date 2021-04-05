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
    double *matrix2 = (double *) m2;
    double *matrix3 = (double *) m3;
    int i, j, k;
    for (i; i < (y1 * x2); i++) {
        matrix3[i] = 0;
        for (j; j < y1; j++) {
            matrix3[i] += matrix1[j + i / y2] * matrix2[j * y2 + (i % x1)];
        }
    }
    return (SUCCESS);
}

#define MM_TEST
#ifdef MM_TEST

int main(void)
{
    BOARD_Init();
    double matrix1[3][2] = {//defines 1st matrix
        {0, 1},
        {2, 3}
    };
    int columns1 = (sizeof (matrix1[0])) / DOUBLE_LENGTH; //finds size of matrix
    int rows1 = (sizeof (matrix1)) / columns1 / DOUBLE_LENGTH;

    double matrix2[2][3] = {//defines 2nd matrix
        {4, 5},
        {6, 7}
    };
    int columns2 = (sizeof (matrix2[0])) / DOUBLE_LENGTH; //finds size of 2nd matrix
    int rows2 = (sizeof (matrix2)) / columns2 / DOUBLE_LENGTH;

    printf("%d %d\n%d %d\n", columns1, rows1, columns2, rows2);

    double matrix3[MAX_MATRIX_SIZE * MAX_MATRIX_SIZE]; //defines third matrix, 1D for sizing
    int columns3; //empty variables to input size of third matrix
    int rows3;

    int success = Matrix_Multiply(&matrix1, columns1, rows1, &matrix2, columns2, rows2, &matrix3, &columns3, &rows3);
    printf("\n%d", success);
    BOARD_End();
    return (0);
}
#endif