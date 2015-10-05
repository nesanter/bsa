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

struct boot_status * volatile boot_status = (void*)0x9D002C00;

int check_reset_reason(unsigned int epc) {
    unsigned int rcon = RCON;

    if (rcon & REASON_BROWN_OUT) {
        boot_print("Warning: BOR\r\n");
        RCONCLR = REASON_BROWN_OUT;
    }
    if (rcon & REASON_POWER_ON) {
        RCONCLR = REASON_POWER_ON;
    }
    if (rcon & REASON_SOFTWARE) {
        RCONCLR = REASON_SOFTWARE;
    }
    if (rcon & REASON_MCLR) {
        boot_print("MCLR @");
        boot_print(tohex(epc,8));
        boot_print("\r\n");
        RCONCLR = REASON_MCLR;
    }

    return 0;
}

unsigned int signal_counter = 0;

void handler_core_timer() {
    unsigned int tick = SYSTEM_TICK;
    asm volatile ("mtc0 $zero, $9; \
                   mtc0 %0, $11;" : "+r"(tick));
    if (signal_counter == 2000) {
        PORTAINV = SIGNAL_BOOT;
        signal_counter = 0;
    } else {
        signal_counter++;
    }
    IFS0CLR = 0x1;
}

int main(void) {
//    flash_write_word_unsafe(boot_flags & 0x40, physical_address(&boot_flags));

    TRISA = 0;
    TRISB = 0;
    ANSELA = 0;
    ANSELB = 0;
    PORTA = 0;
    PORTB = 0;

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

    // enable multi-vector interrupts
    unsigned int tmp;
    asm volatile ("mfc0 %0, $13" : "=r"(tmp));
    tmp |= 0x00800000;
    asm volatile ("mtc0 %0, $13" : "+r"(tmp));

    INTCONSET = 0x1000;

    asm volatile ("ei");

    boot_print("program loaded:");
    int has_program;

    if (boot_status->runtime_entry != (void*)0xFFFFFFFF && boot_status->runtime_entry != (void*)0) {
        boot_print("yes\r\n");
        has_program = 1;
    } else {
        boot_print("no\r\n");
        has_program = 0;
    }

    TRISBSET = 0x8000;

    char buffer [16];
    unsigned int n = 0;

    boot_set_vector_table_entry(_CORE_TIMER_VECTOR, &handler_core_timer);
    unsigned int tick = SYSTEM_TICK;
    asm volatile ("mtc0 $zero, $9; \
                   mtc0 %0, $11;" : "+r"(tick));

    IPC0SET = 0x1C;
    IEC0SET = 0x1;

    while (1) {
        if ((n += boot_read_nonblocking(&buffer[n], 16 - n)) > 0) {
            if (n == 1 && buffer[0] == '?') {
                boot_print("OK");
                n = 0;
            }
            if (n == 3) {
                buffer[n] = '\0';
                if (strcmpn(buffer, "RUN", 3)) {
                    if (has_program) {
                        boot_print("OK");
                        run();
                    } else {
                        boot_print("NO");
                        n = 0;
                    }
                } else if (strcmpn(buffer, "MAP", 3)) {
                    if (has_program) {
                        boot_print("map @ ");
                        boot_print(tohex((unsigned int)boot_status->init_map, 8));
                        boot_print("\r\ndata @ ");
                        boot_print(tohex((unsigned int)boot_status->init_data, 8));
                        boot_print("\r\n");
                        dump_map();
                    } else {
                        boot_print("NO");
                    }
                    n = 0;
                }
            }
            /*
            if (n == 4) {
                buffer[n] = '\0';
                if (strcmpn(buffer, "LOAD", 4)) {
                    boot_print("OK");
                    load();
                    n = 0;
                }
            }
            */
            if (n >= 12) {
                buffer[n] = '\0';
                if (strcmpn(buffer, "BSA PREAMBLE", 12)) {
                    IEC0CLR = 0x1;
                    boot_print("OK");
                    preamble();
                    n = 0;
                    if (boot_expect("LOAD")) {
                        IEC0SET = 0x1;
                        break;
                    }
                    boot_print("OK");
                    load();
                }
            }
            if (n == 16)
                n = 0;
        }
        /*
        if (boot_flag & BOOT_FLAG_LOADED) {
            if (boot_run_read()) {
                run();
            }
        }
        */
        if (boot_run_read() && has_program) {
            run();
        }
        asm volatile ("wait");
    }
}


