#include <stdio.h>

int entry();

int main(int argc, char **argv) {
    return entry();
}

int ___write_builtin(int target, int value, char *str) {
    if (str) {
        printf("write [%d, %d] %d %s\n", target & 0xFFFF, (target & 0xFFFF0000) >> 16, value, str);
    } else {
        printf("write [%d, %d] %d\n", target & 0xFFFF, (target & 0xFFFF0000) >> 16, value);
    }
    return 1;
}

int ___read_builtin(int target) {
    printf("read [%d, %d]\n", target & 0xFFFF, (target & 0xFFFF0000) >> 16);
    
    return 1;
}

int ___fork_builtin(int (*fn) ()) { return 0; }
int ___yield_builtin() { return 0; }

