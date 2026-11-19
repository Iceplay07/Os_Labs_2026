#ifndef OS_LAB4_LIBRARY_H
#define OS_LAB4_LIBRARY_H


float sin_integral(float a, float b, float e);
float area(float a, float b);


typedef float (*sin_integral_func)(float, float, float);
typedef float (*area_func)(float, float);

#endif
