#ifndef HEIGHT_H
#define HEIGHT_H

double Track_Height_Bottom(double measurement);

double Track_Height_Front_Left(double measurement);

double Track_Height_Front_Right(double measurement);

double Actual_Height(double bottom, double right, double left);

int Update_Values(double scalar, int row);

#endif