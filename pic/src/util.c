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
