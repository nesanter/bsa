#ifndef LIMITS_H
#define LIMITS_H

#ifdef IS_BOOTLOADER

#else
#error "runtime cannot access limits.h"
#endif

#endif /* LIMITS_H */
