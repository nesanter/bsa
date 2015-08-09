#define IS_BOOTLOADER

#include "boot/failsafe.h"
#include "proc/processor.h"
#include "boot/handler.h"
#include "boot/bootlib.h"
#include "version.h"


void __attribute((interrupt(IPL2SOFT), nomips16)) failsafe_handler() {
    IFS0CLR = 0x200;

    boot_internal_error(1);
    soft_reset();
}

void setup_failsafe(int timeout_sec) {
    int con = T2CON;
    int sidl = T3CON;

    con = 0x8058;
    sidl = 0x4000;

    int period = (SYSTEM_FREQ / 256) * timeout_sec;

    PR2 = period & 0xFFFF;
    PR3 = (period & 0xFFFF0000) >> 16;

    TMR2 = 0;
    TMR3 = 0;

    int int_config = IEC0;
    int_config |= 0x200;
    
    int int_priority = IPC3;
    int_priority &= 0x1B;
    int_priority |= 0x04;

    boot_set_vector_table_entry(_TIMER_3_VECTOR, &failsafe_handler);

    IPC3 = int_priority;
    IEC0 = int_config;

    T3CON = sidl;
    T2CON = con;
}

void disable_failsafe() {
    PR2 = 0;
    PR3 = 0;
    TMR2 = 0;
    TMR3 = 0;
    IEC0CLR = 0x200;
    T3CON = 0;
    T2CON = 0;
}

void clear_failsafe() {
    TMR2 = 0;
    TMR3 = 0;
}
