#include "ulib/util.h"

char convbuffer[64];

char *tohex(unsigned int n, int length) {
    if (length > 62)
        length = 62;

    char *ptr = convbuffer + length;
    *ptr-- = '\0';
    while (length--) {
        if ((n & 0xF) > 9)
            *ptr-- = (n & 0xF) - 10 + 'A';
        else
            *ptr-- = (n & 0xF) + '0';
        n >>= 4;
    }

    return convbuffer;
}

char *todecimal(int n) {
    char *ptr = &convbuffer[63];
    *ptr-- = '\0';

    int neg = 0;
    if (n < 0) {
        neg = 1;
        n = -n;
    } else if (n == 0) {
        *ptr = '0';
        return ptr;
    }

    while (n > 0) {
        *ptr-- = '0' + (n % 10);
        n /= 10;
    }

    if (neg) {
        *ptr = '-';
        return ptr;
    } else {
        return ptr + 1;
    }
}
