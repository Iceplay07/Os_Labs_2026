#include "common.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>

void send_result(SharedData *data, sem_t *empty, sem_t *ready, long long sum, State state) {
    if (sem_wait(empty) == -1) {
        char msg[] = "error: failed to wait for semaphore\n";
        write(STDERR_FILENO, msg, sizeof(msg) - 1);
        exit(1);
    }
    data->sum = sum;
    data->state = state;
    if (sem_post(ready) == -1) {
        char msg[] = "error: failed to post semaphore\n";
        write(STDERR_FILENO, msg, sizeof(msg) - 1);
        exit(1);
    }
}

int main(int argc, char **argv) {
    if (argc != 4)
        return 1;

    int memory = shm_open(argv[1], O_RDWR, 0600);
    if (memory == -1)
        return 1;
    SharedData *data = (SharedData *)mmap(nullptr, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, memory, 0);
    close(memory);
    if (data == MAP_FAILED)
        return 1;

    sem_t *empty = sem_open(argv[2], 0);
    sem_t *ready = sem_open(argv[3], 0);

    if (empty == SEM_FAILED || ready == SEM_FAILED) {
        char msg[] = "error: failed to open semaphore\n";
        write(STDERR_FILENO, msg, sizeof(msg) - 1);
        if (empty != SEM_FAILED) sem_close(empty);
        if (ready != SEM_FAILED) sem_close(ready);
        munmap(data, sizeof(SharedData));
        return 1;
    }

    char c;
    ssize_t n;

    long long number = 0;
    long long sum = 0;

    int count = 0;

    bool negative = false;
    bool reading_number = false;

    while ((n = read(STDIN_FILENO, &c, 1)) > 0) {

        if (c >= '0' && c <= '9') {
            number = number * 10 + (c - '0');
            reading_number = true;
        }

        else if (c == '-') {
            negative = true;
        }

        else if (c == ' ' || c == '\n') {

            if (reading_number) {

                if (negative) {
                    number = -number;
                }

                sum += number;
                count++;

                number = 0;
                negative = false;
                reading_number = false;
            }

            if (c == '\n' && count > 0) {
                send_result(data, empty, ready, sum, RESULT);

                sum = 0;
                count = 0;
            }
        }
    }

    if (n == 0 && reading_number) {

        if (negative) {
            number = -number;
        }

        sum += number;
        count++;
    }

    if (n == 0 && count > 0) {
        send_result(data, empty, ready, sum, RESULT);
    }

    send_result(data, empty, ready, 0, n == 0 ? FINISH : READ_ERROR);
    sem_close(empty);
    sem_close(ready);
    munmap(data, sizeof(SharedData));
    return 0;
}
