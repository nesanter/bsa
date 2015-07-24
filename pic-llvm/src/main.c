#include "ulib/uart.h"
#include "ulib/pins.h"
#include "ulib/ulib.h"
//#include "ulib/ulib_int.h"
#include "ulib/util.h"
#include "exception.h"

//#include <sys/attribs.h>
//#include "proc/p32mx250f128b.h"

int ___entry(struct eh_t *eh);

void runtime_entry(void) {
//    int i;

//    unsigned int errorepc;
    
//    u_initialize(&errorepc);

//    uart_setup();
//    u_kprint_enable();

    uart_print("Hello, world!\r\n");

    struct eh_t eh = { 0, 0 };

    ___entry(&eh);

    uart_print("done!\r\n");

    while (1);
}

