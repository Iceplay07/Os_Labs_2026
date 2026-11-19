
#include "library.h"

#include <dlfcn.h>
#include <math.h>
#include <stdio.h>

#if defined(__APPLE__)
#define LIB_EXT ".dylib"
#else
#define LIB_EXT ".so"
#endif

const char *paths[] = {
    "./libimpl1" LIB_EXT,
    "./libimpl2" LIB_EXT
};

typedef struct {
    void *handle;
    sin_integral_func integral;
    area_func area_function;
    int implementation;
} Library;

int open_library(Library *out, int implementation) {
    void *handle = dlopen(paths[implementation], RTLD_NOW | RTLD_LOCAL);

    if (handle == NULL) {
        fprintf(stderr, "dlopen error: %s\n", dlerror());
        return 0;
    }

    sin_integral_func integral = NULL;
    area_func area_function = NULL;

    dlerror();
    integral = (sin_integral_func)dlsym(handle, "sin_integral");

    const char *error = dlerror();
    if (error != NULL) {
        fprintf(stderr, "dlsym(sin_integral): %s\n", error);
        dlclose(handle);
        return 0;
    }

    area_function = (area_func)dlsym(handle, "area");

    error = dlerror();
    if (error != NULL) {
        fprintf(stderr, "dlsym(area): %s\n", error);
        dlclose(handle);
        return 0;
    }

    out->handle = handle;
    out->integral = integral;
    out->area_function = area_function;
    out->implementation = implementation;

    return 1;
}

int main(void) {
    Library current = {0};

    if (!open_library(&current, 0)) {
        return 1;
    }

    printf("Program 2: dynamic loading\n");
    printf("Implementation 1: left rectangles and rectangle\n");
    printf("Commands: 0 (switch) | 1 A B e | 2 a b | Ctrl+D to exit\n");

    int command;
    float a, b, e;

    while (scanf("%d", &command) == 1) {

        if (command == 0) {
            int next;

            if (current.implementation == 0) {
                next = 1;
            } else {
                next = 0;
            }

            Library replacement = {0};

            if (open_library(&replacement, next)) {
                dlclose(current.handle);
                current = replacement;

                printf("Switched to implementation %d\n", next + 1);
            } else {
                printf("Switch failed; current implementation is unchanged\n");
            }

        } else if (command == 1) {
            if (scanf("%f %f %f", &a, &b, &e) != 3) {
                printf("Error: invalid input\n");
                break;
            }

            float result = current.integral(a, b, e);

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

            float result = current.area_function(a, b);

            if (!isnan(result)) {
                printf("Result: %.6f\n", result);
            } else {
                printf("Error: side lengths must be nonnegative and finite\n");
            }

        } else {
            printf("Error: expected command 0, 1 or 2\n");
        }
    }

    dlclose(current.handle);
    return 0;
}
