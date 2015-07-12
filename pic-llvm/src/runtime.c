#include "runtime.h"
#include "driver/console.h"
#include "ulib/ulib.h"
#include "ulib/uart.h"
#include "ulib/util.h"

#define DRV_SUCCESS (1)
#define DRV_FAILURE (0)

typedef int (*driver_write_fn)(int val, char *str);
typedef int (*driver_read_fn)();

struct driver {
    const driver_write_fn *write_fns;
    const driver_read_fn *read_fns;
    unsigned int fn_count;
};


const driver_write_fn all_write_fns[] = {
    /* .console */
    &drv_console_write, // 0
};

const driver_read_fn all_read_fns[] = {
    /* .console */
    &drv_console_read, // 0
};

const struct driver drivers[] = {
    { &all_write_fns[0], &all_read_fns[0], 1 }
};

int ___write_builtin(unsigned int target, int val, char *str) {
    unsigned short low = target & 0xFFFF;
    unsigned short high = (target & 0xFFFF0000) > 16;

    return drivers[low].write_fns[high](val, str);
}

int ___read_builtin(unsigned int target) {
    unsigned short low = target & 0xFFFF;
    unsigned short high = (target & 0xFFFF0000) > 16;
    return drivers[low].read_fns[high]();
}

int ___yield_builtin(void) {
    return 0;
}

int ___fork_builtin(int (*fn)()) {
    return 0;
}


/* driver functions */

int drv_console_write(int val, char *str) {
    if (str)
        uart_print(str);
    else
        uart_print(todecimal(val));
    return DRV_SUCCESS;
}

int drv_console_read() {
    char c;
    int r = uart_rx(&c);
    if (r == UART_RX_SUCCESS) {
        return (int)c;
    } else {
        return -1;
    }
}

