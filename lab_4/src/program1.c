
#include "library.h"

#include <math.h>
#include <stdio.h>

int main(void) {
    int command;
    float a, b, e;

    printf("Program 1: linked to implementation 1\n");
    printf("Commands: 1 A B e | 2 a b | Ctrl+D to exit\n");

    while (scanf("%d", &command) == 1) {
        if (command == 1) {
            if (scanf("%f %f %f", &a, &b, &e) != 3) {
                printf("Error: invalid input\n");
                break;
            }

            float result = sin_integral(a, b, e);

            if (!isnan(result)) {
                printf("Result: %.6f\n", result);
            } else {
                printf("Error: invalid arguments or too many intervals\n");
            }

        } else if (command == 2) {
            if (scanf("%f %f", &a, &b) != 2) {
                printf("Error: invalid input\n");
                break;
            }

            float result = area(a, b);

            if (!isnan(result)) {
                printf("Result: %.6f\n", result);
            } else {
                printf("Error: side lengths must be nonnegative and finite\n");
            }

        } else {
            printf("Error: expected command 1 or 2\n");
        }
    }

    return 0;
}
