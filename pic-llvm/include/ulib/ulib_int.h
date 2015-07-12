/* 
 * File:   ulib_int.h
 * Author: noah
 *
 * Created on November 8, 2014, 11:13 AM
 */

#ifndef ULIB_INT_H
#define	ULIB_INT_H

typedef struct {
    unsigned int old_gp;
    unsigned int old_sp;
    unsigned int old_fp;
    unsigned int gp;
    unsigned int sp;
} u_regs;

extern u_regs u_exception_context;

unsigned int __attribute__((nomips16)) u_int_setup(unsigned int *errorepc);

void __attribute__((nomips16)) u_kprint(char *s);
void __attribute__((nomips16)) u_kprint_hex(unsigned int n, unsigned int length);
void u_kprint_enable(void);

//extern int u_syscall(int a, int b, int c);
void u_syscall(void);
void __attribute__((nomips16)) u_exception_handler(unsigned int code, unsigned int status, void *epc);

extern unsigned int __attribute__((nomips16)) k_syscall_handler(unsigned int epc);

//char u_syscall_stack[0x1000];

//extern void _general_exception_handler(void);

void __attribute__((nomips16)) u_cs_high(void);
void __attribute__((nomips16)) u_cs_low(void);

#endif	/* ULIB_INT_H */

