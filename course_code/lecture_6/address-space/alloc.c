#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#define GiB * (1024LL * 1024 * 1024)

int main() {
    volatile uint8_t *p = mmap(
        NULL,
        2 GiB,
        PROT_READ | PROT_WRITE,
        MAP_ANONYMOUS | MAP_PRIVATE,
        -1, 0
    );

    printf("mmap: %lx\n", (uintptr_t)p);

    if ((intptr_t)p == -1) {
        perror("cannot map");
        exit(1);
    }

    *(p +  1 GiB) = 10;
    // *(p + 4 GiB) = 2;
    // *(p + 7 GiB) = 3;
    printf("Read get: %d\n", *(p + 1 GiB));
    printf("Read get: %d\n", *(p + 6 GiB));
    // printf("Read get: %d\n", *(p + 7 GiB));
}
