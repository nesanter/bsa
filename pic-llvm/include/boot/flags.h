#ifndef BOOT_FLAGS_H
#define BOOT_FLAGS_H


enum {
    SYSCALL_RESET = 0x0
};

/*
enum {
    BOOT_FLAG_NOPROGRAM = 0x1,
    BOOT_FLAG_HOLD = 0x2,
    BOOT_FLAG_HOLD_ON_ERROR = 0x4,
    BOOT_FLAG_RESET_ON_ERROR = 0x8
};
*/

#ifdef IS_BOOTLOADER
struct boot_status {
    void * runtime_entry;
    void * init_map;
    void * init_data;
};
#endif

#endif /* BOOT_FLAGS_H */
