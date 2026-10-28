#include "common.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdlib.h>

void print_number(long long number) {
    char buffer[20];
    int size = 0;

    if (number == 0) {
        write(STDOUT_FILENO, "0\n", 2);
        return;
    }

    unsigned long long value = number;
    if (number < 0) {
        write(STDOUT_FILENO, "-", 1);
        value = 0 - value;
    }

    while (value > 0) {
        buffer[size++] = '0' + value % 10;
        value /= 10;
    }

    for (int i = size - 1; i >= 0; i--) {
        write(STDOUT_FILENO, &buffer[i], 1);
    }

    write(STDOUT_FILENO, "\n", 1);
}


int main(int argc, char **argv) {
    if (argc != 2) {
        const char message[] = "Usage: ./parent filename\n";
        write(STDERR_FILENO, message, sizeof(message) - 1);
        return 1;
    }

    int file = open(argv[1], O_RDONLY);
    if (file == -1) {
        const char error[] = "Cannot open file\n";
        write(STDERR_FILENO, error, sizeof(error) - 1);
        return 1;
    }

    char memory_name[32], empty_name[32], ready_name[32];
    make_name(memory_name, 'm');
    make_name(empty_name, 'e');
    make_name(ready_name, 'r');

    int result = 1;
    pid_t pid = -1;
    SharedData *data = (SharedData *)MAP_FAILED;
    sem_t *empty = SEM_FAILED, *ready = SEM_FAILED;
    int memory = shm_open(memory_name, O_CREAT | O_EXCL | O_RDWR, 0600);

    if (memory != -1 && ftruncate(memory, sizeof(SharedData)) == 0) {
        data = (SharedData *)mmap(nullptr, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, memory, 0);
    }
    if (data != MAP_FAILED) {
        empty = sem_open(empty_name, O_CREAT | O_EXCL, 0600, 1);
        ready = sem_open(ready_name, O_CREAT | O_EXCL, 0600, 0);
    }
    if (empty != SEM_FAILED && ready != SEM_FAILED) {
        pid = fork();
    }
    if (pid == 0) {
        if (dup2(file, STDIN_FILENO) != -1) {
            if (file != STDIN_FILENO) close(file);
            close(memory);
            char program[] = "./child";
            char *args[] = {program, memory_name, empty_name, ready_name, nullptr};
            execv(program, args);
        }
        data->state = READ_ERROR;
        sem_post(ready);
        exit(1);
    }
    if (pid > 0) {
        if(close(file) == -1) {
            const char msg[] = "error: failed to close file\n";
            write(STDERR_FILENO, msg, sizeof(msg) - 1);
            result = 1;
        }

        while (sem_wait(ready) == 0) {
            if (data->state == FINISH) {
                result = 0;
                break;
            }
            if (data->state == READ_ERROR){
                result = 1;
                break;
            }
            print_number(data->sum);
            sem_post(empty); 
        }

        if (waitpid(pid, nullptr, 0) == -1)
            result = 1;
    }
    
    if (data != MAP_FAILED)
        munmap(data, sizeof(SharedData));

    if (empty != SEM_FAILED) {
        sem_close(empty);
        sem_unlink(empty_name);
    }
    if (ready != SEM_FAILED) {
        sem_close(ready);
        sem_unlink(ready_name);
    }
    if (memory != -1) {
        close(memory);
        shm_unlink(memory_name);
    }
    if (result != 0) {
        char msg[] = "error: failed\n";
        write(STDERR_FILENO, msg, sizeof(msg) - 1);
    }
    return result;
}
