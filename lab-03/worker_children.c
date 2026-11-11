#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define CHILDREN 3

int main(void) {
    pid_t pids[CHILDREN];

    for (int i = 0; i < CHILDREN; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            return 1;
        }
        if (pid == 0) {
            sleep(1);
            if (i == 1) {
                abort();
            }
            _exit(10 + i);
        }
        pids[i] = pid;
    }

    for (int i = 0; i < CHILDREN; i++) {
        int status;
        if (waitpid(pids[i], &status, 0) == -1) {
            perror("waitpid");
            return 1;
        }
        if (WIFEXITED(status)) {
            printf("Нащадок %d: код завершення %d\n", i, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Нащадок %d: сигнал %d\n", i, WTERMSIG(status));
        }
    }
    return 0;
}
