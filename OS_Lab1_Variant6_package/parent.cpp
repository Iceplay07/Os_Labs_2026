#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        const char msg[] = "Usage: ./parent filename\n";
        write(STDERR_FILENO, msg, sizeof(msg) - 1);
        return 1;
    }



    int file = open(argv[1], O_RDONLY);

    if (file == -1) {
        const char error[] = "Cannot open file\n";
        write(STDERR_FILENO, error, sizeof(error) - 1);
        return 1;
    }


    int fd[2];

    if (pipe(fd) == -1) {
        const char error[] = "Pipe error\n";
        write(STDERR_FILENO, error, sizeof(error) - 1);
        return 1;
    }


    pid_t pid = fork();

    if (pid == -1) {
        const char error[] = "Fork error\n";
        write(STDERR_FILENO, error, sizeof(error) - 1);
        return 1;
    }

    if (pid == 0) {


        close(fd[0]);


        dup2(file, STDIN_FILENO);


        dup2(fd[1], STDOUT_FILENO);

        close(file);
        close(fd[1]);

        char program[] = "./child";
        char* args[] = {program, nullptr};

        execv(program, args);


        const char error[] = "Exec error\n";
        write(STDERR_FILENO, error, sizeof(error) - 1);

        _exit(1);
    }



    close(file);
    close(fd[1]);

    char buffer[256];
    int n;


    while ((n = read(fd[0], buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, n);
    }

    close(fd[0]);

 
    waitpid(pid, nullptr, 0);

    return 0;
}