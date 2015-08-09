#ifndef FAILSAFE_H
#define FAILSAFE_H

#ifdef IS_BOOTLOADER

void setup_failsafe(int timeout_sec);
void disable_failsafe();
void clear_failsafe();

#else
#error "runtime cannot access failsafe.h"
#endif

#endif /* FAILSAFE_H */
