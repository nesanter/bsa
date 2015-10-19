#include "ulib/util.h"
#include "proc/processor.h"

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

int strcmpn(char *stra, char *strb, unsigned int len) {
    int i = 0;
    while (i++ < len && (*stra++ == *strb++) && *stra);
    return (i == len) || *stra == *strb;
}

void *memset(void *s, int c, unsigned int n) {
    unsigned int fill = (c << 24 | c << 16 | c << 8 | c);
    void *sorig = s;
//    while (n--  (~(sizeof(unsigned int)-1))) {
//        *(unsigned int*)s = fill;
//        s += sizeof(unsigned int);
//    }
    while (n--) {
        *(unsigned char*)s++ = (unsigned char)c;
    }
    return sorig;
}

void runtime_delay(int n) {
    unsigned int old_iec0 = IEC0 & 0x1;
    IEC0CLR = 0x1;

    unsigned int start, cur;
    asm volatile ("mfc0 %0, $9" : "=r"(start));
    start += n;

    do {
        asm volatile ("mfc0 %0, $9" : "=r"(cur));
    } while (cur < start);

    IEC0SET = old_iec0;
    asm volatile ("mtc0 %0, $9" : "+r"(start));
}
