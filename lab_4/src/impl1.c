#include "library.h"

#include <math.h>
#include <stddef.h>

float sin_integral(float a, float b, float e) {
    if (e <= 0.0f) {
        return NAN;
    }

    double left = a;
    double right = b;
    double sign = 1.0;
    if (left > right) {
        double temp = left;
        left = right;
        right = temp;
        sign = -1.0;
    }
    if (left == right) {
        return 0.0f;
    }

    double count = ceil((right - left) / (double)e);
    if (count > 10000000.0) {
        return NAN;
    }

    double sum = 0.0;
    double x = left;
    for (size_t i = 0; i < (size_t)count; ++i) {
        double next = fmin(x + (double)e, right);
        if (next <= x) {
            return NAN;
        }
        sum += sin(x) * (next - x);
        x = next;
    }
    return (float)(sign * sum);
}


float area(float a, float b) {
    if (a < 0.0f || b < 0.0f) {
        return NAN;
    }
    return a * b;
}
