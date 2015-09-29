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

extern int uart_enabled;

int a [3] = { 12, 47, 1020 };

void runtime_entry(void) {
//    int i;

//    unsigned int errorepc;
    
//    u_initialize(&errorepc);

//    uart_setup();
//    u_kprint_enable();

//    uart_print("Hello, world!\r\n");

    uart_enabled = 1;

    uart_print("[runtime]\r\n");

    /*
    for (int i = 0 ; i < 3 ; i++) {
        uart_print(tohex(a[i], 8));
    }
    */
 
    init_tasks();

    struct task_attributes attr = { TASK_SIZE_LARGE };
    if (create_task(&___entry, attr)) {
        uart_print("failed to create task\r\n");
    }
    schedule_task();

    /* unreachable */

    uart_print("???\r\n");

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

    TRISBSET = BITS(15) | BITS(14);
    TRISASET = BITS(2) | BITS(3) | BITS(4);

    unsigned int tmp;
/*    asm volatile ("di");
    asm volatile ("ehb");
    asm volatile ("mtc0 %0, $15, $1" : "+r"(ebase_val));*/
    asm volatile ("mfc0 %0, $13" : "=r"(tmp));
    tmp |= 0x00800000;
    asm volatile ("mtc0 %0, $13" : "+r"(tmp));

    INTCONSET = BITS(12);

    while (!(PORTB & BITS(15)));

    CNCONASET = BITS(15);
    CNCONBSET = BITS(15);

    IPC8SET = BITS(19);
    IPC8CLR = BITS(20) | BITS(18);
    IFS1CLR = BITS(13);
    IEC1SET = BITS(13) | BITS(14);

    IPC2SET = BITS(2) | BITS(3);
    IPC2CLR = BITS(4);

    IPC3SET = BITS(2) | BITS(3);
    IPC3CLR = BITS(4);

    IPC4SET = BITS(2) | BITS(3);
    IPC4CLR = BITS(4);

    IPC5SET = BITS(2) | BITS(3);
    IPC5CLR = BITS(4);

    asm volatile ("ei");
    
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

void init_ldr() {
    u_ana_config cfg = u_ana_load_config();
    cfg.on = 1;
    cfg.format = 5;
    cfg.conversion_trigger_source = 7;
    cfg.sample_auto_start = 1;
    cfg.sequences_per_interrupt = 1;
    cfg.auto_sample_time = 31;
    cfg.clock_select = 63;

    u_ana_set_mux(0, 2, 0, 0);

    Pin ldr_pin = {PIN_GROUP_B, BITS(2)};
    pin_mode_set(ldr_pin, 1, 1);

    u_ana_save_config(cfg);
}


#endif

