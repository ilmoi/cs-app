#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

void mmapcopy(int fd, int size) {
    int *buf;
    buf = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (buf == MAP_FAILED)
        printf("map failed");
    write(1, buf, size);
    return;
}

int main(int argc, char **argv) {
    printf("yay\n");
    struct stat stat; //system struct that is designed to store information about files
    int fd;
    fd = open(argv[1], O_RDONLY, 0);
    fstat(fd, &stat);
    mmapcopy(fd, stat.st_size);
    exit(0);
}

