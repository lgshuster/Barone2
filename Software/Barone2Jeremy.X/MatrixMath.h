#ifndef MATRIX_MATH_H    /* Guard against multiple inclusion */
#define MATRIX_MATH_H

int Matrix_Multiply(void *m1, int x1, int y1, void *m2, int x2, int y2, void *m3, int *x3, int *y3);

int Matrix_ScalarMultiply(void *m, int x, int y, double s);

int Matrix_Inverse(void *matrix1, int x, int y, void *matrix2);
   
int Matrix_Add(void *m1, int x, int y, void *m2, void *m3);

double Matrix_Determinant(void *m, int x, int y);
    
#endif