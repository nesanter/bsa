/* 
 * File:   ulib_int.h
 * Author: noah
 *
 * Created on November 8, 2014, 11:13 AM
 */

#ifndef HANDLER_H
#define	HANDLER_H

#ifdef IS_BOOTLOADER

typedef struct {
    unsigned int old_gp;
    unsigned int old_sp;
    unsigned int old_fp;
    unsigned int gp;
    unsigned int sp;
} boot_regs;

boot_regs boot_exception_context;

unsigned int __attribute__((nomips16)) boot_handler_setup(unsigned int *errorepc);

void boot_print_enable(void);
void boot_print_disable(void);

//extern int u_syscall(int a, int b, int c);
void boot_syscall(void);
void __attribute__((nomips16)) boot_exception_handler(unsigned int code, unsigned int status, void *epc);

//extern unsigned int __attribute__((nomips16)) boot_syscall_handler(unsigned int epc);

//char u_syscall_stack[0x1000];

//extern void _general_exception_handler(void);

void __attribute__((nomips16)) boot_cs_high(void);
void __attribute__((nomips16)) boot_cs_low(void);

typedef void __attribute__((interrupt)) (*handler_t)(void);

void boot_set_vector_table_entry(unsigned int entry, handler_t handler);

#else
#error "runtime cannot access handler.h"
#endif

#endif	/* HANDLER_H */

