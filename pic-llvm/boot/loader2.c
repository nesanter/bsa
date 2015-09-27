#define IS_BOOTLOADER

#include "boot/bootlib.h"
#include "boot/loader2.h"
#include "version.h"

const unsigned int BLOCK_SIZE = 1024;

// triggered after "BSA PREAMBLE" is received
void preamble(void) {

    unsigned int cmd, new_baud = DEFAULT_BAUD;
    while (1) {
        boot_read_blocking((char*)&cmd, 4);
        switch (cmd) {
            case 0x5645523FU:
                boot_print_int(BOOTLOADER_VERSION);
                if (boot_expect("OK"))
                    boot_internal_error(1);
                break;
            case 0x42415544U:
                boot_read_blocking((char*)&new_baud, 4);
                // could check baud here
                boot_print("OK");
                break;
            case 0x42535A3FU:
                boot_print_int(BLOCK_SIZE);
                break;
            case 0x444F4E45U:
                boot_print("OK");
                boot_uart_change_baud(new_baud);
                boot_print("BOOTLOADER READY");
                if (boot_expect("OK"))
                    boot_internal_error(1);
                return;
            default:
                boot_print("NO");
                boot_internal_error(1);
                break;
        }
    }
}

// triggered after "LOAD" is received
void load(void) {
    // get entry

    // get pre-info

    // get data
}
