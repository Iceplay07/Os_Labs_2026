#include "matrix.h"

#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {
    const Matrix *a;
    const Matrix *b;
    Matrix *result;
    size_t first_row;
    size_t last_row;
} ThreadData;

static void *worker(void *argument) {
    ThreadData *data = argument;
    multiply_rows(data->a, data->b, data->result,
                  data->first_row, data->last_row);
    return NULL;
}

int multiply_parallel(const Matrix *a, const Matrix *b, Matrix *result,
                      size_t thread_count) {
    if (thread_count == 0 ) {
        return 0;
    }

    pthread_t *threads = malloc(thread_count * sizeof(*threads));
    ThreadData *thread_data = malloc(thread_count * sizeof(*thread_data));
    if (threads == NULL || thread_data == NULL) {
        free(threads);
        free(thread_data);
        return 0;
    }


    size_t created = 0;
    size_t rows_per_thread = a->rows / thread_count;
    size_t extra_rows = a->rows % thread_count;
    size_t first_row = 0;

    for (size_t i = 0; i < thread_count; i++) {
        size_t row_count = rows_per_thread + (i < extra_rows ? 1 : 0);
        thread_data[i].a = a;
        thread_data[i].b = b;
        thread_data[i].result = result;
        thread_data[i].first_row = first_row;
        thread_data[i].last_row = first_row + row_count;
        first_row += row_count;

        if (pthread_create(&threads[i], NULL, worker, &thread_data[i]) != 0) {
            break;
        }
        created++;
    }

    for (size_t i = 0; i < created; i++) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(thread_data);
    return created == thread_count;
}
