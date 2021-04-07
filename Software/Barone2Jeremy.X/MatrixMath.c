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

//inputs: 2 2D array addresses with their columns(x) and rows(y), matrix 3's is 1D and size variable addresses are passed by reference
//returns: SUCCESS Or ERROR if arrays do not match dimensions, 
//columns(x) and rows(y) are passed by reference and are set when the function is done,
//need to manually put data from pointer *m3 into 2D array after
int Matrix_Multiply(void *m1, int x1, int y1, void *m2, int x2, int y2, void *m3, int *x3, int *y3)
{
    if ((x1 != y2) || (x2 != y1)) {
        return (ERROR);
    }
    double *matrix1 = (double *) m1; //turns void pointers back into double array pointers
    double *matrix2 = (double *) m2; //only 1 dimensional to simplify turning back into 2D later
    double *matrix3 = (double *) m3;
    int i, j;
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

//inputs: 2D array address with columns(x) and rows(y), scalar double value s to multiply by
//returns: SUCCESS
int Matrix_ScalarMultiply(void *m, int x, int y, double s)
{
    double *matrix = (double *) m;
    int i;
    for (i = 0; i < (y * x); i++) {// multiplies each value in array by scalar value
        matrix[i] = matrix[i] * s; //does this in 1 dimension, but it loops through the rows automatically
    }
    return (SUCCESS);
}

//inputs: 2D array address with columns(x) and rows(y), matrix 2's address is 1D
//returns: SUCCESS Or ERROR if array is not square
//need to manually put data from pointer *matrix2 into 2D array after
int Matrix_Inverse(void *matrix1, int x, int y, void *matrix2)
{
    
}

//inputs: 2 2D array addresses with their columns(x) and rows(y), matrix 3's is 1D and size variable addresses are passed by reference
//returns: SUCCESS Or ERROR if arrays do not match dimensions, 
//matrix 3 columns(x) and rows(y) are passed by reference and are set when the function is done,
//need to manually put data from pointer *m3 into 2D array after
int Matrix_Add(void *m1, int x1, int y1, void *m2, int x2, int y2, void *m3, int *x3, int *y3)
{
    if ((x1 != x2) || (y1 != y2)) {
        return (ERROR);
    }
    double *matrix1 = (double *) m1; //turns void pointers back into double array pointers
    double *matrix2 = (double *) m2; //only 1 dimensional to simplify turning back into 2D later
    double *matrix3 = (double *) m3;

    int i;
    for (i = 0; i < (y1 * x2); i++) { //puts new matrix values into 1D array for matrix 3
        matrix3[i] = matrix1[i] + matrix2[i];
    }
    *y3 = y2;
    *x3 = x2;
    return (SUCCESS);
}

//inputs: 2D array address, x = number of columns, y = number of rows
//returns: determinant or 0 if not a square matrix
double Matrix_Determinant(void *matrix, int x, int y)
{
    double *ma = (double *) matrix; //turns void pointer back into double array pointer
    double B[y][x];
    int i, j;
    for (i = 0; i < y; i++) {
        for (j = 0; j < x; j++) {
            B[i][j] = ma[x * i + j]; //inputs matrix values back into 2D array
        }
    }
    //from here the matrix B[y][x] is used
    int m = x;
    int n = y;
    int row_size = m;
    int column_size = n;

    if (row_size != column_size) { //returns 0 for error if matrix is not square
        return (0);
    }
    else if (row_size == 1) //returns single number if only one element
        return (B[0][0]);
    else if (row_size == 2) //simple 2x2 matrix calculation
        return (B[0][0] * B[1][1] - B[1][0] * B[0][1]);
    else { //recurses until a 2x2 matrix is achieved
        double minor[row_size - 1][column_size - 1];
        int row_minor, column_minor;
        int firstrow_columnindex;
        double sum = 0;
        register int row, column;
        for (firstrow_columnindex = 0; firstrow_columnindex < row_size;
                firstrow_columnindex++) {
            row_minor = 0;
            for (row = 1; row < row_size; row++) {
                column_minor = 0;
                for (column = 0; column < column_size; column++) {
                    if (column == firstrow_columnindex)
                        continue;
                    else
                        minor[row_minor][column_minor] = B[row][column];
                    column_minor++;
                }
                row_minor++;
            }
            m = row_minor;
            n = column_minor;
            if (firstrow_columnindex % 2 == 0)
                sum += B[0][firstrow_columnindex] * Matrix_Determinant(&minor, column_minor, row_minor);
            else
                sum -= B[0][firstrow_columnindex] * Matrix_Determinant(&minor, column_minor, row_minor);
        }
        return sum;
    }
}

//#define MATRIX_DETERMINANT_TEST
#ifdef MATRIX_DETERMINANT_TEST

int main(void)
{
    BOARD_Init();
    double matrix[4][4] = {//defines 1st matrix, edit for testing
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
    int x = sizeof (matrix[0]) / DOUBLE_LENGTH; //needs columns and row number for function
    int y = sizeof (matrix) / x / DOUBLE_LENGTH;

    double det1 = Matrix_Determinant(&matrix, x, y);
    printf("%f", det1); //prints determinant of matrix
    BOARD_End();
    return (0);
}
#endif

//#define MATRIX_MULTIPLY_TEST
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

    double matrix3temp[MAX_MATRIX_SIZE * MAX_MATRIX_SIZE]; //defines third matrix, 1D for sizing, max size to determine rows/columns later
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

//#define MATRIX_SCALAR_MULTIPLY_TEST
#ifdef MATRIX_SCALAR_MULTIPLY_TEST
int main(void)
{
    BOARD_Init();
    double matrix[2][3] = {//defines 1st matrix, edit for testing
        {0, 1, 2},
        {3, 4, 5}
    };
    int columns = (sizeof (matrix[0])) / DOUBLE_LENGTH; //finds size of matrix
    int rows = (sizeof (matrix)) / columns / DOUBLE_LENGTH;
    double scalar = 2;
    int success = Matrix_ScalarMultiply(&matrix, columns, rows, scalar);

    int i, j;
    for (i = 0; i < rows; i++) { //prints array
        for (j = 0; j < columns; j++) {
            printf("%f ", matrix[i][j]);
        }
        printf("\n");
    }
    BOARD_End();
    return (0);
}
#endif

//#define MATRIX_ADD_TEST
#ifdef MATRIX_ADD_TEST

int main(void)
{
    BOARD_Init();
    double matrix1[3][3] = {//defines 1st matrix, edit for testing
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8}
    };
    int columns1 = (sizeof (matrix1[0])) / DOUBLE_LENGTH; //finds size of matrix
    int rows1 = (sizeof (matrix1)) / columns1 / DOUBLE_LENGTH;

    double matrix2[3][3] = {//defines 2nd matrix, edit for testing
        {9, 10, 11},
        {12, 13, 14},
        {15, 16, 17}
    };
    int columns2 = (sizeof (matrix2[0])) / DOUBLE_LENGTH; //finds size of 2nd matrix
    int rows2 = (sizeof (matrix2)) / columns2 / DOUBLE_LENGTH;

    double matrix3temp[MAX_MATRIX_SIZE * MAX_MATRIX_SIZE]; //defines third matrix, 1D for sizing, max size to determine rows/columns later
    int columns3; //empty variables to input size of third matrix
    int rows3;

    int success = Matrix_Add(&matrix1, columns1, rows1, &matrix2, columns2, rows2, &matrix3temp, &columns3, &rows3);

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