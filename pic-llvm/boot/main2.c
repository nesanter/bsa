#define IS_BOOTLOADER

#include "proc/p32mx250f128b.h"
#include "boot/config.h"
#include "boot/flags.h"
#include "version.h"
#include "boot/bootlib.h"
#include "boot/handler.h"
#include "boot/loader2.h"
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

int check_reset_reason(unsigned int epc) {
    unsigned int rcon = RCON;

    if (rcon & REASON_BROWN_OUT) {
        boot_print("mWarning: BOR\r\n");
        RCONCLR = REASON_BROWN_OUT;
    }
    if (rcon & REASON_POWER_ON) {
        RCONCLR = REASON_POWER_ON;
    }
    if (rcon & REASON_MCLR) {
        boot_print("MCLR @");
        boot_print(tohex(epc,8));
        boot_print("\r\n");
    }

    return 0;
}

int main(void) {
//    flash_write_word_unsafe(boot_flags & 0x40, physical_address(&boot_flags));

    unsigned int errorepc;
    boot_handler_setup(&errorepc);
    boot_uart_init();
    
    boot_print("booting...\r\n");

    boot_signal_init();

    check_reset_reason(errorepc);
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

    char buffer [16];
    unsigned int n;

    while (1) {
        if ((n = boot_read_nonblocking(&buffer[n], 16 - n)) > 0) {
            if (n >= 12) {
                if (boot_strcmpn(buffer, "BSA PREAMBLE")) {
                    preamble();
                } else {
                    n = 0;
                }
            }
        }
        /*
        if (boot_flag & BOOT_FLAG_LOADED) {
            if (boot_run_read()) {
                run();
            }
        }
        */
        if (boot_run_read())
            boot_internal_error(0);
    }
}
