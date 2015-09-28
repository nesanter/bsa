#include <stdio.h>
#include <stdlib.h>

struct eh_t {
    struct eh_t *parent;
    char *ident;
    unsigned int flags;
    void (*always_handler)();
    void (*success_handler)();
    void (*failure_handler)(int info);
};

int ___entry(struct eh_t *eh);

int main(int argc, char **argv) {

    struct eh_t eh = { 0, 0 };

    return ___entry(&eh);
}

int ___write_builtin(struct eh_t *eh, int target, int value, char *str) {
    if (str) {
        printf("write [%d, %d] %d %s\n", target & 0xFFFF, (target & 0xFFFF0000) >> 16, value, str);
    } else {
        printf("write [%d, %d] %d\n", target & 0xFFFF, (target & 0xFFFF0000) >> 16, value);
    }
    return 1;
}

int ___read_builtin(struct eh_t *eh, int target) {
    printf("read [%d, %d]\n", target & 0xFFFF, (target & 0xFFFF0000) >> 16);
    
    return 1;
}

int ___write_addr_builtin(struct eh_t *eh, int target, int value, int addr) {
    printf("write_addr [%d, %d] %d %d\n", target & 0xFFFF, (target & 0xFFFF0000) >> 16, value, addr);

    return 1;
}

int ___read_addr_builtin(struct eh_t *eh, int target, int addr) {
    printf("read_addr [%d, %d] %d\n", target & 0xFFFF, (target & 0xFFFF0000) >> 16, addr);

    return 1;
}

int ___fork_builtin(int (*fn) ()) { return 0; }
int ___yield_builtin() { return 0; }

void ___fail_builtin(struct eh_t *eh) {
    while (eh) {
        if (eh->flags & 0x4) {
            eh->failure_handler(0);
        }
        if (eh->flags & 0x1) {
            eh->always_handler();
        }
        eh = eh->parent;
    }
    exit(1);
}

void ___trace_builtin(struct eh_t *eh) {
    while (eh) {
        if (eh->ident)
            printf("%s\n", eh->ident);
        eh = eh->parent;
    }
}
