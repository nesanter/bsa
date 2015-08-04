#define IS_BOOTLOADER

#include "proc/p32mx250f128b.h"
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

unsigned int const volatile __attribute((section(".boot_flags"))) boot_flags = BOOT_FLAG_HOLD | BOOT_FLAG_NOPROGRAM | BOOT_FLAG_HOLD_ON_ERROR;
unsigned int const volatile * const boot_flags_uncached = &boot_flags + 0x8000000;

void boot_flag_prompt() {
    char cmd;
    boot_read_blocking(&cmd, 1);
    int comma;

    unsigned int new_flags = *boot_flags_uncached;

    switch (cmd) {
        case '?':
            if (new_flags == 0) {
                boot_print("(none)");
                break;
            }
            if (new_flags & BOOT_FLAG_NOPROGRAM) {
                boot_print("noprog");
                comma = 1;
            }
            if (new_flags & BOOT_FLAG_HOLD) {
                if (comma)
                    boot_print(",hold");
                else
                    boot_print("hold");
                comma = 1;
            }
            if (new_flags & BOOT_FLAG_HOLD_ON_ERROR) {
                if (comma)
                    boot_print(",hoe");
                else
                    boot_print("hoe");
                comma = 1;
            }
            if (new_flags & BOOT_FLAG_RESET_ON_ERROR) {
                if (comma)
                    boot_print(",roe");
                else
                    boot_print("roe");
                comma = 1;
            }
            boot_print("\r\n");
            break;
        case 'H':
            //set hold
            new_flags |= BOOT_FLAG_HOLD;
            break;
        case 'h':
            new_flags &= (0xFFFFFFFF ^ BOOT_FLAG_HOLD);
            //clear hold
            break;
        case 'E':
            new_flags |= BOOT_FLAG_HOLD_ON_ERROR;
            //set hoe
            break;
        case 'e':
            //clear hoe
            new_flags &= (0xFFFFFFFF ^ BOOT_FLAG_HOLD_ON_ERROR);
            break;
        case 'X':
            //set roe
            new_flags |= BOOT_FLAG_RESET_ON_ERROR;
            break;
        case 'x':
            //clear roe
            new_flags &= (0xFFFFFFFF ^ BOOT_FLAG_RESET_ON_ERROR);
            break;
        default:
            return;
    }

    if (new_flags != *boot_flags_uncached) {
        boot_print(tohex(new_flags, 8));
        boot_print("\r\n");

        /*
        unsigned int res = flash_write_word_unsafe(new_flags, physical_address(boot_flags_uncached));
        if (res & 0x1000) {
            boot_print("LVD");
        } else if (res & 0x2000) {
            boot_print("ERR");
        } else if (res == 1) {
            boot_print("ADDR");
        } else {
            boot_print("OK");
        }
        */
    }
}

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
        boot_print("t");
        boot_print(tohex(epc,8));
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
                }/* else if (startup_messages == 1 && errorepc) {
                    boot_print("merror may have occured in prior life [");
                    boot_print(tohex(errorepc, 8));
                    boot_print("]\r\n");
                    startup_messages = 2;
                } */ else {
                    boot_print(">");
                    startup_messages = 3;
                }
                break;
            case '?':
                boot_print("mBootloader OK\r\n");
                break;
            case 'F':
                boot_flag_prompt();
            case 'Q':
                soft_reset();
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
                boot_internal_error(0);
                break;
            default:
                break;
        }
    }


}
