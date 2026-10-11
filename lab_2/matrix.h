#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

typedef struct {
    double real;
    double imag;
} Complex;

typedef struct {
    size_t rows;
    size_t cols;
    Complex *data;
} Matrix;

int matrix_create(Matrix *matrix, size_t rows, size_t cols);
void matrix_free(Matrix *matrix);
void matrix_fill(Matrix *matrix, int seed);

void multiply_rows(const Matrix *a, const Matrix *b, Matrix *result,
                   size_t first_row, size_t last_row);
void multiply_sequential(const Matrix *a, const Matrix *b, Matrix *result);
int multiply_parallel(const Matrix *a, const Matrix *b, Matrix *result,
                      size_t thread_count);
int matrices_equal(const Matrix *a, const Matrix *b);

#endif
