//#include <sys/attribs.h>

#define IS_BOOTLOADER

#include "proc/p32mx250f128b.h"
#include "boot/handler.h"
#include "boot/bootlib.h"
#include "ulib/util.h"

handler_t __attribute__((section(".vector_table"))) __vector_table[43];

//#include "uart.h"
int boot_print_enabled = 0;

unsigned int handler_old_sp;
unsigned char handler_stack[32];

//private functions
void boot_cs_setup(void);
unsigned int __attribute__((nomips16)) boot_handler_setup(unsigned int *errorepc) {

    //asm volatile ("di");
    //asm volatile ("ehb");
    

    unsigned int errorepctmp;
    asm volatile ("mfc0 %0, $30" : "=r" (errorepctmp));

    *errorepc = errorepctmp;

    
    void *gp_ptr = &boot_exception_context.gp;
    void *sp_ptr = &boot_exception_context.sp;
    
    asm volatile ("sw $gp, (%0)" : "+r" (gp_ptr));
    asm volatile ("sw $sp, (%0)" : "+r" (sp_ptr));

    unsigned int val;
    asm volatile ("mfc0 %0, $13" : "=r" (val));
    val |= 0x00080000;
    asm volatile ("mtc0 %0, $13" : "+r" (val));

    INTCONSET = 0x0000100;

    unsigned int status = 0;
    asm volatile ("ei %0" : "=r"(status));

    boot_cs_setup();
    
    return status;
    
}

void boot_cs_setup(void) {
    IPC0CLR = 0x001F0030;
    IPC0SET = 0x00021C00;
    
    IEC0SET = 0x00000006;
}

void boot_set_vector_table_entry(unsigned int entry, handler_t handler) {
    __vector_table[entry] = handler;
}

void __attribute__((nomips16)) boot_cs_high(void) {
    IFS0SET = 0x00000002;
}

void __attribute__((nomips16)) boot_cs_low(void) {
    IFS0SET = 0x00000004;
}

void boot_print_enable(void) {
    boot_print_enabled = 1;
}

void boot_syscall(void) {
    asm volatile("syscall");
}

unsigned int __attribute__((nomips16)) boot_syscall_handler(unsigned int epc) {
    return epc + 4;
}

//void __attribute__((nomips16)) _general_exception_handler(int x, int y, int z) {
//void __attribute__((nomips16)) _general_exception_handler(void) {
void __attribute__((nomips16)) boot_exception_handler(unsigned int code, unsigned int status, void *epc) {
//    unsigned int code;

//    asm volatile ("mfc0 %0, $13" : "=r"(code));

//    switch ((code & 0x3C) >> 2) {

    boot_print("\r\nEXCEPTION AT ");
    boot_print(tohex((unsigned int)epc, 8));
    boot_print(" [");
    boot_print(tohex(code, 2));
    boot_print("] ");
    switch (code) {
        case 0x00: //interrupt
            boot_print("(interrupt)\r\n");
            break;
        case 0x04: //address error (load/fetch)
            boot_print("(address error during load/fetch\r\n");
        case 0x05: //address error (store)
            boot_print("(address error during store)\r\n");
            break;
        case 0x06: //bus error (fetch)
            boot_print("(bus error during fetch)\r\n");
            break;
        case 0x07: //bus error (load/store)
            boot_print("(bus error during load/store)\r\n");
            break;
        case 0x08: //syscall
            boot_print("(system call)\r\n");
            return;
        case 0x09: //breakpoint
            boot_print("(breakpoint)\r\n");
            break;
        case 0x0A: //reserved instruction
            boot_print("(reserved instruction)\r\n");
            break;
        case 0x0B: //coprocessor unusable
            boot_print("(coprocessor unusable)\r\n");
            break;
        case 0x0C: //arithmetic overflow
            boot_print("(arithmetic overflow)\r\n");
            break;
        case 0x0D: //trap
            boot_print("(trap)\r\n");
            break;
        default: //reserved
            boot_print("(unknown)\r\n");
            break;
    }

    /*
    if (boot_auto_reset) {
        boot_system_unlock();
        boot_soft_reset();
    }
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
