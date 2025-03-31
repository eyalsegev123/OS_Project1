#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    printf("Memory size: %d bytes\n", memsize());
    void* p = malloc(20000);
    printf("After malloc: %d bytes\n", memsize());
    free(p);
    printf("After free: %d bytes\n", memsize());
    exit(0);
}