#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void) {
    const char *path = "notes.txt";
    char buffer[64];
    ssize_t n;
    int fd;

    fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        fprintf(stderr, "open (write): %s\n", strerror(errno));
        return 1;
    }

    const char *line = "Hello from lab-02\n";
    if (write(fd, line, strlen(line)) == -1) {
        fprintf(stderr, "write: %s\n", strerror(errno));
        close(fd);
        return 1;
    }
    close(fd);

    fd = open(path, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "open (read): %s\n", strerror(errno));
        return 1;
    }

    n = read(fd, buffer, sizeof(buffer) - 1);
    if (n == -1) {
        fprintf(stderr, "read: %s\n", strerror(errno));
        close(fd);
        return 1;
    }

    buffer[n] = '\0';
    printf("Прочитано %zd байтів: %s", n, buffer);

    if (lseek(fd, 11, SEEK_SET) == -1) {
        fprintf(stderr, "lseek: %s\n", strerror(errno));
        close(fd);
        return 1;
    }

    n = read(fd, buffer, 6);
    if (n == -1) {
        fprintf(stderr, "read: %s\n", strerror(errno));
        close(fd);
        return 1;
    }

    buffer[n] = '\0';
    printf("Зі зміщення 11 прочитано %zd байтів: %s\n", n, buffer);

    close(fd);
    return 0;
}

