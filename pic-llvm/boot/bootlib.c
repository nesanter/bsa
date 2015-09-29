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
    unsigned int baud = ((SYSTEM_FREQ / DEFAULT_BAUD) / 16) - 1;
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

void boot_uart_change_baud(unsigned int baud) {
//    boot_print_disable();
//    U1MODECLR = 0x00008000;
//
//    unsigned int count;
//    asm volatile ("mtc0 $zero, $9");
//    do {
//        asm volatile ("mfc0 %0, $9" : "=r" (count));
//    } while (count < 40000000);

    U1BRG = ((SYSTEM_FREQ / baud) / 16) - 1;

//    U1MODESET = 0x00008000;

//    boot_print_enable();
}

void boot_print_flush() {
    while (!(U1STA & 0x100));
}

void boot_print(char *s) {
    while (*s) {
        while (!(U1STA & 0x200) && *s) {
            U1TXREG = *s++;
        }
    }
}

void boot_print_n(char *s, unsigned int len) {
    while (len) {
        while (!(U1STA & 0x200) && len) {
            U1TXREG = s[len++];
        }
    }
}

void boot_print_int(int n) {
    for (int i = 0 ; i < 4; i++) {
        while (U1STA & 0x200);
        U1TXREG = (n & 0xFF000000) >> 24;
        n <<= 8;
    }
}

int boot_expect(char *s) {
    while (*s) {
        while (U1STA & 0x1) {
            if (*s++ != U1RXREG)
                return 1;
        }
    }
    return 0;
}

void boot_read_blocking(char *buffer, unsigned int length) {
    while (length) {
        while ((U1STA & 0x1)) {
            *buffer++ = U1RXREG;
            length--;
        }
    }
}

unsigned int boot_read_nonblocking(char *buffer, unsigned int length) {
    unsigned int n = 0;
    while ((n < length) && (U1STA & 0x1)) {
        *buffer++ = U1RXREG;
        n++;
    }
    return n;
}

unsigned int __attribute__((nomips16)) flash_unlock(nvm_op op) {
    unsigned int int_status;
    asm volatile ("di %0" : "=r" (int_status));

    NVMCON = op;
    NVMCONSET = 0x4000;
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
    if (dest_addr < 0x1D002400 || dest_addr >= 0x1D040000 || dest_addr & 0x3) {
        return 1;
    }
    NVMDATA = value;
    NVMADDR = dest_addr;

    return flash_unlock(NVM_WRITE_WORD);
}

unsigned int flash_write_row(unsigned int src_addr, unsigned int dest_addr) {
    if (/*src_addr >= 0x8000 || */src_addr & 0x3) {
        return 1;
    }
    if (dest_addr < 0x1D002400 || dest_addr >= 0x1D040000 || dest_addr & ROW_MASK) {
        return 1;
    }
    
    NVMADDR = dest_addr;
    NVMSRCADDR = src_addr;

    return flash_unlock(NVM_WRITE_ROW);
}

unsigned int flash_page_erase(unsigned int page_addr) {
    if (page_addr < 0x1D002400 || page_addr >= 0x1D040000 || page_addr & PAGE_MASK) {
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

int boot_run_read() {
    return PORTB & 0x8000;
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
    if (virt >= (void*)0xBF800000)
        return (unsigned int)(virt - (void*)0xA0000000); //sfrs
    if (virt >= (void*)0xBD000000)
        return (unsigned int)(virt - (void*)0xA0000000); //kseg1
    if (virt >= (void*)0xA0000000)
        return (unsigned int)(virt - (void*)0xA0000000); //ram
    if (virt >= (void*)0x9D000000)
        return (unsigned int)(virt - (void*)0x80000000); //kseg0
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

void __attribute((interrupt(IPL2SOFT), nomips16)) boot_timeout_handler() {
    boot_internal_error(1);
    soft_reset();
}

// timeout = ~60 seconds
void boot_timeout_init() {
    int tcon = T2CON;
    tcon |= 0x00000078; // PRESCALER = 256, 32-bit
    
    boot_set_vector_table_entry(12, &boot_timeout_handler);

    PR3 = (4687500 & 0xFFFF0000) >> 16;
    PR2 = (4687500 & 0x0000FFFF);

    IPC3SET = 0x00000008;
    IPC3CLR = 0x00000017;
    IFS0CLR = 0x00004000;
    IEC0SET = 0x00040000;

    T2CON = tcon;
}

void boot_timeout_start() {
    T2CONSET = 0x00008000; // ON
}

void boot_timeout_stop() {
    T2CONCLR = 0x00008000; // OFF
}

void boot_timeout_reset() {
    TMR2 = TMR3 = 0;
}

