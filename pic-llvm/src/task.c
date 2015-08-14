#include "task.h"
#include "ulib/ulib.h"
#include "ulib/uart.h"
#include "ulib/util.h"
#include "proc/processor.h"


enum {
    TOTAL_STACK_SPACE = 0x4000,
    TASK_SLOT_SIZE = 0x100,
    TOTAL_STACK_SLOTS = (TOTAL_STACK_SPACE / TASK_SLOT_SIZE),
    MAX_TASKS = 16,
};
unsigned int SMALL_TASK_SLOTS = 2;
unsigned int LARGE_TASK_SLOTS = 4;

struct task_info *current_task = 0;
struct task_info task_list[MAX_TASKS];

void *task_stack[TOTAL_STACK_SPACE / 0x4];
struct task_info *task_stack_slots[(TOTAL_STACK_SPACE / TASK_SLOT_SIZE)];

void create_new_context(struct context *context, int (*entry)(void*), void *sp) {
    context->s0 = 0;
    context->s1 = 0;
    context->s2 = 0;
    context->s3 = 0;
    context->s4 = 0;
    context->s5 = 0;
    context->s6 = 0;
    context->s7 = 0;
#ifdef HARD_RUNTIME
    unsigned int gp;
    asm volatile ("add %0, $gp, $zero" : "=r"(gp));
    context->gp = gp;
#else
    context->gp = 0;
#endif
    context->sp = sp;
    context->fp = 0;
    context->ra = entry;
}

int create_task(int (*fn)(void *), struct task_attributes attributes) {
    struct task_info *new_task = 0;
    for (unsigned int i = 0; i < MAX_TASKS; i++) {
        if (task_list[i].state == TASK_STATE_EMPTY) {
            new_task = &task_list[i];
            break;
        }
    }

    if (!new_task) {
        return 1;
    }

    new_task->state = TASK_STATE_NEW;
    new_task->parent = current_task;
    new_task->depth = (current_task ? current_task->depth + 1 : 0);

    void *stack_ptr = 0;
    unsigned int found = 0, needed;
    if (attributes.size == TASK_SIZE_SMALL)
        needed = SMALL_TASK_SLOTS;
    else
        needed = LARGE_TASK_SLOTS;

    for (unsigned int i = 0; i < (TOTAL_STACK_SPACE / TASK_SLOT_SIZE); i++) {
        if (task_stack_slots[i] == 0) {
            found++;
            if (found == needed) {
                stack_ptr = task_stack + (0x100 * i);
                for (unsigned int j = 0; j < found; j++) {
                    task_stack_slots[i-j] = new_task;
                }
                break;
            }
        } else {
            found = 0;
        }
    }

    if (!stack_ptr) {
        uart_print("[out of stack space]\r\n");
        return 1;
    }

    create_new_context(&new_task->context, fn, stack_ptr);

    return 0;
}

int schedule_task() {
    struct task_info *next_task = 0;
    int any_tasks = 0;

    for (int i = 0; i < MAX_TASKS; i++) {
        switch (task_list[i].state) {
            case TASK_STATE_EMPTY:
                break;
            case TASK_STATE_NEW:
            case TASK_STATE_READY:
            case TASK_STATE_RUNNING: /* this one shouldn't happen */
                if (!next_task || next_task->depth > task_list[i].depth) {
                    next_task = &task_list[i];
                }
                any_tasks = 1;
                break;
            case TASK_STATE_SOFT_BLOCKED:
                task_list[i].state = TASK_STATE_READY;
                any_tasks = 1;
                break;
            case TASK_STATE_HARD_BLOCKED:
                /*
                if (task_list[i].block_fn(&task_list[i], task_list[i].block_data)) {
                    task_list[i].state = TASK_STATE_READY;
                }
                if (!next_task || next_task->depth > task_list[i].depth) {
                    next_task = &task_list[i];
                }
                */
                any_tasks = 1;
                break;
            case TASK_STATE_ERROR:
                break;
        }
    }

    /*
    struct context *old_context;
    if (current_task)
        &current_task->context;
    else
        old_context = 0;
    */

    current_task = next_task;

    if (current_task) {
        if (next_task->state == TASK_STATE_NEW) {
            uart_print("[scheduled new task]\r\n");
            next_task->state = TASK_STATE_RUNNING;
            context_switch(&next_task->context, &task_exit);
        } else {
            uart_print("[scheduled old task]\r\n");
            next_task->state = TASK_STATE_RUNNING;
            context_switch(&next_task->context, 0);
        }
        return -1; // unreachable
    } else if (any_tasks) {
        return 1;
    } else {
        return 2;
    }
}

void scheduler_loop() {
    uart_print("[entering scheduler loop]\r\n");
    int res;
    while (schedule_task() == 1) {
        // there are still tasks, but none are schedulable
        // enter "idle" mode (OSCCON<4> has already been cleared by bootloader)
//        uart_print("[idleing]\r\n");
//        asm volatile ("wait");
    }
    // there are no tasks left, return to bootloader
#ifdef HARD_RUNTIME
    uart_print("[all tasks ended]\r\n");
    while (1);
#else
    asm volatile ("syscall");
#endif
}

void task_exit() {
    uart_print("[task exit]\r\n");
    current_task->state = TASK_STATE_EMPTY;

    for (unsigned int i = 0; i < (TOTAL_STACK_SPACE / TASK_SLOT_SIZE); i++) {
        if (task_stack_slots[i] == current_task) {
            task_stack_slots[i] = 0;
        }
    }

    if (schedule_task())
        scheduler_loop();
}

void block_task(struct task_info *task, int (*block_fn)(struct task_info *, unsigned int), enum block_reason reason, unsigned int data) {
    uart_print("[blocking task]");
    task->state = TASK_STATE_HARD_BLOCKED;
    task->reason = reason;
    task->block_data = data;
    task->block_fn = block_fn;
}

void unblock_tasks(enum block_reason reason, unsigned int info) {
    for (unsigned int i = 0 ; i < MAX_TASKS; i++) {
        if (task_list[i].reason == reason) {
            if (task_list[i].block_fn(&task_list[i], info)) {
                task_list[i].state = TASK_STATE_READY;
                task_list[i].reason = BLOCK_REASON_UNBLOCKED;
            }
        }
    }
}

void handler_sw_edge() {
    uart_print("[handling sw edge]\r\n");
    int changed_a = u_cn_changed(CNA);
    int changed_b = u_cn_changed(CNB);
    int val_a = PORTA;
    int val_b = PORTB;
    for (unsigned int i = 0 ; i < MAX_TASKS; i++) {
        if (task_list[i].reason == BLOCK_REASON_SW) {
            unsigned int p;
            switch (task_list[i].block_data & 0xFFFF) {
                case 0:
                    if (changed_a & BITS(2)) {
                        if ((val_a & BITS(2)) ? task_list[i].block_data & 0x20000 : task_list[i].block_data & 0x10000) {
                            task_list[i].state = TASK_STATE_READY;
                            task_list[i].reason = BLOCK_REASON_UNBLOCKED;
                        }
                    }
                    break;
                case 1:
                    if (changed_a & BITS(3)) {
                        if ((val_a & BITS(3)) ? task_list[i].block_data & 0x20000 : task_list[i].block_data & 0x10000) {
                            task_list[i].state = TASK_STATE_READY;
                            task_list[i].reason = BLOCK_REASON_UNBLOCKED;
                        }
                    }
                    break;
                case 2:
                    if (changed_a & BITS(4)) {
                        if ((val_a & BITS(4)) ? task_list[i].block_data & 0x20000 : task_list[i].block_data & 0x10000) {
                            task_list[i].state = TASK_STATE_READY;
                            task_list[i].reason = BLOCK_REASON_UNBLOCKED;
                        }
                    }
                    break;
                case 3:
                    if (changed_b & BITS(14)) {
                        if ((val_b & BITS(14)) ? task_list[i].block_data & 0x20000 : task_list[i].block_data & 0x10000) {
                            task_list[i].state = TASK_STATE_READY;
                            task_list[i].reason = BLOCK_REASON_UNBLOCKED;
                        }
                    }
                    break;
            }
        }
    }
    IFS1CLR = BITS(13) | BITS(14);
}

void handler_timer_b2() {
    unblock_tasks(BLOCK_REASON_TIMER, 2);
    IFS0CLR = BITS(9);
    IEC0CLR = BITS(9);
}
void handler_timer_b3() {
    unblock_tasks(BLOCK_REASON_TIMER, 3);
    IFS0CLR = BITS(14);
    IEC0CLR = BITS(14);
}
void handler_timer_b4() {
    unblock_tasks(BLOCK_REASON_TIMER, 4);
    IFS0CLR = BITS(19);
    IEC0CLR = BITS(19);
}
void handler_timer_b5() {
    unblock_tasks(BLOCK_REASON_TIMER, 5);
    IFS0CLR = BITS(24);
    IEC0CLR = BITS(24);
}

