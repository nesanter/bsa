#define IS_BOOTLOADER

#include "boot/bootlib.h"
#include "proc/p32mx250f128b.h"
#include "boot/handler.h"
#include "version.h"

/*  The bootloader enables UART1:
 *  9600 baud 81N, no interrupts
 *  RX on B2, TX on B3 (physical pins 6 & 7)
 */

int active_buffer = 1;

void boot_uart_init() {
    // pin mode
    unsigned int trisb = TRISB;
    unsigned int anselb = ANSELB;
    trisb |= 0x00000004;
    trisb &= 0xFFFFFFF7;
    anselb &= 0xFFFFFFF3;

    TRISB = trisb;
    ANSELB = anselb;

    // pps
    U1RXR = 0x5; // RX
    RPB3R = 0x1; // TX

    // baud
    unsigned int baud = ((SYSTEM_FREQ / 9600) / 16) - 1;
    U1BRG = baud;

    // uart1 setup
    unsigned int cur_mode, cur_status;
    cur_mode = U1MODE;
    cur_status = U1STA;

    cur_mode |= 0x00008000; // on = 1
    cur_mode &= 0xFFFFFFF9; // parity_data_mode = 0
    cur_mode &= 0xFFFFFFFD; // stop_mode = 0
    
    cur_status |= 0x00001400; // rx/tx enable = 1
    cur_status &= 0xFFFFFF3F; // int_mode = 0
    
    U1STA = cur_status;
    U1MODE = cur_mode;

    boot_print_enable();
}

void boot_transfer_init() {
    unsigned int dmacon = DMACON;
    unsigned int dch1con = DCH1CON;
    unsigned int dch1econ = DCH1ECON;
 
    DMACONSET = 0x00008000; // DMA on

    DCH1INT = 0x00080000; // Interrupt on block transfer complete
    DCH1SSA = (&U1RXR) - 0xA0000000; // Channel 1 source is UART1
    DCH1DSA = (&transfer_buffer) - 0xA0000000; // Channel 1 dest is transfer_buffer
    DCH1SSIZ = 1; // Source size is 1 byte
    DCH1DSIZ = TRANSFER_BUFFER_SIZE; // Dest size is SIZE bytes
    DCH1CSIZ = 1; // Cell size is 1 byte

    DCRCCON = 0x0001F80; // CRC in LFSR mode, 32-bit polynomial
    DCRCDATA = 0; // 0 seed
    DCRCXOR = 0; // disable xor

    DCH1CON = 0; // no pattern, no chain, not enabled, no auto, single-shot, priority 0
    DCH1ECON = (_UART1_RX_IRQ << 8) & 0x8; // start transfer on RX interrupt
}

void boot_transfer_enable(int buffern) {
    if (buffern == 1) {
        active_buffer = 1;
        DCH1DSA = (&transfer_buffer_1) - 0xA0000000;
    } else {
        active_buffer = 2;
        DCH1DSA = (&transfer_buffer_2) - 0xA0000000;
    }
    DCH1CONSET = 0x80;
}

void boot_print(char *s) {
    while (*s) {
        while (!(U1STA & 0x200)) {
            U1TXREG = *s++;
        }
    }
}

void __ISR(_DMA_0_VECTOR, ipl2soft) transfer_handler() {
    if (active_buffer == 1) {
        transfer_ready |= 1;
        active_buffer = 2;
    } else {
        transfer_ready |= 2;
        active_buffer = 1;
    }

    DCH1INTCLR = 0x4;
}
