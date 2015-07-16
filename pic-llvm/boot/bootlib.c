#define IS_BOOTLOADER

#include "boot/bootlib.h"
#include "proc/p32mx250f128b.h"
#include "boot/handler.h"
#include "version.h"

#define VIRT_OFFSET (0xA0000000)
#define HANDLER_VECTOR_SECTION ".vector_31"

int transfer_ready = 0;

/*  The bootloader enables UART1:
 *  9600 baud 81N, no interrupts
 *  RX on B2, TX on B3 (physical pins 6 & 7)
 *
 *  And signals BOOT (A0) and USER (A1)
 */

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
    /*
    unsigned int bmxcon = BMXCON;
    bmxcon = (bmxcon & 0xFFFFFFFC) | 0x00000002; // set arbitration mode to rotating priority
    BMXCON = bmxcon;
    */

    DMACONSET = 0x00008000; // DMA on

    DCH1INT = 0x00080000; // Interrupt on block transfer complete
    DCH1SSA = (unsigned int)((&U1RXR) - VIRT_OFFSET); // Channel 1 source is UART1
    DCH1DSA = (unsigned int)((&transfer_buffer) - VIRT_OFFSET); // Channel 1 dest is transfer_buffer
    DCH1SSIZ = 1; // Source size is 1 byte
    DCH1DSIZ = TRANSFER_BUFFER_SIZE_WITH_HEADER; // Dest size is SIZE bytes
    DCH1CSIZ = 1; // Cell size is 1 byte

    DCRCCON = 0x0001F80; // CRC in LFSR mode, 32-bit polynomial
    DCRCDATA = 0; // 0 seed
    DCRCXOR = 0; // disable xor

    DCH1CON = 0; // no pattern, no chain, not enabled, no auto, single-shot, priority 0
    DCH1ECON = (_UART1_RX_IRQ << 8) & 0x8; // start transfer on RX interrupt
}

void boot_transfer_enable() {
    DCH1DSA = (unsigned int)((&transfer_buffer) - VIRT_OFFSET);
    DCH1CONSET = 0x80;
}

unsigned int boot_get_crc() {
    return DCRCDATA;
}

void boot_print(char *s) {
    while (*s) {
        while (!(U1STA & 0x200)) {
            U1TXREG = *s++;
        }
    }
}

void boot_read_blocking(char *buffer, unsigned int length) {
    while (length) {
        while ((U1STA & 0x1)) {
            *buffer++ = U1RXREG;
            length--;
        }
    }
}

void __attribute((interrupt(IPL2SOFT), nomips16)) boot_transfer_handler() {
    transfer_ready = 1;

    DCH1INTCLR = 0x4;
}

unsigned int flash_unlock_write_row(unsigned int row_addr) {
    return 0;
}

unsigned int flash_unlock_erase(unsigned int page_addr) {
    return 0;
}

void boot_signal_init() {
    TRISACLR = 0x3;
    ANSELACLR = 0x3;
}

void boot_signal_set(boot_signal sig, unsigned int on) {
    if (on)
        PORTASET = 0x1;
    else
        PORTACLR = 0x1;
}

void boot_internal_error() {
    PORTACLR = SIGNAL_USER | SIGNAL_BOOT;
    
    int phase = 0, phases = 5;
    unsigned int delay[5] = { 10000000, 1000000, 1000000, 1000000, 30000000 };

    unsigned int count, current, overflow;
    asm volatile ("mfc0 %0, $9" : "=r" (count));
    while (1) {
        if (count + delay[phase] < count) {
            overflow = 1;
        } else {
            overflow = 0;
        }
        count += delay[phase];
        do {
            asm volatile ("mfc0 %0, $9" : "=r" (current));
        } while ((overflow && (current > count) || (!overflow && current < count)));
        PORTAINV = SIGNAL_BOOT;
        phase++;
    }
 
}
