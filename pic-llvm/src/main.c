#include "ulib/uart.h"
#include "ulib/pins.h"
#include "ulib/ulib.h"
//#include "ulib/ulib_int.h"
#include "ulib/util.h"
#include "exception.h"
#include "task.h"

#include "proc/processor.h"

#ifdef HARD_RUNTIME
#define IS_BOOTLOADER
#include "boot/config.h"
#include "boot/handler.h"
#undef IS_BOOTLOADER
#endif

//#include <sys/attribs.h>
//#include "proc/p32mx250f128b.h"

int ___entry(void *);

void runtime_entry(void) {
//    int i;

//    unsigned int errorepc;
    
//    u_initialize(&errorepc);

//    uart_setup();
//    u_kprint_enable();

//    uart_print("Hello, world!\r\n");

    struct task_attributes attr = { TASK_SIZE_LARGE };
    create_task(&___entry, attr);
    schedule_task();

    /* unreachable */

    while (1);
}

#ifdef HARD_RUNTIME
int main(void) {
    //perform setup normally done by bootloader
    TRISA = 0;
    TRISB = 0;
    ANSELA = 0;
    ANSELB = 0;
    PORTA = 0;
    PORTB = 0;
    uart_setup();
    unsigned int ignore;
    boot_print_enable();
    boot_handler_setup(&ignore);
    uart_print("[booting hard runtime]\r\n");
    
    // not needed, OSCCON<4> defaults to cleared
    /*
    unsigned int * nvmkeyaddr = &NVMKEY;

    unsigned int int_status;
    asm volatile ("di %0" : "=r" (int_status));

    asm volatile ("lui $8, 0xAA99\n"
                  "ori $8, 0x6655\n"
                  "lui $9, 0x5566\n"
                  "ori $9, 0x99AA\n"
                  "sw $8, (%0)\n"
                  "sw $9, (%0)\n" : "+r"(nvmkeyaddr));


    *nvmkeyaddr = 0x33333333;

    if (int_status & 0x00000001)
        asm volatile ("ei");
    else
        asm volatile ("di");
    */

    runtime_entry();
}
#endif

