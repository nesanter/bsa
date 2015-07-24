#include "ulib/uart.h"
#include "ulib/pins.h"
#include "ulib/ulib.h"
//#include "ulib/ulib_int.h"
#include "ulib/util.h"
#include "exception.h"
#include "task.h"

//#include <sys/attribs.h>
//#include "proc/p32mx250f128b.h"

int ___entry(void *);

void runtime_entry(void) {
//    int i;

//    unsigned int errorepc;
    
//    u_initialize(&errorepc);

//    uart_setup();
//    u_kprint_enable();

    uart_print("Hello, world!\r\n");

    struct task_attributes attr = { TASK_SIZE_LARGE };
    create_task(&___entry, attr);
    schedule_task();

    /* unreachable */

    while (1);
}

