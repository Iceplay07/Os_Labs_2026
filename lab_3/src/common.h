#ifndef LAB3_COMMON_H
#define LAB3_COMMON_H

#include <unistd.h>
#include <semaphore.h>

enum State { RESULT, FINISH, READ_ERROR };

struct SharedData {
    long long sum;
    State state;
};


void make_name(char *name, char kind) {
    name[0] = '/'; name[1] = 'l'; name[2] = '3';
    name[3] = kind; name[4] = '_';
    char digits[20];
    int count = 0;
    unsigned long pid = (unsigned long)getpid();
    do {
        digits[count++] = '0' + pid % 10;
        pid /= 10;
    } while (pid > 0);
    int pos = 5;
    while (count > 0) name[pos++] = digits[--count];
    name[pos] = '\0';
}

#endif
