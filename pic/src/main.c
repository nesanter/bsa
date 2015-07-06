#include "ulib/uart.h"
#include "ulib/pins.h"
#include "ulib/ulib.h"
#include "ulib/ulib_int.h"
#include "ulib/util.h"

#include <sys/attribs.h>
#include "proc/p32mx250f128b.h"


void main(void) {
    int i;

    unsigned int errorepc;
    
    u_initialize(&errorepc);

    Pin p = {PIN_GROUP_A, BITS(0)};
    pin_mode_set(p, PIN_OUTPUT, PIN_DIGITAL);

    uart_setup();
    u_kprint_enable();
    

    uart_print("hello, world!\r\n");
    
    uart_print(tohex(errorepc, 8));
    uart_print("\r\n");

    while (!u_uartx_get_tx_shift_empty(UART1));


    uart_print("\r\ndone.");

    while (1);
}


