#include "runtime.h"
#include "driver/console.h"
#include "driver/basic_io.h"
#include "driver/timer.h"
#include "driver/system.h"
#include "ulib/ulib.h"
#include "ulib/uart.h"
#include "ulib/util.h"
#include "ulib/pins.h"
#include "exception.h"
#include "task.h"
#include "proc/processor.h"

#define DRV_SUCCESS (1)
#define DRV_FAILURE (0)

#define DRV_TRUE (1)
#define DRV_FALSE (0)

const Pin STATUS_LED_PIN = { PIN_GROUP_A, BITS(1) };

extern struct task_info *current_task;

extern handler_t volatile __vector_table[43];

struct driver {
    const driver_write_fn *write_fns;
    const driver_read_fn *read_fns;
    const driver_block_fn *block_fns;
    unsigned int fn_count;
};

// [manifest] .console               0   0   wB,s
// [manifest] .console.tx            0   1   wB,v
// [manifest] .console.tx.block      0   2   rwB,vB
// [manifest] .console.rx.block      0   3   rwB,vB
// [manifest] .console.rx.ready      0   4   rB
// [manifest] .console.rx            0   5   r
// [manifest] .console.tx.ready      0   6   rB
// [manifest] .console.rx.wait       0   0   b
// [manifest] .led                   1   0   rw,v
// [manifest] .led.select            1   1   rw,v
// [manifest] .sw                    2   0   r
// [manifest] .sw.select             2   1   rw,v
// [manifest] .sw.edge               2   2   rw,v
// [manifest] .sw.wait               2   0   b
// [manifest] .system.delay          3   0   w,v
// [manifest] .system.led            3   1   rw,v
// [manifest] .system.tick           3   2   rw,v
// [manifest] .timer                 4   0   rw,v
// [manifest] .timer.enable          4   1   rwB,vB
// [manifest] .timer.select          4   2   rw,v
// [manifest] .timer.period          4   3   rw,v
// [manifest] .timer.prescaler       4   4   rw,v
// [manifest] .timer.wait            4   0   b
// [manifest] .ldr                   5   0   r

const driver_write_fn console_write_fns[] = {
    &drv_console_write, // 0.0
    &drv_console_raw_write, // 0.1
    &drv_console_tx_block_write, // 0.2
    &drv_console_rx_block_write, // 0.3
    0, // 0.4
    0, // 0.5
    0 // 0.6
};

const driver_write_fn led_write_fns[] = {
    &drv_led_write, // 1.0
    &drv_led_select_write // 1.1
};

const driver_write_fn sw_write_fns[] = {
    0, // 2.0
    &drv_sw_select_write, //2.1
    &drv_sw_edge_write // 2.2
};

const driver_write_fn sys_write_fns[] = {
    &drv_sys_delay_write, // 3.0
    &drv_sys_led_write, // 3.1
    &drv_sys_tick_write // 3.2
};

const driver_write_fn timer_write_fns[] = {
    &drv_timer_write, // 4.0
    &drv_timer_enable_write, // 4.1
    &drv_timer_select_write, // 4.2
    &drv_timer_period_write // 4.3
};

const driver_write_fn ldr_write_fns[] = {
    0 // 5.0
};

const driver_read_fn console_read_fns[] = {
    0, // 0.0
    0, // 0.1
    &drv_console_rx_block_read, // 0.2
    &drv_console_rx_block_read, // 0.3
    &drv_console_rx_ready, // 0.4
    &drv_console_rx_read, // 0.5
    &drv_console_tx_ready // 0.6
};
const driver_read_fn led_read_fns[] = {
    &drv_led_read, // 1.0
    &drv_led_select_read, // 1.1
};

const driver_read_fn sw_read_fns[] = {
    &drv_sw_read, // 2.0
    &drv_sw_select_read, // 2.1
    &drv_sw_edge_read, // 2.2
};

const driver_read_fn sys_read_fns[] = {
    0, // 3.0
    &drv_sys_led_read, // 3.1
    &drv_sys_tick_read // 3.2
};

const driver_read_fn timer_read_fns[] = {
    &drv_timer_read, // 4.0
    &drv_timer_enable_read, // 4.1
    &drv_timer_select_read, // 4.2
    &drv_timer_period_read, // 4.3
};

const driver_read_fn ldr_read_fns[] = {
    &drv_ldr_read // 5.0
};

const driver_block_fn all_block_fns[] = {
    /* .console */
    &drv_console_rx_block, // 0.0
    
    /* .sw */
    &drv_sw_block, // 2.0

    /* .timer */
    &drv_timer_block, // 4.0
};

const struct driver drivers[] = {
    { console_write_fns, console_read_fns, &all_block_fns[0], 7 }, /* .console */
    { led_write_fns, led_read_fns, 0, 2 }, /* .led */
    { sw_write_fns, sw_read_fns, &all_block_fns[1], 2 }, /* .sw */
    { sys_write_fns, sys_read_fns, 0, 1 }, /* .system */
    { timer_write_fns, timer_read_fns, &all_block_fns[2], 1}, /* .timer */
    { ldr_write_fns, ldr_read_fns, 0, 1 }, /* .ldr */
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

    uart_print("[block builtin]\r\n");

    driver_block_fn fn = drivers[low].block_fns[high];
    if (fn) {
        if (fn()) {
            context_save(&current_task->context, &&restore);
            if (schedule_task())
                scheduler_loop();
        } else {
            return 0;
        }
    } else {
        return 0;
    }
restore:
    return 1;
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

void ___fail_builtin(struct eh_t *eh) {
    throw_exception(0);

}

void ___trace_builtin(struct eh_t *eh) {
    while (eh) {
        if (eh->ident) {
            uart_print(eh->ident);
            uart_print("\r\n");
        }
        eh = eh->parent;
    }
}

void runtime_set_vector_table_entry(unsigned int entry, handler_t handler) {
    __vector_table[entry] = handler;
}

/* block utilities */

int block_util_match_data(struct task_info * task, unsigned int info) {
    return info == task->block_data;
}

int block_util_match_sw(struct task_info * task, unsigned int info) {
    return (task->block_data & 0xFFFF) == (info & 0xFFFF) && (task->block_data & info & 0xFFFF0000);
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
    return DRV_SUCCESS;
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
unsigned int selected_sw_edge = 1;

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

int drv_sw_edge_write(int val, char *str) {
    if (val >= 0 && val < 3) {
        selected_sw_edge = val + 1;
        return 1;
    } else {
        return 0;
    }
}

int drv_sw_edge_read() {
    return selected_sw_edge;
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

int drv_sys_led_write(int val, char *str) {
    if (val)
        pin_set(STATUS_LED_PIN);
    else
        pin_clear(STATUS_LED_PIN);

    return DRV_SUCCESS;
}

int drv_sys_tick_write(int val, char *str) {
    asm volatile ("mtc0 %0, $9" : "+r"(val));
    return DRV_SUCCESS;
}

int drv_sys_led_read() {
    return pin_test(STATUS_LED_PIN);
}

int drv_sys_tick_read() {
    unsigned int tick;
    asm volatile ("mfc0 %0, $9" : "=r"(tick));
    return tick;
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

int drv_timer_enable_read(int val, char *str) {
    u_timerb_config config = u_timerb_load_config(selected_timer);
    if (config.on) {
        return DRV_TRUE;
    } else {
        return DRV_FALSE;
    }
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

int drv_ldr_read() {
    return *u_ana_buffer_ptr(0);
}

int rx_block_init = 0;

int drv_console_rx_block() {
    block_task(current_task, &block_util_match_data, BLOCK_REASON_CONSOLE_RX, 0);
    return DRV_SUCCESS;
}

int sw_block_init = 0;

int drv_sw_block() {
    unsigned int data;
    if (selected_sw.group == PIN_GROUP_A) {
        switch (selected_sw.pin) {
            case BITS(2): data = 0; break;
            case BITS(3): data = 1; break;
            case BITS(4): data = 2; break;
            default: return 0;
        }
    } else {
        data = 3;
    }

    runtime_set_vector_table_entry(_CHANGE_NOTICE_VECTOR, &handler_sw_edge);

    block_task(current_task, &block_util_match_sw, BLOCK_REASON_SW, data | (selected_sw_edge << 16));
    
    u_cn_enable(selected_sw);
    return 1;
}

int timer_block_init = 0;

int drv_timer_block() {
    int n;
    switch (selected_timer) {
        case 0:
            runtime_set_vector_table_entry(_TIMER_2_VECTOR, &handler_timer_b2);
            n = 2;
            IEC0SET = BITS(9);
            break;
        case 1:
        case 4:
            n = 3;
            runtime_set_vector_table_entry(_TIMER_3_VECTOR, &handler_timer_b3);
            IEC0SET = BITS(14);
            break;
        case 2:
            n = 4;
            runtime_set_vector_table_entry(_TIMER_4_VECTOR, &handler_timer_b4);
            IEC0SET = BITS(19);
            break;
        case 3:
        case 5:
            n = 5;
            runtime_set_vector_table_entry(_TIMER_5_VECTOR, &handler_timer_b5);
            break;
    }
    block_task(current_task, &block_util_match_data, BLOCK_REASON_TIMER, n);

    if (n == 2)
        IEC0SET = BITS(9);
    else if (n == 3)
        IEC0SET = BITS(14);
    else if (n == 4)
        IEC0SET = BITS(19);
    else if (n == 5)
        IEC0SET = BITS(19);

    return 1;
}


