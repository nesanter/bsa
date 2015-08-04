#include "runtime.h"
#include "driver/console.h"
#include "driver/basic_io.h"
#include "ulib/ulib.h"
#include "ulib/uart.h"
#include "ulib/util.h"
#include "ulib/pins.h"
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

// [manifest] .console               0   0   w,s
// [manifest] .console.tx            0   1   w,v
// [manifest] .console.tx.block      0   2   rw,v
// [manifest] .console.rx.block      0   3   rw,v
// [manifest] .console.rx.ready      0   4   r
// [manifest] .console.rx            0   5   r
// [manifest] .console.tx.ready      0   6   r

const driver_write_fn all_write_fns[] = {
    /* .console */
    &drv_console_write, // 0.0
    &drv_console_raw_write, //0.1
    &drv_console_tx_block_write, // 0.2
    &drv_console_rx_block_write, // 0.3
    0, // 0.4
    0, // 0.5
    0, // 0.6
    &drv_led_write, // 1.0
    &drv_led_select_write, // 1.1
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

    &drv_led_read, // 1.0
    &drv_led_select_read, // 1.1
};

const struct driver drivers[] = {
    { &all_write_fns[0], &all_read_fns[0], 7 },
    { &all_write_fns[7], &all_read_fns[7], 2 }
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
    unsigned int low = target & 0xFFFF;
    unsigned int high = (target & 0xFFFF0000) > 16;

    driver_read_fn fn = drivers[low].read_fns[high];
    if (fn) {
        return fn();
    } else {
        uart_print("oops\r\n");
        return 0;
    }
}

void ___yield_builtin() {
    if (schedule_task())
        scheduler_loop();
}

/* technically, the second argument is a int (*)(struct eh_t *)
 * in reality, it doesn't matter
 */
void ___fork_builtin(struct eh_t *eh, int (*fn)(void*)) {
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
    uart_print("here");
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

Pin selected_led = {PIN_GROUP_B, BITS(4)};

int drv_led_select_write(int val, char *str) {
    switch (val) {
        case 0: selected_led.pin = BITS(4); break;
        case 1: selected_led.pin = BITS(5); break;
        case 2: selected_led.pin = BITS(6); break;
        case 3: selected_led.pin = BITS(7); break;
        case 4: selected_led.pin = BITS(10); break;
        case 5: selected_led.pin = BITS(11); break;
        case 6: selected_led.pin = BITS(12); break;
        case 7: selected_led.pin = BITS(13); break;
        default: break;
    }
    return DRV_SUCCESS;
}

int drv_led_write(int val, char *str) {
    if (val)
        pin_set(selected_led);
    else
        pin_clear(selected_led);
}

int drv_led_select_read() {
    switch (selected_led.pin) {
        case BITS(4): return 0;
        case BITS(5): return 1;
        case BITS(6): return 2;
        case BITS(7): return 3;
        case BITS(10): return 4;
        case BITS(11): return 5;
        case BITS(12): return 6;
        case BITS(13): return 7;
        default: return -1;
    }
}

int drv_led_read() {
    return pin_test(selected_led);
}
