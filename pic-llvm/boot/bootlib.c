#define IS_BOOTLOADER

#include "boot/bootlib.h"
#include "proc/p32mx250f128b.h"

/*  The bootloader enables UART1:
 *  9600 baud 81N, no interrupts
 *  RX on B2, TX on B3 (physical pins 6 & 7)
 */

void boot_uart_init() {
    // pin mode
    int trisb = TRISB;
    int anselb = ANSELB;
    trisb |= 0x00000004;
    trisb &= 0xFFFFFFF7;
    anselb &= 0xFFFFFFF3;

    TRISB = trisb;
    ANSELB = anselb;

    // pps
    U1RXR = 0x5; // RX
    RPB34 = 0x1; // TX

    // baud
    unsigned int baud = ((SYSTEM_FREQ / 9600) / 16) - 1;
    U1BRG = baud;

    // uart1 setup
    int cur_mode, cur_status;
    cur_mode = U1MODE;
    cur_status = U1STA;

    cur_mode |= 0x00008000; // on = 1
    cur_mode &= 0xFFFFFFF9; // parity_data_mode = 0
    cur_mode &= 0xFFFFFFFD; // stop_mode = 0
    
    cur_status |= 0x00001400; // rx/tx enable = 1
    cur_status &= 0xFFFFFF3F; // int_mode = 0
    
    U1STA = cur_status;
    U1MODE = cur_mode;
}

void boot_uart_print(char *s) {
    while (*s) {
        while (!(U1STA & 0x200)) {
            U1TXREG = *s++;
        }
    }
}
