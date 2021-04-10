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

//inputs: 2D array address input m1 with its columns(x) and rows(y), 
//matrix m2 must have it's size defined before the function to be opposite of m1
//returns: SUCCESS

int Matrix_Transpose(void *m1, int x, int y, void *m2)
{
    double *min = (double *) m1; //turns 2D pointers into 1D pointers
    double *mout = (double *) m2;

    int i, j;
    for (i = 0; i < x; i++) {
        for (j = 0; j < y; j++) {
            mout[y * i + j] = min[j * x + i]; //transposes min matrix into mout matrix
        }
    }
    return (SUCCESS);
}

//inputs: 2 2D array addresses 1 and 2 with their columns(x) and rows(y), 
//matrix 3 is also 2D whose size must be determined before function
//returns: SUCCESS Or ERROR if arrays do not match dimensions, 

int Matrix_Multiply(void *m1, int x1, int y1, void *m2, int x2, int y2, void *m3)
{
    if ((x1 != y2) || (x2 != y1)) { //error if matrix 1 and 2 aren't compatible
        return (ERROR);
    }
    double *matrix1 = (double *) m1; //turns void pointers back into 1D double array pointers
    double *matrix2 = (double *) m2; //
    double *matrix3 = (double *) m3;

    int i, j;
    for (i = 0; i < (y1 * x2); i++) { //puts new matrix values into 1D array for matrix 3
        matrix3[i] = 0;
        for (j = 0; j < y2; j++) { //must multiply each row of matrix 1 by each row in matrix 2
            matrix3[i] += matrix1[j + ((i / y1) * x1)] * matrix2[j * x2 + (i % x2)];
        }
    }
    return (SUCCESS);
}

//inputs: 2D array address with columns(x) and rows(y), scalar double value s to multiply by, 
//also output matrix mout must be defined before function
//returns: SUCCESS

int Matrix_ScalarMultiply(void *m, int x, int y, double s, void *mout)
{
    double *matrix = (double *) m;
    double *matrixout = (double *) mout;
    int i;
    for (i = 0; i < (y * x); i++) {// multiplies each value in array by scalar value
        matrixout[i] = matrix[i] * s; //does this in 1 dimension, but it loops through the rows automatically
    }
    return (SUCCESS);
}

//inputs: 2D array address matrix 1 with columns(x) and rows(y), 
//matrix 2 must have it's size defined to be the same as matrix 1 before function
//returns: SUCCESS Or ERROR if array is not square

int Matrix_Inverse(void *matrix1, int x, int y, void *matrix2)
{
    if (x != y) { //error if matrix is not square
        return (ERROR);
    }
    double *min = (double *) matrix1; //turns 2D input pointers into 1D pointers
    double *mout = (double *) matrix2;

    double det = Matrix_Determinant(min, x, y); //finds the determinant

    int i, j;
    int k = 0;
    double mtemp[y][x];
    for (i = 0; i < y; i++) { //creates 2D cofactor matrix of min into mtemp
        for (j = 0; j < x; j++) {
            if (k % 2 == 1) {
                mtemp[i][j] = min[i * x + j] * -1;
            } else {
                mtemp[i][j] = min[i * x + j];
            }
            k++;
        }
    }
    for (i = 0; i < y; i++) {
        for (j = 0; j < x; j++) {
            mout[x * i + j] = mtemp[j][i] / det; //transposes cofactor matrix and divides by determinant
        }
    }
    return (SUCCESS);
}

//inputs: 2 2D array addresses with their columns(x) and rows(y), matrix 3 address as well, 
//whose size must be defined before the function
//returns: SUCCESS Or ERROR if arrays do not match dimensions, 

int Matrix_Add(void *m1, int x1, int y1, void *m2, int x2, int y2, void *m3)
{
    if ((x1 != x2) || (y1 != y2)) {
        return (ERROR);
    }
    double *matrix1 = (double *) m1; //turns void pointers back into double array pointers
    double *matrix2 = (double *) m2; //only 1 dimensional to simplify turning back into 2D later
    double *matrix3 = (double *) m3;

    int i;
    for (i = 0; i < (y1 * x1); i++) { //puts new matrix values into 1D array for matrix 3
        matrix3[i] = matrix1[i] + matrix2[i];
    }
    return (SUCCESS);
}

//inputs: 2D array address, x = number of columns, y = number of rows
//returns: determinant or 0 if not a square matrix

double Matrix_Determinant(void *matrix, int x, int y)
{
    if (x != y) {
        return (0);
    }
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
    } else if (row_size == 1) //returns single number if only one element
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

//inputs: 3 1x3 array addresses, m3 is the output matrix
//returns: SUCCESS
int Matrix_CrossProduct(double m1[1][3], double m2[1][3], double m3[1][3])
{
    m3[0][0] = m1[0][1] * m2[0][2] - m1[0][2] * m2[0][1];
    m3[0][1] = m1[0][0] * m2[0][2] - m1[0][2] * m2[0][0];
    m3[0][2] = m1[0][0] * m2[0][1] - m1[0][1] * m2[0][0];
    return (SUCCESS);
}
//#define MATRIX_CROSSPRODUCT_TEST
#ifdef MATRIX_CROSSPRODUCT_TEST

int main(void)
{
    BOARD_Init();
    double matrix1[1][3] = {//defines matrix1, edit for testing
        {0, 1, 2}
    };
    double matrix2[1][3] = {//defines matrix2, edit for testing
        {3, 4, 5}
    };
    double matrix3[1][3]; //output matrix 3

    int success = Matrix_CrossProduct(matrix1, matrix2, matrix3);
    
    int i;
    for (i = 0; i < 3; i++) {
        printf("%f", matrix3[0][i]); //prints cross product
    }
    BOARD_End();
    return (0);
}
#endif


//#define MATRIX_TRANSPOSE_TEST
#ifdef MATRIX_TRANSPOSE_TEST

int main(void)
{
    BOARD_Init();
    double matrix[4][4] = {//defines matrix, edit for testing
        {0, 1, 2, 3},
        {4, 5, 6, 7},
        {8, 9, 10, 11},
        {12, 13, 14, 15}
    };
    int x = sizeof (matrix[0]) / DOUBLE_LENGTH; //x = number of columns
    int y = sizeof (matrix) / x / DOUBLE_LENGTH; //y = number of rows

    double transpose[x][y]; //transpose x and y dimensions switch
    double success = Matrix_Transpose(&matrix, x, y, &transpose); //inputs array addresses and dimensions

    int i, j;
    for (i = 0; i < x; i++) { //prints transpose of original matrix
        for (j = 0; j < y; j++) {
            printf("%f ", transpose[i][j]);
        }
        printf("\n");
    }

    BOARD_End();
    return (0);
}
#endif

//#define MATRIX_INVERSE_TEST
#ifdef MATRIX_INVERSE_TEST

int main(void)
{
    BOARD_Init();
    double matrix[4][4] = {//defines matrix, edit for testing
        {1, 2, 3, 4},
        {4, 5, 6, 7},
        {8, 11, 10, 11},
        {12, 13, 19, 15}
    };
    int x = sizeof (matrix[0]) / DOUBLE_LENGTH; //needs columns and row number for function
    int y = sizeof (matrix) / x / DOUBLE_LENGTH;

    double inverse[y][x]; //needs to define new 2D matrix with the same size to input values
    double success = Matrix_Inverse(&matrix, x, y, &inverse); //inputs array addresses and dimensions

    int i, j;
    for (i = 0; i < 4; i++) { //prints new inverse matrix
        for (j = 0; j < 4; j++) {
            printf("%f", inverse[i][j]);
        }
        printf("\n");
    }
    BOARD_End();
    return (0);
}
#endif

//#define MATRIX_DETERMINANT_TEST
#ifdef MATRIX_DETERMINANT_TEST

int main(void)
{
    BOARD_Init();
    double matrix[4][4] = {//defines matrix, edit for testing
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
    int x = sizeof (matrix[0]) / DOUBLE_LENGTH; //needs columns and row number for function
    int y = sizeof (matrix) / x / DOUBLE_LENGTH;

    double det = Matrix_Determinant(&matrix, x, y);
    printf("%f", det); //prints determinant of matrix
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


    int columns3 = columns2;
    int rows3 = rows1;
    double matrix3[rows3][columns3]; //defines third matrix and its size based on matrix 1 and 2

    //2d array addresses matrix 1 and 2 are inputted along with their sizes, also address for matrix 3
    int success = Matrix_Multiply(&matrix1, columns1, rows1, &matrix2, columns2, rows2, &matrix3);

    int i, j;
    for (i = 0; i < rows3; i++) { //prints matrix3
        for (j = 0; j < columns3; j++) {
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

    double scalar = 2; //defines scalar to multiply by
    double matrixout[rows][columns]; //creates new matrix based on original matrix

    int success = Matrix_ScalarMultiply(&matrix, columns, rows, scalar, &matrixout);

    int i, j;
    for (i = 0; i < rows; i++) { //puts new matrix
        for (j = 0; j < columns; j++) {
            printf("%f ", matrixout[i][j]);
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
    int columns1 = (sizeof (matrix1[0])) / DOUBLE_LENGTH; //finds size of matrix1
    int rows1 = (sizeof (matrix1)) / columns1 / DOUBLE_LENGTH;

    double matrix2[3][3] = {//defines 2nd matrix, edit for testing
        {9, 10, 11},
        {12, 13, 14},
        {15, 16, 17}
    };
    int columns2 = (sizeof (matrix2[0])) / DOUBLE_LENGTH; //finds size of matrix2
    int rows2 = (sizeof (matrix2)) / columns2 / DOUBLE_LENGTH;

    double matrix3[rows2][columns2]; //defines new matrix same size as the previous ones

    //inputs 2d array addresses matrix 1 and 2, along with their sizes, and address to 1D matrix3temp
    int success = Matrix_Add(&matrix1, columns1, rows1, &matrix2, columns2, rows2, &matrix3);

    int i, j;
    for (i = 0; i < rows1; i++) { //prints new matrix
        for (j = 0; j < columns1; j++) {
            printf("%f ", matrix3[i][j]);
        }
        printf("\n");
    }
    //in the end values are put into matrix3, whose size is dependent on matrix 1 and 2
    BOARD_End();
    return (0);
}
#endif