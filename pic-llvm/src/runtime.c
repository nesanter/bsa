#include "runtime.h"
#include "driver/console.h"
#include "driver/basic_io.h"
#include "driver/timer.h"
#include "driver/system.h"
#include "driver/task.h"
#include "driver/gfx.h"
#include "driver/pulse.h"
#include "driver/js.h"
#include "ulib/ulib.h"
#include "ulib/uart.h"
#include "ulib/util.h"
#include "ulib/pins.h"
#include "ulib/ldr.h"
#include "ulib/js.h"
#include "ulib/piezo.h"
#include "ulib/gfx.h"
#include "exception.h"
#include "task.h"
#include "version.h"
#include "proc/processor.h"

#define DRV_SUCCESS (1)
#define DRV_FAILURE (0)

#define DRV_TRUE (1)
#define DRV_FALSE (0)

const Pin STATUS_LED_PIN = { PIN_GROUP_A, BITS(1) };

extern unsigned int current_system_tick;

extern struct task_info *current_task;

extern handler_t volatile __vector_table[43];

struct driver {
    const driver_write_fn *write_fns;
    const driver_read_fn *read_fns;
    const driver_block_fn *block_fns;
    unsigned int fn_count;
};

struct task_attributes default_task_attributes = { TASK_SIZE_LARGE };

extern unsigned int SMALL_TASK_SLOTS, LARGE_TASK_SLOTS;
extern struct task_info * current_task;

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
// [manifest] .system.exit           3   3   w,v
// [manifest] .timer                 4   0   rw,v
// [manifest] .timer.enable          4   1   rwB,vB
// [manifest] .timer.select          4   2   rw,v
// [manifest] .timer.period          4   3   rw,v
// [manifest] .timer.prescaler       4   4   rw,v
// [manifest] .timer.wait            4   0   b
// [manifest] .ldr                   5   0   r
// [manifest] .ldr.enable            5   1   wB,vB
// [manifest] .task.size             6   0   rw,v
// [manifest] .task.stack.small      6   1   rw,v
// [manifest] .task.stack.large      6   2   rw,v
// [manifest] .task.stack.free       6   3   r
// [manifest] .task.count            6   4   r
// [manifest] .task.max              6   5   r
// [manifest] .task.this.depth       6   6   rw,v
// [manifest] .task.this.stack       6   7   r
// [manifest] .task.this.allocation  6   8   r
// [manifest] .piezo.enable          7   0   rwB,vB
// [manifest] .piezo.width           7   1   rw,v
// [manifest] .piezo.active          7   2   rwB,vB
// [manifest] .gfx.enable            8   0   rwB,vB
// [manifest] .gfx.bb                8   1   w,v
// [manifest] .gfx                   8   2   w,v
// [manifest] .gfx.char              8   3   w,v,s
// [manifest] .gfx.buffer            8   4   rw,v
// [manifest] .gfx.buffer.flush      8   5   rw,v
// [manifest] .gfx.buffer.mode       8   6   rw,v
// [manifest] .expm.select           9   0   rw,v
// [manifest] .expm.wait             9   0   b
// [manifest] .expm.emit             9   1   w,v
// [manifest] .pulse.enable.square   10  0   wB,vB
// [manifest] .pulse.enable.pwm      10  1   wB,vB
// [manifest] .pulse.select          10  2   rw,v
// [manifest] .pulse.map             10  3   w,v
// [manifest] .pulse.compare         10  4   rw,v
// [manifest] .pulse.active          10  5   rw,v
// [xxx] .js.enable             11  0   rwB,vB
// [xxx] .js.x                  11  1   r
// [xxx] .js.y                  11  2   r
// [xxx] .js.sw                 11  3   r
// [manifest] .ana.enable            12  0   rwB,vB
// [manifest] .ana.select            12  1   rw,v
// [manifest] .ana                   12  2
// [manifest] .ana.cfg.active        12  3   rwB,vB
// [manifest] .ana.cfg.cclk          12  4   rw,v
// [manifest] .ana.cfg.sclk          12  5   rw,v

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
    &drv_led_select_write, // 1.1
};

const driver_write_fn sw_write_fns[] = {
    0, // 2.0
    &drv_sw_select_write, //2.1
    &drv_sw_edge_write // 2.2
};

const driver_write_fn sys_write_fns[] = {
    &drv_sys_delay_write, // 3.0
    &drv_sys_led_write, // 3.1
    &drv_sys_tick_write, // 3.2
    &drv_sys_exit_write // 3.3
};

const driver_write_fn timer_write_fns[] = {
    &drv_timer_write, // 4.0
    &drv_timer_enable_write, // 4.1
    &drv_timer_select_write, // 4.2
    &drv_timer_period_write, // 4.3
    &drv_timer_prescaler_write // 4.4
};

const driver_write_fn ldr_write_fns[] = {
    0, // 5.0
    &drv_ldr_enable_write // 5.1
};

const driver_write_fn js_write_fns[] = {
    &drv_js_enable_write, // 10.0
    0, // 10.1
    0, // 10.2
    0 // 10.3
};

const driver_write_fn task_write_fns[] = {
    &drv_task_size_write, // 6.0
    &drv_task_stack_small_write, // 6.1
    &drv_task_stack_large_write, // 6.2
    0, // 6.3
    0, // 6.4
    0, // 6.5
    &drv_task_this_depth_write, // 6.6
    0, // 6.7
    0 // 6.8
};


const driver_write_fn piezo_write_fns[] = {
    &drv_piezo_enable_write, // 7.0
    &drv_piezo_width_write, // 7.1
    0,
    &drv_piezo_active_write // 7.2
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
    &drv_sys_tick_read, // 3.2
    0 // 3.3
};

const driver_read_fn timer_read_fns[] = {
    &drv_timer_read, // 4.0
    &drv_timer_enable_read, // 4.1
    &drv_timer_select_read, // 4.2
    &drv_timer_period_read, // 4.3
    &drv_timer_prescaler_read // 4.4
};

const driver_read_fn ldr_read_fns[] = {
    &drv_ldr_read // 5.0
};

const driver_read_fn js_read_fns[] = {
    &drv_js_enable_read, // 10.0
    &drv_js_x_read, // 10.1
    &drv_js_y_read, // 10.2
    &drv_js_sw_read // 10.3
};

const driver_read_fn task_read_fns[] = {
    &drv_task_size_read, // 6.0
    &drv_task_stack_small_read, // 6.1
    &drv_task_stack_large_read, // 6.2
    &drv_task_stack_free_read, // 6.3
    &drv_task_count_read, // 6.4
    &drv_task_max_read, // 6.5
    &drv_task_this_depth_read, // 6.6
    &drv_task_this_stack_read, // 6.7
    &drv_task_this_allocation_read // 6.8
};

const driver_read_fn piezo_read_fns[] = {
    &drv_piezo_enable_read, // 7.0
    &drv_piezo_width_read, // 7.1
    0,
    &drv_piezo_active_read // 7.2
};

const driver_write_fn gfx_write_fns[] = {
    &drv_gfx_enable_write, // 8.0
    &drv_gfx_bb_write, // 8.1
    &drv_gfx_write, // 8.2
    &drv_gfx_char_write, // 8.3
    &drv_gfx_buf_write, // 8.4
    &drv_gfx_buf_flush_write, // 8.5
    &drv_gfx_buf_ptr_write, // 8.6
    &drv_gfx_buf_mode_write // 8.7
};

const driver_read_fn gfx_read_fns[] = {
    0, // 8.0
    0, // 8.1
    0, // 8.2
    0, // 8.3
};

const driver_write_fn expm_write_fns[] = {
    &drv_expm_select_write, // 9.0
    &drv_expm_emit_write // 9.1
};

const driver_read_fn expm_read_fns[] = {
    &drv_expm_select_read, // 9.0
    0
};

const driver_write_fn pulse_write_fns[] = {
    &drv_pulse_square_enable_write, // 10.0
    &drv_pulse_pwm_enable_write, // 10.1
    &drv_pulse_select_write, // 10.2
    &drv_pulse_map_write, // 10.3
    &drv_pulse_compare_write, // 10.4
    &drv_pulse_active_write // 10.5
};

const driver_read_fn pulse_read_fns[] = {
    0, // 10.0
    0, // 10.1
    &drv_pulse_select_read, // 10.2
    0, // 10.3
    &drv_pulse_compare_read, // 10.4
    &drv_pulse_active_read // 10.5
};

const driver_write_fn ana_write_fns[] = {
    &drv_ana_enable_write, // 12.0
    &drv_ana_select_write, // 12.1
    0, // 12.2
    &drv_ana_cfg_active_write, // 12.3
    &drv_ana_cfg_cclk_write, // 12.4
    &drv_ana_cfg_sclk_write // 12.5
};

const driver_read_fn ana_read_fns[] = {
    &drv_ana_enable_read, // 12.0
    &drv_ana_select_read, // 12.1
    &drv_ana_read, // 12.2
    &drv_ana_cfg_active_read, // 12.3
    &drv_ana_cfg_cclk_read, // 12.4
    &drv_ana_cfG_sclk_read // 12.5
};

const driver_block_fn all_block_fns[] = {
    /* .console */
    &drv_console_rx_block, // 0.0

    /* .sw */
    &drv_sw_block, // 2.0

    /* .timer */
    &drv_timer_block, // 4.0

    &drv_expm_block, // 9.0
};

const struct driver drivers[] = {
    { console_write_fns, console_read_fns, &all_block_fns[0], 7 }, /* .console */
    { led_write_fns, led_read_fns, 0, 2 }, /* .led */
    { sw_write_fns, sw_read_fns, &all_block_fns[1], 2 }, /* .sw */
    { sys_write_fns, sys_read_fns, 0, 1 }, /* .system */
    { timer_write_fns, timer_read_fns, &all_block_fns[2], 1}, /* .timer */
    { ldr_write_fns, ldr_read_fns, 0, 1 }, /* .ldr */
    { task_write_fns, task_read_fns, 0, 0 }, /* .task */
    { piezo_write_fns, piezo_read_fns, 0, 0 }, /* .piezo */
    { gfx_write_fns, gfx_read_fns, 0, 0 }, /* .gfx */
    { expm_write_fns, expm_read_fns, &all_block_fns[3], 0 }, /* .expm */
    { pulse_write_fns, pulse_read_fns, 0, 0 }, /* .pulse */
//    { js_write_fns, js_read_fns, 0, 0}, /* .js */
    0,
    { ana_write_fns, ana_read_fns, 0, 0 }, /* .ana */
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

#ifdef RUNTIME_INFO
    uart_print("[block builtin]\r\n");
#endif

    driver_block_fn fn = drivers[low].block_fns[high];
    if (fn) {
        if (fn()) {
            current_task->eh_ptr = eh;
            context_save(&current_task->context, &&restore);
            if (schedule_task())
                scheduler_loop();
        } else {
            return current_task->unblock_info;
        }
    } else {
        return 0;
    }
restore:
    return current_task->unblock_info;
}

/*
int ___write_addr_builtin(struct eh_t *eh, unsigned int target, int val, int addr) {
    unsigned int low = target & 0xFFFF;
    unsigned int high = (target & 0xFFFF0000) >> 16;

    if (drivers[low].write_addr_fns) {
        driver_write_addr_fn fn = drivers[low].write_addr_fns[high];
        if (fn) {
            return fn(val, addr);
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

int ___read_addr_builtin(struct eh_t *eh, unsigned int target, int addr) {
    unsigned int low = target & 0xFFFF;
    unsigned int high = (target & 0xFFFF0000) >> 16;

    if (drivers[low].read_addr_fns) {
        driver_read_addr_fn fn = drivers[low].read_addr_fns[high];
        if (fn) {
            return fn(addr);
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}
*/

void ___yield_builtin(struct eh_t *eh, unsigned int wait) {
    /*
    void *ra;
    asm volatile ("add %0, $ra, $zero" : "=r"(ra));
    */
#ifdef RUNTIME_INFO
    uart_print("[saving old task]\r\n");
#endif
    current_task->eh_ptr = eh;
    context_save(&current_task->context, &&restore);

    if (wait == 0) {
        current_task->state = TASK_STATE_SOFT_BLOCKED;
    } else {
        current_task->block_data = wait;
        current_task->reason = BLOCK_REASON_CORE_TIMER;
        current_task->state = TASK_STATE_HARD_BLOCKED;
    }
#ifdef RUNTIME_INFO
    uart_print("[yielding]\r\n");
#endif
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
    struct task_attributes attr = default_task_attributes;
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

void ___canary_builtin(struct eh_t *eh) {
    void * sp;
    asm ("move %0, $sp" : "=r"(sp));
    if (task_stack_owner(sp) != current_task) {
        uart_print("[the canary is dead]\r\n");
        asm volatile ("addi $k1, $zero, 0; \
                       syscall;");
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

int block_util_always(struct task_info * task, unsigned int info) {
    return 1;
}

/* driver functions */

int TASK_LOCAL tx_block = 0;
int TASK_LOCAL rx_block = 0;

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

Pin TASK_LOCAL selected_led = {PIN_GROUP_B, BITS(4)};

int drv_led_select_write(int val, char *str) {
    selected_led.group = PIN_GROUP_B;
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

Pin TASK_LOCAL selected_sw = { PIN_GROUP_A, BITS(2) };
unsigned int TASK_LOCAL selected_sw_edge = 1;

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
        default: return DRV_FAILURE;
    }
    pin_mode_set(selected_sw, 1, 0);
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
    runtime_delay(val);

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
    current_system_tick = val;
    return DRV_SUCCESS;
}

int drv_sys_led_read() {
    return pin_test(STATUS_LED_PIN);
}

int drv_sys_tick_read() {
//    unsigned int tick;
//    asm volatile ("mfc0 %0, $9" : "=r"(tick));
//    return tick;
    return current_system_tick;
}

int drv_sys_exit_write(int val, char *str) {
    throw_global_exception(val);
    runtime_exit();
    return DRV_SUCCESS;
}

u_timerb_select TASK_LOCAL selected_timer;

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

int drv_ldr_enable_write(int val, char *str) {
    if (val)
        init_ldr();
    return DRV_SUCCESS;
}

Pin js_sw_pin = { PIN_GROUP_B, BITS(4) };

int drv_js_enable_write(int val, char *str) {
    if (val) {
        init_js();
        pin_mode_set(js_sw_pin, 1, 0);
    } else {
        deinit_js();
        pin_mode_set(js_sw_pin, 0, 0);
    }
    return DRV_SUCCESS;
}

int drv_js_enable_read() {
    return get_js_init();
}

int drv_js_x_read() {
    return *u_ana_buffer_ptr(0);
}

int drv_js_y_read() {
//    return *u_ana_buffer_ptr(1);
    return ADC1BUF1;
}


int drv_js_sw_read() {
    return pin_test(js_sw_pin);
}

int drv_task_size_write(int val ,char *str) {
    if (val == 0) {
        default_task_attributes.size = TASK_SIZE_SMALL;
    } else if (val == 1) {
        default_task_attributes.size = TASK_SIZE_LARGE;
    } else {
        return DRV_FAILURE;
    }
    return DRV_SUCCESS;
}

int drv_task_stack_small_write(int val, char *str) {
    SMALL_TASK_SLOTS = val;
    return DRV_SUCCESS;
}

int drv_task_stack_large_write(int val, char *str) {
    LARGE_TASK_SLOTS = val;
    return DRV_SUCCESS;
}

int drv_task_this_depth_write(int val, char *str) {
    current_task->depth = val;
    return DRV_SUCCESS;
}

int drv_task_size_read() {
    return default_task_attributes.size == TASK_SIZE_SMALL ? 0 : 1;
}

int drv_task_stack_small_read() {
    return SMALL_TASK_SLOTS;
}

int drv_task_stack_large_read() {
    return LARGE_TASK_SLOTS;
}

int drv_task_stack_free_read() {
    return task_stack_free();
}

int drv_task_count_read() {
    return task_count(1);
}

int drv_task_max_read() {
    return MAX_TASKS;
}

int drv_task_this_depth_read() {
    return current_task->depth;
}

int drv_task_this_stack_read() {
    /*
    int sp;
    asm volatile ("move %0, $sp;" : "=r"(sp));
    return sp;
    */
    void * start = task_stack_start(current_task);
    void * sp;
    asm volatile ("move %0, $sp;" : "=r"(sp));
    return (int)(start - sp);
}

int drv_task_this_allocation_read() {
    return task_stack_allocation(current_task);
}

int drv_piezo_enable_write(int val, char *str) {
    init_piezo();
    return DRV_SUCCESS;
}

int drv_piezo_width_write(int val, char *str) {
    u_oc_set_secondary_compare(OC3, val);
    return DRV_SUCCESS;
}

int drv_piezo_active_write(int val, char *str) {
    if (val) {
        OC3CONSET = BITS(15);
    } else {
        OC3CONCLR = BITS(15);
    }

    return DRV_SUCCESS;
}

int drv_piezo_enable_read() {
    return 1;
}

int drv_piezo_width_read() {
    return u_oc_get_secondary_compare(OC3);
}

int drv_piezo_active_read() {
    if (OC3CON & BITS(15)) {
        return DRV_TRUE;
    } else {
        return DRV_FALSE;
    }
}

int drv_gfx_enable_write(int val, char *str) {
    gfx_setup();
    gfx_init_sequence();

    return DRV_SUCCESS;
}

int drv_gfx_bb_write(int val, char *str) {
    unsigned char c0 = val & 0xFF;
    unsigned char c1 = (val & 0xFF00) >> 8;
    unsigned char p0 = (val & 0xFF0000) >> 16;
    unsigned char p1 = (val & 0xFF000000) >> 24;

    gfx_bb_set(c0, c1, p0, p1);

    return DRV_SUCCESS;
}

int drv_gfx_write(int val, char *str) {
/*
    if (gfx_buffer_head < 8) {
        gfx_buffer[gfx_buffer_head++] = (unsigned char)(val & 0xFF);
    }
    return 8 - gfx_buffer_head;
*/
    while (u_spi_get_tx_full(SPI1));
    u_spi_buffer_write(SPI1, val & 0xFF);
    return DRV_SUCCESS;
}

/*
int drv_gfx_flush_write(int val, char *str) {
    if (val) {
        gfx_write(gfx_buffer, gfx_buffer_head);
        int old = gfx_buffer_head;
        gfx_buffer_head = 0;
        return old;
    } else {
        return 0;
    }
}
*/

int drv_gfx_char_write(int val, char *str) {
    unsigned char blank = 0;
    if (str) {
        while (*str) {
            gfx_write((unsigned char *)font_lookup(*str), 6);
            gfx_write(&blank, 1);
            str++;
        }
    } else {
        gfx_write((unsigned char *)font_lookup(val), 6);
        gfx_write(&blank, 1);
    }
    return DRV_SUCCESS;
}

unsigned char GFX_BUFFER [ 128 * 8 ];
int TASK_LOCAL gfx_buf_mode = 0;
int TASK_LOCAL gfx_buf_ptr = 0;

int drv_gfx_buf_write(int val, char *str) {
    switch (gfx_buf_mode) {
        case 0: // write over
            GFX_BUFFER[gfx_buf_ptr++] = val;
            break;
        case 1: // OR
            GFX_BUFFER[gfx_buf_ptr++] |= val;
            break;
        case 2: // AND
            GFX_BUFFER[gfx_buf_ptr++] &= val;
            break;
        case 3: // XOR
            GFX_BUFFER[gfx_buf_ptr++] ^= val;
            break;
        case 4: // inverted write over
            GFX_BUFFER[gfx_buf_ptr++] = ~val;
            break;
        case 5: // inverted OR
            GFX_BUFFER[gfx_buf_ptr++] |= ~val;
            break;
        case 6: // inverted AND
            GFX_BUFFER[gfx_buf_ptr++] &= ~val;
            break;
        case 7: // inverted xor
            GFX_BUFFER[gfx_buf_ptr++] ^= ~val;
            break;
        default: break;
    }

    if (gfx_buf_ptr == (128 * 8))
        gfx_buf_ptr = 0;

    return DRV_SUCCESS;
}

int drv_gfx_buf_ptr_write(int val, char *str) {
    gfx_buf_ptr = val;
    return DRV_SUCCESS;
}

int drv_gfx_buf_mode_write(int val, char *str) {
    gfx_buf_mode = val;
    return DRV_SUCCESS;
}

int drv_gfx_buf_flush_write(int val, char *str) {
    gfx_write(GFX_BUFFER, 128 * 8);

    return DRV_SUCCESS;
}

int drv_gfx_buf_read() {
    return GFX_BUFFER[gfx_buf_ptr];
}

int drv_gfx_buf_ptr_read() {
    return gfx_buf_ptr;
}

int drv_gfx_buf_mode_read() {
    return gfx_buf_mode;
}

int TASK_LOCAL expm_selected;

int drv_expm_select_write(int val, char *str) {
    expm_selected = val;

    return DRV_SUCCESS;
}

int drv_expm_select_read() {
    return expm_selected;
}

int drv_expm_emit_write(int val, char *str) {
    unblock_tasks(BLOCK_REASON_MANUAL, val);
    return DRV_SUCCESS;
}

OC TASK_LOCAL pulse_oc_select;

int drv_pulse_square_enable_write(int val, char *str) {
    u_oc_config cfg = u_oc_load_config(pulse_oc_select);

    if (val) {
        cfg.on = 1;
        cfg.timer_select = 0;
        cfg.mode_32 = 1;
        cfg.mode_select = 3;
    } else {
        cfg.on = 0;
    }

    u_oc_save_config(pulse_oc_select, cfg);
    return DRV_SUCCESS;
}

int drv_pulse_pwm_enable_write(int val, char *str) {
    u_oc_config cfg = u_oc_load_config(pulse_oc_select);

    if (val) {
        cfg.on = 1;
        cfg.timer_select =0;
        cfg.mode_32 = 1;
        cfg.mode_select = 6;
    } else {
        cfg.on = 0;
    }

    u_oc_save_config(pulse_oc_select, cfg);

    return DRV_SUCCESS;
}

int drv_pulse_select_write(int val, char *str) {
    switch (val) {
        case 0:
            pulse_oc_select = OC1;
            break;
        case 1:
            pulse_oc_select = OC2;
            break;
        case 2:
            pulse_oc_select = OC3;
            break;
        case 3:
            pulse_oc_select = OC4;
            break;
        case 4:
            pulse_oc_select = OC5;
            break;
        default: return DRV_FAILURE;
    }

    return DRV_SUCCESS;
}

int drv_pulse_select_read() {
    return pulse_oc_select;
}

int drv_pulse_map_write(int val, char *str) {
    switch (pulse_oc_select) {
        case OC1:
            if (val < 0 || val > 4)
                return DRV_FAILURE;

            u_pps_out_oc1(val);
            break;
        case OC2:
            if (val < 0 || val > 6)
                return DRV_FAILURE;

            u_pps_out_oc2(val);
            break;
        case OC3:
            if (val < 0 || val > 4)
                return DRV_FAILURE;

            u_pps_out_oc3(val);
            break;
        case OC4:
            if (val < 0 || val > 3)
                return DRV_FAILURE;
            
            u_pps_out_oc4(val);
            break;
        case OC5:
            if (val < 0 || val > 3)
                return DRV_FAILURE;
            u_pps_out_oc5(val);
        default: return DRV_FAILURE;
    }

    return DRV_FAILURE;
}

int drv_pulse_compare_write(int val, char *str) {
    u_oc_set_secondary_compare(pulse_oc_select, val);

    return DRV_SUCCESS;
}

int drv_pulse_compare_read() {
    return u_oc_get_secondary_compare(pulse_oc_select);
}


int drv_pulse_active_write(int val, char *str) {
    switch (pulse_oc_select) {
        case OC1:
            if (val)
                OC1CONSET = BITS(15);
            else
                OC1CONCLR = BITS(15);
            break;
        case OC2:
            if (val)
                OC2CONSET = BITS(15);
            else
                OC2CONCLR = BITS(15);
            break;
        case OC3:
            if (val)
                OC3CONSET = BITS(15);
            else
                OC3CONCLR = BITS(15);
            break;
        case OC4:
            if (val)
                OC4CONSET = BITS(15);
            else
                OC4CONCLR = BITS(15);
            break;
        case OC5:
            if (val)
                OC5CONSET = BITS(15);
            else
                OC5CONCLR = BITS(15);
            break;
        default: return DRV_FAILURE;
    }

    return DRV_SUCCESS;
}

int drv_pulse_active_read() {
    switch (pulse_oc_select) {
        case OC1: return OC1CON & BITS(15) ? 1 : 0;
        case OC2: return OC2CON & BITS(15) ? 1 : 0;
        case OC3: return OC3CON & BITS(15) ? 1 : 0;
        case OC4: return OC4CON & BITS(15) ? 1 : 0;
        case OC5: return OC5CON & BITS(15) ? 1 : 0;
        return 0;
    }
}



const driver_write_fn ana_write_fns[] = {
    &drv_ana_enable_write, // 12.0
    &drv_ana_select_write, // 12.1
    0, // 12.2
    &drv_ana_cfg_active_write, // 12.3
    &drv_ana_cfg_cclk_write, // 12.4
    &drv_ana_cfg_sclk_write // 12.5
};

const driver_read_fn ana_read_fns[] = {
    &drv_ana_enable_read, // 12.0
    &drv_ana_select_read, // 12.1
    &drv_ana_read, // 12.2
    &drv_ana_cfg_active_read, // 12.3
    &drv_ana_cfg_cclk_read, // 12.4
    &
int rx_block_init = 0;

int drv_console_rx_block() {
    if (u_uartx_get_rx_available(UART1)) {
        current_task->unblock_info = U1RXREG;
        return DRV_FAILURE;
    }
    if (!rx_block_init) {
        runtime_set_vector_table_entry(_UART_1_VECTOR, &handler_console_rx);
        uart_setup_rx_interrupts();
        rx_block_init = 1;
    }
    block_task(current_task, &block_util_always, BLOCK_REASON_CONSOLE_RX, 0);
    IEC1SET = BITS(8);
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
            default: throw_exception(0); return 0;
        }
    } else {
        data = 3;
    }

    if (!sw_block_init) {
        sw_block_init = 1;

        CNCONASET = BITS(15);
        CNCONBSET = BITS(15);

        IPC8SET = BITS(19);
        IPC8CLR = BITS(20) | BITS(18);

        runtime_set_vector_table_entry(_CHANGE_NOTICE_VECTOR, &handler_sw_edge);
    }

    block_task(current_task, &block_util_match_sw, BLOCK_REASON_SW, data | (selected_sw_edge << 16));
    
    u_cn_enable(selected_sw);
    IEC1SET = BITS(13) | BITS(14);
    return 1;
}

int timer_block_init = 0;

int drv_timer_block() {
    int n;
    switch (selected_timer) {
        case 0:
            if (!timer_block_init)
                runtime_set_vector_table_entry(_TIMER_2_VECTOR, &handler_timer_b2);
            n = 2;
            IEC0SET = BITS(9);
            break;
        case 1:
        case 4:
            n = 3;
            if (!timer_block_init)
                runtime_set_vector_table_entry(_TIMER_3_VECTOR, &handler_timer_b3);
            IEC0SET = BITS(14);
            break;
        case 2:
            n = 4;
            if (!timer_block_init)
                runtime_set_vector_table_entry(_TIMER_4_VECTOR, &handler_timer_b4);
            IEC0SET = BITS(19);
            break;
        case 3:
        case 5:
            n = 5;
            if (!timer_block_init)
                runtime_set_vector_table_entry(_TIMER_5_VECTOR, &handler_timer_b5);
            IEC0SET = BITS(24);
            break;
    }
    
    if (!timer_block_init) {
        timer_block_init = 1;
        // Timer B2-B5 interrupt priority = 4.0
        IPC2SET = BITS(2) | BITS(3);
        IPC2CLR = BITS(4);

        IPC3SET = BITS(2) | BITS(3);
        IPC3CLR = BITS(4);

        IPC4SET = BITS(2) | BITS(3);
        IPC4CLR = BITS(4);

        IPC5SET = BITS(2) | BITS(3);
        IPC5CLR = BITS(4);
    }

    block_task(current_task, &block_util_match_data, BLOCK_REASON_TIMER, n);

    /*
    if (n == 2)
        IEC0SET = BITS(9);
    else if (n == 3)
        IEC0SET = BITS(14);
    else if (n == 4)
        IEC0SET = BITS(19);
    else if (n == 5)
        IEC0SET = BITS(24);
    */

    return 1;
}

int drv_expm_block() {
    block_task(current_task, *block_util_match_data, BLOCK_REASON_MANUAL, expm_selected);
    return DRV_SUCCESS;
}

void runtime_exit() {
    asm volatile ("addi $k0, $zero, 0; syscall;");
    while (1); // unreachable
}

