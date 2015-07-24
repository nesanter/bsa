#include "runtime.h"
#include "driver/console.h"
#include "ulib/ulib.h"
#include "ulib/uart.h"
#include "ulib/util.h"
#include "exception.h"
#include "task.h"

#define DRV_SUCCESS (1)
#define DRV_FAILURE (0)

#define DRV_TRUE (1)
#define DRV_FALSE (0)

extern handler_t volatile __vector_table[43];

typedef int (*driver_write_fn)(int val, char *str);
typedef int (*driver_read_fn)();

struct driver {
    const driver_write_fn *write_fns;
    const driver_read_fn *read_fns;
    unsigned int fn_count;
};

// [manifest] .console               0   0
// [manifest] .console.tx            0   1
// [manifest] .console.tx.block      0   2
// [manifest] .console.rx.block      0   3
// [manifest] .console.rx.ready      0   4
// [manifest] .console.rx            0   5
// [manifest] .console.tx.ready      0   6

const driver_write_fn all_write_fns[] = {
    /* .console */
    &drv_console_write, // 0.0
    &drv_console_raw_write, //0.1
    &drv_console_tx_block_write, // 0.2
    &drv_console_rx_block_write, // 0.3
    0, // 0.4
    0, // 0.5
    0, // 0.6
};

const driver_read_fn all_read_fns[] = {
    /* .console */
    0, // 0
    0, // 0.1
//    &drv_console_tx_ready, // 0.1
    &drv_console_rx_block_read, // 0.2
    &drv_console_rx_block_read, // 0.3
    &drv_console_rx_ready, // 0.4
    &drv_console_rx_read, // 0.5
    &drv_console_tx_ready, // 0.6
};

const struct driver drivers[] = {
    { &all_write_fns[0], &all_read_fns[0], 2 }
};

int ___write_builtin(struct eh_t *eh, unsigned int target, int val, char *str) {
    unsigned short low = target & 0xFFFF;
    unsigned short high = (target & 0xFFFF0000) > 16;

    driver_write_fn fn = drivers[low].write_fns[high];
    if (fn) {
        return fn(val, str);
    } else {
        return 0;
    }
}

int ___read_builtin(struct eh_t *eh, unsigned int target) {
    unsigned short low = target & 0xFFFF;
    unsigned short high = (target & 0xFFFF0000) > 16;

    driver_read_fn fn = drivers[low].read_fns[high];
    if (fn) {
        return fn();
    } else {
        return 0;
    }
}

void ___yield_builtin() {
    if (schedule_task())
        scheduler_loop();
}

void ___fork_builtin(int (*fn)()) {
    struct task_attributes attr  = { TASK_SIZE_LARGE };
    if (create_task(fn, attr)) {
        // error situation
    }
}

void runtime_set_vector_table_entry(unsigned int entry, handler_t handler) {
    __vector_table[entry] = handler;
}

/* driver functions */

int tx_block = 0, rx_block = 0;

int drv_console_write(int val, char *str) {
    if (str)
        uart_print(str);
    else {
        uart_print(todecimal(val));
    }
    return DRV_SUCCESS;
}

int drv_console_raw_write(int val, char *str) {
    if (tx_block) {
        while (uart_tx(val & 0xFF) == UART_TX_FAILURE);
        return DRV_SUCCESS;
    } else {
        if (uart_tx(val & 0xFF) == UART_TX_SUCCESS) {
            return DRV_SUCCESS;
        } else {
            return DRV_FAILURE;
        }
    }
}

int drv_console_tx_block_write(int val, char *str) {
    tx_block = val;
    return DRV_SUCCESS;
}

int drv_console_rx_block_write(int val, char *str) {
    rx_block = val;
    return DRV_SUCCESS;
}

int drv_console_rx_read() {
    char c;
    if (rx_block) {
        while (uart_rx(&c) == UART_RX_FAILURE);
        return (int)c;
    } else {
        int r = uart_rx(&c);
        if (r == UART_RX_SUCCESS) {
            return (int)c;
        } else {
            return -1;
        }
    }
}

int drv_console_tx_ready() {
    if (u_uartx_get_tx_full(UART1)) {
        return DRV_FAILURE;
    } else {
        return DRV_SUCCESS;
    }
}

int drv_console_rx_ready() {
    if (u_uartx_get_rx_available(UART1)) {
        return DRV_SUCCESS;
    } else {
        return DRV_FAILURE;
    }
}

int drv_console_tx_block_read() {
    if (tx_block)
        return DRV_TRUE;
    else
        return DRV_FALSE;
}

int drv_console_rx_block_read() {
    if (rx_block)
        return DRV_TRUE;
    else
        return DRV_FALSE;
}
