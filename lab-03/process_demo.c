#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    printf("Батько: pid=%d, ppid=%d\n", getpid(), getppid());
    fflush(NULL);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        printf("Нащадок: pid=%d, ppid=%d\n", getpid(), getppid());
        fflush(NULL);
        execlp("echo", "echo", "Нащадок запустив команду echo", (char *)NULL);
        perror("execlp");
        _exit(127);
    }

    int status;
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
        return 1;
    }

    if (WIFEXITED(status)) {
        printf("Батько: нащадок завершився з кодом %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("Батько: нащадок завершився від сигналу %d\n", WTERMSIG(status));
    }
    return 0;
}
