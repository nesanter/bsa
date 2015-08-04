#include "task.h"
#include "ulib/uart.h"


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
struct task_info *task_stack_slots[(TOTAL_STACK_SPACE / 0x100)];

void create_new_context(struct context *context, int (*entry)(void*), void *sp) {
    context->s0 = 0;
    context->s1 = 0;
    context->s2 = 0;
    context->s3 = 0;
    context->s4 = 0;
    context->s5 = 0;
    context->s6 = 0;
    context->s7 = 0;
    context->gp = 0;
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

    void *stack_ptr;
    unsigned int found, needed;
    if (attributes.size == TASK_SIZE_SMALL)
        needed = SMALL_TASK_SLOTS;
    else
        needed = LARGE_TASK_SLOTS;

    for (unsigned int i = 0; i < (TOTAL_STACK_SPACE / 0x100); i++) {
        if (task_stack_slots[i] == 0) {
            found++;
            if (found == needed) {
                stack_ptr = task_stack + (0x100 * i);
                for (unsigned int j = 0; j < found; j++) {
                    task_stack_slots[i+j] = new_task;
                }
                break;
            }
        }
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
                if (task_list[i].block_fn(task_list[i].block_data)) {
                    task_list[i].state = TASK_STATE_READY;
                }
                if (!next_task || next_task->depth > task_list[i].depth) {
                    next_task = &task_list[i];
                }
                any_tasks = 1;
                break;
            case TASK_STATE_ERROR:
                break;
        }
    }

    struct context *old_context;
    if (current_task)
        &current_task->context;
    else
        old_context = 0;

    current_task = next_task;

    if (current_task) {
        if (next_task->state == TASK_STATE_NEW) {
            next_task->state = TASK_STATE_RUNNING;
            context_switch(old_context, &next_task->context, &task_exit);
        } else {
            next_task->state = TASK_STATE_RUNNING;
            context_switch(old_context, &next_task->context, 0);
        }
        return -1; // unreachable
    } else if (any_tasks) {
        return 1;
    } else {
        return 2;
    }
}

void scheduler_loop() {
    int res;
    while (schedule_task() == 1) {
        // there are still tasks, but none are schedulable
        // enter "idle" mode (OSCCON<4> has already been cleared by bootloader)
        asm volatile ("wait");
    }
    // there are no tasks left, return to bootloader
#ifdef HARD_RUNTIME
    uart_print("[all tasks ended]");
    while (1);
#else
    asm volatile ("syscall");
#endif
}

void task_exit() {
    current_task->state = TASK_STATE_EMPTY;
    schedule_task();
}


