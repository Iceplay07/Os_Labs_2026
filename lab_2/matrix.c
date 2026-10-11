#include "matrix.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

int matrix_create(Matrix *matrix, size_t rows, size_t cols) {
    if (rows == 0 || cols == 0 ) {
        return 0;
    }

    matrix->rows = rows;
    matrix->cols = cols;
    matrix->data = malloc(rows * cols * sizeof(Complex));
    return matrix->data != NULL;
}

void matrix_free(Matrix *matrix) {
    free(matrix->data);
    matrix->data = NULL;
}

void matrix_fill(Matrix *matrix, int seed) {
    for (size_t row = 0; row < matrix->rows; row++) {
        for (size_t col = 0; col < matrix->cols; col++) {
            matrix->data[row * matrix->cols + col].real = (double)((row + 2 * col + seed) % 7) - 3.0;
            matrix->data[row * matrix->cols + col].imag = (double)((2 * row + col + seed) % 5) - 2.0;
        }
    }
}

void multiply_rows(const Matrix *a, const Matrix *b, Matrix *result,
                   size_t start, size_t end) {
    for (size_t row = start; row < end; row++) {
        for (size_t col = 0; col < b->cols; col++) {
            Complex sum = {0.0, 0.0};

            for (size_t k = 0; k < a->cols; k++) {
                Complex x = a->data[row * a->cols + k];
                Complex y = b->data[k * b->cols + col];
                sum.real += x.real * y.real - x.imag * y.imag;
                sum.imag += x.real * y.imag + x.imag * y.real;
            }

            result->data[row * result->cols + col] = sum;
        }
    }
}

void multiply_sequential(const Matrix *a, const Matrix *b, Matrix *result) {
    multiply_rows(a, b, result, 0, a->rows);
}

int matrices_equal(const Matrix *a, const Matrix *b) {
    if (a->rows != b->rows || a->cols != b->cols) {
        return 0;
    }

    size_t element_count = a->rows * a->cols;
    for (size_t i = 0; i < element_count; i++) {
        if (fabs(a->data[i].real - b->data[i].real) > 1e-6 ||
            fabs(a->data[i].imag - b->data[i].imag) > 1e-6) {
            return 0;
        }
    }
    return 1;
}
