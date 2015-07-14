//#include <sys/attribs.h>

#include "ulib/ulib.h"
#include "ulib/ulib_int.h"
#include "proc/p32mx250f128b.h"

#include "ulib/pins.h"
//#include "uart.h"
int kprint_enabled = 0;

const int u_cs0_request_bit = BITS(8);
const int u_cs1_request_bit = BITS(9);

int syscall_a, syscall_b, syscall_c;

//private functions
void u_cs_setup(void);
/* This is in the bootloader
unsigned int __attribute__((nomips16)) u_int_setup(unsigned int *errorepc) {

    //asm volatile ("di");
    //asm volatile ("ehb");
    

    unsigned int errorepctmp;
    asm volatile ("mfc0 %0, $30" : "=r" (errorepctmp));

    *errorepc = errorepctmp;

    
    void *gp_ptr = &u_exception_context.gp;
    void *sp_ptr = &u_exception_context.sp;
    
    asm volatile ("sw $gp, (%0)" : "+r" (gp_ptr));
    asm volatile ("sw $sp, (%0)" : "+r" (sp_ptr));

    unsigned int val;
    asm volatile ("mfc0 %0, $13" : "=r" (val));
    val |= BITS(23);
    asm volatile ("mtc0 %0, $13" : "+r" (val));

    INTCONSET = _INTCON_MVEC_MASK;

    unsigned int status = 0;
    asm volatile ("ei %0" : "=r"(status));

    u_cs_setup();
    
    return status;
    
}
*/

void u_cs_setup(void) {
    Pin p = {PIN_GROUP_A, BITS(0)};

    pin_mode_set(p, PIN_OUTPUT, PIN_DIGITAL);
    pin_clear(p);

    IPC0CLR = BITS(20) | BITS(19) | BITS(18) | BITS(17) | BITS(16) | BITS(9) | BITS(8);
    IPC0SET = BITS(18) | BITS(12) | BITS(11) | BITS(10);
    
    IEC0SET = BITS(1) | BITS(2);
}


void __attribute__((nomips16)) u_cs_high(void) {
    /*
    unsigned int val;
    asm volatile ("mfc0 %0, $13" : "=r" (val));
    val |= u_cs0_request_bit;
    asm volatile ("mtc0 %0, $13" : "+r" (val));
    */
    IFS0SET = BITS(1);
}

void __attribute__((nomips16)) u_cs_low(void) {
    /*
    unsigned int val;
    asm volatile ("mfc0 %0, $13" : "=r" (val));
    val |= u_cs1_request_bit;
    asm volatile ("mtc0 %0, $13" : "+r" (val));
    */
    IFS0SET = BITS(2);
}

void __attribute__((nomips16)) u_kprint(char *s) {
    if (kprint_enabled) {
        while (*s) {
            if (!(U1STA & BITS(9)))
                U1TXREG = *s++;
        }
    }
}

void __attribute__((nomips16)) u_kprint_hex(unsigned int n, unsigned int length) {
    if (kprint_enabled) {
        char buffer[64];
        char *ptr = buffer + length;
        *ptr-- = '\0';
        while (length--) {
            if ((n & 0xF) > 9)
                *ptr-- = (n & 0xF) - 10 + 'A';
            else
                *ptr-- = (n & 0xF) + '0';
            n >>= 4;
        }

        ptr++;

        while (*ptr) {
            if (!(U1STA & BITS(9)))
                U1TXREG = *ptr++;
        }
    }
}

void u_kprint_enable(void) {
    kprint_enabled = 1;
}
/*

void u_syscall(int a, int b, int c) {
    register v0 asm("v0");
    v0 = a;


    asm volatile ("move $a0, %0; move $a1, %1; move $a2, %2; syscall" : "+r"(a), "+r"(b), "+r"(c));
}
*/

void u_syscall(void) {
    asm volatile("syscall");
}

unsigned int __attribute__((nomips16)) k_syscall_handler(unsigned int epc) {
    return epc + 4;
}

//void __attribute__((nomips16)) _general_exception_handler(int x, int y, int z) {
//void __attribute__((nomips16)) _general_exception_handler(void) {
void __attribute__((nomips16)) u_exception_handler(unsigned int code, unsigned int status, void *epc) {
//    unsigned int code;

//    asm volatile ("mfc0 %0, $13" : "=r"(code));

//    switch ((code & 0x3C) >> 2) {

    u_kprint("\r\nEXCEPTION AT ");
    u_kprint_hex((unsigned int)epc, 8);
    u_kprint(" [");
    u_kprint_hex(code, 2);
    u_kprint("] ");
    switch (code) {
        case 0x00: //interrupt
            u_kprint("(interrupt)\r\n");
            break;
        case 0x04: //address error (load/fetch)
            u_kprint("(address error during load/fetch\r\n");
        case 0x05: //address error (store)
            u_kprint("(address error during store)\r\n");
            break;
        case 0x06: //bus error (fetch)
            u_kprint("(bus error during fetch)\r\n");
            break;
        case 0x07: //bus error (load/store)
            u_kprint("(bus error during load/store)\r\n");
            break;
        case 0x08: //syscall
            u_kprint("(system call)\r\n");
            return;
        case 0x09: //breakpoint
            u_kprint("(breakpoint)\r\n");
            break;
        case 0x0A: //reserved instruction
            u_kprint("(reserved instruction)\r\n");
            break;
        case 0x0B: //coprocessor unusable
            u_kprint("(coprocessor unusable)\r\n");
            break;
        case 0x0C: //arithmetic overflow
            u_kprint("(arithmetic overflow)\r\n");
            break;
        case 0x0D: //trap
            u_kprint("(trap)\r\n");
            break;
        default: //reserved
            u_kprint("(unknown)\r\n");
            break;
    }

    /*
    u_system_unlock();
    u_soft_reset();
    */
    
    while (1);
}

/*
int __attribute__((nomips16)) u_syscall_handler(void) {
//    u_kprint("\r\nhandler\r\n");
}
*/
/*
void __ISR(_CORE_SOFTWARE_1_VECTOR, IPL1SOFT) u_handler_core_software_1(void) {
    if (uart_get_enabled())
        uart_print("\r\n---cs1---\r\n");

    IFS0CLR = BITS(2);
}

void __ISR(_CORE_SOFTWARE_0_VECTOR, IPL7SOFT) u_handler_core_software_0(void) {
    if (uart_get_enabled())
        uart_print("\r\n---cs0---\r\n");

    IFS0CLR = BITS(1);
}
*/
