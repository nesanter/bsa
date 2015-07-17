#define IS_BOOTLOADER

#include "boot/config.h"
#include "boot/flags.h"
#include "version.h"
#include "boot/bootlib.h"
#include "boot/handler.h"
#include "boot/loader.h"
#include "ulib/util.h"

/* Quick note on signal patterns:
 *
 * One signal (RESET) is driven by the reset circuit,
 *   independent of the processor
 * Two signals (BOOT and USER) are driven by the processor
 *
 * The following combinations are possible:
 *   RESET -- reset switch is active
 *   BOOT, solid -- bootloader is ready and idle
 *   BOOT, flashing -- bootloader is loading user image
 *   USER, flashing -- loading complete, holding
 *   USER, solid -- user program is running
 *   BOOT/USER alternating -- a general exception has occured
 */

unsigned int __attribute__((section(".boot_flags"))) boot_flags = BOOT_FLAG_HOLD | BOOT_FLAG_NOPROGRAM;

int main(void) {
    unsigned int errorepc;
    boot_handler_setup(&errorepc);
    boot_uart_init();
    
    boot_print("booting...\r\n");

    boot_signal_init();
//    boot_transfer_init();

    /*
    boot_print("BOOTLOADER ");
    boot_print(BOOTLOADER_VERSION);
    boot_print("\r\n");

    if (errorepc) {
        boot_print("ERROR MAY HAVE OCCURED [");
        boot_print(tohex(errorepc, 8));
        boot_print("\r\n");
    }
    */

    boot_signal_set(SIGNAL_BOOT, 1);
    boot_signal_set(SIGNAL_USER, 0);

    char cmd;
    int startup_messages = 0;
    unsigned int entry, user_sp, user_gp;

    while (startup_messages < 3) {
        boot_read_blocking(&cmd, 1);

        switch (cmd) {
            case '>':
                if (startup_messages == 0) {
                    boot_print("mrunning bootloader ");
                    boot_print(BOOTLOADER_VERSION);
                    boot_print("\r\n");
                    startup_messages = 1;
                } else if (startup_messages == 1 && errorepc) {
                    boot_print("merror may have occured in prior life [");
                    boot_print(tohex(errorepc, 8));
                    boot_print("]\r\n");
                    startup_messages = 2;
                } else {
                    boot_print(">");
                    startup_messages = 3;
                }
                break;
            case '?':
                boot_print("mOK\r\n");
                break;
            default:
                break;
        }
    }

    while (1) {
        boot_read_blocking(&cmd, 1);

        switch (cmd) {
            case 'L':
//                if (!load_user_program(&entry, &user_sp, &user_gp)) {
//                    start_user_program(entry, user_sp, user_gp);
//                }
                boot_internal_error();
                break;
            default:
                break;
        }
    }


}
