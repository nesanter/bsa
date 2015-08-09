#define IS_BOOTLOADER

#include "boot/bootlib.h"
#include "proc/p32mx250f128b.h"
#include "boot/handler.h"
#include "version.h"

#define VIRT_OFFSET (0xA0000000)

int transfer_ready = 0;
unsigned char __attribute__((section(".transfer_buffer"))) transfer_buffer[TRANSFER_BUFFER_SIZE_WITH_HEADER];

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
    U1RXR = 0x4; // RX
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

void __attribute((interrupt(IPL2SOFT), nomips16)) boot_transfer_handler() {
    transfer_ready = 1;

    DCH1INTCLR = 0x4;
}

void boot_transfer_init() {
    /*
    unsigned int bmxcon = BMXCON;
    bmxcon = (bmxcon & 0xFFFFFFFC) | 0x00000002; // set arbitration mode to rotating priority
    BMXCON = bmxcon;
    */
    
    boot_set_vector_table_entry(31, &boot_transfer_handler);

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

int boot_transfer_seek(unsigned int seek) {
    boot_print("SEEK");
    while (U1STA & 0x200);
    U1TXREG = (seek & 0xFF00) >> 8;
    while (U1STA & 0x200);
    U1TXREG = seek & 0xFF;
    while (!(U1STA & 0x1));
    return (U1RXREG != 0x06); // ACK
}

void boot_transfer_read() {
    boot_transfer_enable();
    boot_print("READ");
    while (U1STA & 0x200);
    U1TXREG = (TRANSFER_BUFFER_SIZE & 0xFF00) >> 8;
    while (U1STA & 0x200);
    U1TXREG = TRANSFER_BUFFER_SIZE & 0xFF;
}

void boot_transfer_shutdown() {
    boot_set_vector_table_entry(31, 0);
}

unsigned int boot_get_crc() {
    return DCRCDATA;
}

void boot_print(char *s) {
    while (*s) {
        while (!(U1STA & 0x200) && *s) {
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


unsigned int __attribute__((nomips16)) flash_unlock(nvm_op op) {
    unsigned int int_status;
    asm volatile ("di %0" : "=r" (int_status));

    NVMCON = op;

//    NVMCONSET = 0x00040000;
    
    unsigned int volatile * nvmkeyaddr = &NVMKEY;

    unsigned int t0, t1;
    asm volatile ("mfc0 %0, $9" : "=r"(t0));
    t0 += 150;
    do {
        asm volatile ("mfc0 %0, $9" : "=r"(t1));
    } while (t1 < t0);

    asm volatile ("lui $8, 0xAA99\n"
                  "ori $8, 0x6655\n"
                  "lui $9, 0x5566\n"
                  "ori $9, 0x99AA\n"
                  "sw $8, (%0)\n"
                  "sw $9, (%0)\n" : "+r"(nvmkeyaddr));


//    NVMKEY = UNLOCK_KEY_A;
//    NVMKEY = UNLOCK_KEY_B;

    NVMCONSET = 0x00008000;

    while (NVMCON & 0x00008000);

    if (int_status & 0x00000001)
        asm volatile ("ei");
    else
        asm volatile ("di");

    NVMCONCLR = 0x00004000;

    return (NVMCON & 0x00003000);
}

/*
unsigned int flash_write_word_unsafe(unsigned int value, unsigned int dest_addr) {
    if (dest_addr & 0x3) {
        return 1;
    }

    NVMDATA = value;
    NVMADDR = dest_addr;

    return flash_unlock(NVM_WRITE_WORD);
}
*/

unsigned int flash_write_word(unsigned int value, unsigned int dest_addr) {
    if (dest_addr < 0x1002400 || dest_addr >= 0x1DF000 || dest_addr & 0x3) {
        return 1;
    }
    NVMDATA = value;
    NVMADDR = dest_addr;

    return flash_unlock(NVM_WRITE_WORD);
}

unsigned int flash_write_row(unsigned int src_addr, unsigned int dest_addr) {
    if (src_addr >= 0x8000 || src_addr & 0x3) {
        return 1;
    }
    if (dest_addr < 0x1D002400 || dest_addr >= 0x1D1F000 || dest_addr & 0x1FF) {
        return 1;
    }
    
    NVMADDR = dest_addr;
    NVMSRCADDR = src_addr;

    return flash_unlock(NVM_WRITE_ROW);
}

unsigned int flash_page_erase(unsigned int page_addr) {
    if (page_addr < 0x1D002400 || page_addr >= 0x1DF000 || page_addr & 0xFFF) {
        return 1;
    }
    NVMADDR = page_addr;

    return flash_unlock(NVM_PAGE_ERASE);
}

void boot_signal_init() {
    TRISACLR = 0x3;
    ANSELACLR = 0x3;
}

void boot_signal_set(boot_signal sig, unsigned int on) {
    if (on)
        PORTASET = sig;
    else
        PORTACLR = sig;
}

void boot_internal_error(int single) {
    PORTACLR = SIGNAL_USER | SIGNAL_BOOT;
    
    int phase = 0, phases = 5;
    unsigned int delay[5] = { 1000000, 1000000, 1000000, 1000000, 3000000 };

    unsigned int count;
    asm volatile ("mtc0 $zero, $9");
    while (1) {
        do {
            asm volatile ("mfc0 %0, $9" : "=r" (count));
        } while (count < delay[phase]);
        asm volatile ("mtc0 $zero, $9");
        PORTAINV = SIGNAL_BOOT;
        phase++;
        if (phase == phases) {
            if (single) {
                return;
            }
            phase = 0;
        }
    }
 
}

unsigned int physical_address(void * const virt) {
    if (virt >= 0xBF800000)
        return (unsigned int)(virt - 0xA0000000); //sfrs
    if (virt >= 0xBD000000)
        return (unsigned int)(virt - 0xA0000000); //kseg1
    if (virt >= 0xA0000000)
        return (unsigned int)(virt - 0xA0000000); //ram
    if (virt >= 0x9D000000)
        return (unsigned int)(virt - 0x80000000); //kseg0
    return 0; //error
}

void soft_reset() {
    asm volatile ("di");
    DMACONSET = 0x00001000; //suspend any DMA

    SYSKEY = 0x00000000;
    SYSKEY = UNLOCK_KEY_A;
    SYSKEY = UNLOCK_KEY_B;

    RSWRSTSET = 0x1;

    unsigned int volatile dummy;
    dummy = RSWRSTSET;

    while (1);
}

