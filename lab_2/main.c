
#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static double current_time_ms(void) {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time.tv_sec * 1000.0 + time.tv_nsec / 1000000.0;
}

int main(int argc, char *argv[]) {

    if (argc != 9 || strcmp(argv[1], "-m") != 0 ||
        strcmp(argv[3], "-n") != 0 || strcmp(argv[5], "-k") != 0 ||
        strcmp(argv[7], "-t") != 0) {
        printf("Использование: ./main -m M -n N -k K -t ПОТОКИ\n");
        printf("A[M x N] * B[N x K] = C[M x K]\n");
        return 1;
    }

    int m = atoi(argv[2]);
    int n = atoi(argv[4]);
    int k = atoi(argv[6]);
    int thread_count = atoi(argv[8]);

    if (m <= 0 || n <= 0 || k <= 0 || thread_count <= 0) {
        printf("Все размеры и число потоков должны быть больше нуля.\n");
        return 1;
    }

    Matrix a = {0};
    Matrix b = {0};
    Matrix sequential_result = {0};
    Matrix parallel_result = {0};

    int memory_ok = matrix_create(&a, m, n) &&
                    matrix_create(&b, n, k) &&
                    matrix_create(&sequential_result, m, k) &&
                    matrix_create(&parallel_result, m, k);

    if (!memory_ok) {
        fprintf(stderr, "Не удалось выделить память для матриц.\n");
        matrix_free(&a);
        matrix_free(&b);
        matrix_free(&sequential_result);
        matrix_free(&parallel_result);
        return 1;
    }

    matrix_fill(&a, 1);
    matrix_fill(&b, 2);

    double start = current_time_ms();
    multiply_sequential(&a, &b, &sequential_result);
    double sequential_ms = current_time_ms() - start;

    start = current_time_ms();
    int parallel_ok = multiply_parallel(&a, &b, &parallel_result,
                                        thread_count);
    double parallel_ms = current_time_ms() - start;

    int result_ok = parallel_ok &&
                    matrices_equal(&sequential_result, &parallel_result);

    if (parallel_ok) {
        double speedup = sequential_ms / parallel_ms;
        double efficiency = speedup / thread_count * 100.0;

        printf("A[%d x %d] * B[%d x %d] = C[%d x %d]\n",
               m, n, n, k, m, k);
        printf("Последовательно: %.3f мс\n", sequential_ms);
        printf("Потоков:         %d\n", thread_count);
        printf("Параллельно:     %.3f мс\n", parallel_ms);
        printf("Ускорение:       %.3f\n", speedup);
        printf("Эффективность:   %.2f%%\n", efficiency);
        printf("Проверка:        %s\n", result_ok ? "OK" : "ОШИБКА");
    } else {
        fprintf(stderr, "Не удалось создать потоки.\n");
    }

    matrix_free(&a);
    matrix_free(&b);
    matrix_free(&sequential_result);
    matrix_free(&parallel_result);

    return result_ok ? 0 : 1;
}
