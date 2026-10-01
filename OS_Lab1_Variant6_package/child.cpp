#include <unistd.h>

void print_number(long long number) {
    char buffer[20];
    int size = 0;

    if (number == 0) {
        write(STDOUT_FILENO, "0\n", 2);
        return;
    }

    if (number < 0) {
        write(STDOUT_FILENO, "-", 1);
        number = -number;
    }

    while (number > 0) {
        buffer[size++] = '0' + number % 10;
        number /= 10;
    }


    for (int i = size - 1; i >= 0; i--) {
        write(STDOUT_FILENO, &buffer[i], 1);
    }

    write(STDOUT_FILENO, "\n", 1);
}


int main() {
    char c;

    long long number = 0;
    long long sum = 0;

    int count = 0;

    bool negative = false;
    bool reading_number = false;

    while (read(STDIN_FILENO, &c, 1) > 0) {

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
                print_number(sum);

                sum = 0;
                count = 0;
            }
        }
    }


    if (reading_number) {

        if (negative) {
            number = -number;
        }

        sum += number;
        count++;
    }


    if (count > 0) {
        print_number(sum);
    }

    return 0;
}