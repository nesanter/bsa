#include "runtime.h"
#include "driver/console.h"
#include "driver/basic_io.h"
#include "driver/timer.h"
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

extern struct task_info *current_task;

extern handler_t volatile __vector_table[43];

typedef int (*driver_write_fn)(int val, char *str);
typedef int (*driver_read_fn)();
typedef int (*driver_block_fn)();

struct driver {
    const driver_write_fn *write_fns;
    const driver_read_fn *read_fns;
    const driver_block_fn *block_fns;
    unsigned int fn_count;
};

// [manifest] .console               0   0   w,s
// [manifest] .console.tx            0   1   w,v
// [manifest] .console.tx.block      0   2   rw,v
// [manifest] .console.rx.block      0   3   rw,v
// [manifest] .console.rx.ready      0   4   r
// [manifest] .console.rx            0   5   r
// [manifest] .console.tx.ready      0   6   r
// [manifest] .console.rx.wait       0   0   b
// [manifest] .led                   1   0   rw,v
// [manifest] .led.select            1   1   rw,v
// [manifest] .sw                    2   0   r
// [manifest] .sw.select             2   1   rw,v
// [manifest] .sw.edge               2   0   b
// [manifest] .system.delay          3   0   w,v
// [manifest] .timer.tick            4   0   rw,v
// [manifest] .timer.enable          4   1   w,v
// [manifest] .timer.select          4   2   rw,v
// [manifest] .timer.period          4   3   rw,v
// [manifest] .timer.wait            4   0   b

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

    0,
    &drv_sw_select_write, // 2.1

    &drv_sys_delay_write, // 3.0

    &drv_timer_write, // 4.0
    &drv_timer_enable_write, // 4.1
    &drv_timer_select_write, // 4.2
    &drv_timer_period_write, // 4.3
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

    &drv_sw_read, // 2.0
    &drv_sw_select_read, // 2.1

    0, // 3.0

    &drv_timer_read, // 4.0
    0, // 4.1
    &drv_timer_select_read, // 4.2
    &drv_timer_period_read // 4.3
};

const driver_block_fn all_block_fns[] = {
    /* .console */
    &drv_console_rx_block, // 0.0
    
    /* .sw */
    &drv_console_sw_block, // 2.0

    /* .timer */
    &drv_console_timer_block, // 4.0
};

const struct driver drivers[] = {
    { &all_write_fns[0], &all_read_fns[0], &all_block_fns[0], 7 }, /* .console */
    { &all_write_fns[7], &all_read_fns[7], 0, 2 }, /* .led */
    { &all_write_fns[9], &all_read_fns[9], &all_block_fns[1], 2 }, /* .sw */
    { &all_write_fns[11], &all_read_fns[11], 0, 1 }, /* .system */
    { &all_write_fns[12], &all_read_fns[12], &all_block_fns[2], 1}, /* .timer */
};

int ___write_builtin(struct eh_t *eh, unsigned int target, int val, char *str) {
    unsigned int low = target & 0xFFFF;
    unsigned int high = (target & 0xFFFF0000) >> 16;

    driver_write_fn fn = drivers[low].write_fns[high];
    if (fn) {
        return fn(val, str);
    } else {
        return 0;
    }
}

int ___read_builtin(struct eh_t *eh, unsigned int target) {
    unsigned int low = target & 0xFFFF;
    unsigned int high = (target & 0xFFFF0000) >> 16;

    driver_read_fn fn = drivers[low].read_fns[high];
    if (fn) {
        return fn();
    } else {
        return 0;
    }
}

int ___block_builtin(struct eh_t *eh, unsigned int target) {
    unsigned int low = target & 0xFFFF;
    unsigned int high = (target & 0xFFFF0000) >> 16;

    driver_block_fn fn = drivers[low].block_fns[high];
    if (fn) {
        return fn();
    } else {
        return 0;
    }
}

void ___yield_builtin() {
    /*
    void *ra;
    asm volatile ("add %0, $ra, $zero" : "=r"(ra));
    */
    uart_print("[saving old task]\r\n");
    context_save(&current_task->context, &&restore);
    current_task->state = TASK_STATE_SOFT_BLOCKED;
    uart_print("[yielding]\r\n");
    if (schedule_task()) {
        scheduler_loop();
    }
restore:
    return;
}

/* technically, the second argument is a int (*)(struct eh_t *)
 * in reality, it doesn't matter
 */
void ___fork_builtin(struct eh_t *eh, int (*fn)(void*)) {
    struct task_attributes attr  = { TASK_SIZE_LARGE };
    if (create_task(fn, attr)) {
        // error situation
        uart_print("[error during fork]\r\n");
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
        default: return DRV_FAILURE;
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

Pin selected_sw = { PIN_GROUP_A, BITS(2) };

int drv_sw_select_write(int val, char *str) {
    switch (val) {
        case 0: selected_sw.group = PIN_GROUP_A;
                selected_sw.pin = BITS(2);
                break;
        case 1: selected_sw.group = PIN_GROUP_A;
                selected_sw.pin = BITS(3);
                break;
        case 2: selected_sw.group = PIN_GROUP_A;
                selected_sw.pin = BITS(4);
                break;
        case 3: selected_sw.group = PIN_GROUP_B;
                selected_sw.pin = BITS(14);
                break;
        default: return DRV_SUCCESS;
    }
    return DRV_SUCCESS;
}

int drv_sw_select_read() {
    if (selected_sw.group == PIN_GROUP_A) {
        switch (selected_sw.pin) {
            case BITS(2): return 0;
            case BITS(3): return 1;
            case BITS(4): return 2;
            default: return -1;
        }
    } else {
        return 3;
    }
}

int drv_sw_read() {
    return pin_test(selected_sw);
}

int drv_sys_delay_write(int val, char *str) {
    unsigned int start, cur;
    asm volatile ("mfc0 %0, $9" : "=r"(start));
    start += val;

    do {
        asm volatile ("mfc0 %0, $9" : "=r"(cur));
    } while (cur < start);

    return DRV_SUCCESS;
}

u_timerb_select selected_timer;

int drv_timer_select_write(int val, char *str) {
    switch (val) {
        case 0:
            selected_timer = TIMER2;
            break;
        case 1:
            selected_timer = TIMER3;
            break;
        case 2:
            selected_timer = TIMER4;
            break;
        case 3:
            selected_timer = TIMER5;
            break;
        case 4:
            selected_timer = TIMER23;
            break;
        case 5:
            selected_timer = TIMER45;
            break;
        default: return DRV_FAILURE;
    }
    return DRV_SUCCESS;
}

int drv_timer_select_read() {
    switch (selected_timer) {
        case TIMER2: return 0;
        case TIMER3: return 1;
        case TIMER4: return 2;
        case TIMER5: return 3;
        case TIMER23: return 4;
        case TIMER45: return 5;
        default: return -1;
    }
}

int drv_timer_enable_write(int val, char *str) {
    u_timerb_config config = u_timerb_load_config(selected_timer);
    if (val) {
        config.on = 1;
        if (selected_timer > 3) {
            config.t32_enable = 1;
        } else {
            config.t32_enable = 0;
        }
    } else {
        config.on = 0;
    }
    u_timerb_save_config(selected_timer, config);
    return DRV_SUCCESS;
}

int drv_timer_prescaler_write(int val, char *str) {
    if (val < 0 || val > 7)
        return DRV_FAILURE;
    u_timerb_config config = u_timerb_load_config(selected_timer);
    config.prescaler = val;
    u_timerb_save_config(selected_timer, config);
    return DRV_SUCCESS;
}

int drv_timer_prescaler_read() {
    u_timerb_config config = u_timerb_load_config(selected_timer);
    return config.prescaler;
}

int drv_timer_write(int val, char *str) {
    u_timerb_write(selected_timer, val);
    return DRV_SUCCESS;
}

int drv_timer_read() {
    return u_timerb_read(selected_timer);
}

int drv_timer_period_write(int val, char *str) {
    u_timerb_period_write(selected_timer, val);
    return DRV_SUCCESS;
}

int drv_timer_period_read() {
    return u_timerb_period_read(selected_timer);
}

int drv_console_rx_block() {
    
}

int drv_console_sw_block() {
    
}

int drv_console_timer_block() {
    
}

