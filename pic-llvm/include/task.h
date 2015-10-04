#ifndef TASK_H
#define TASK_H

#define TASK_LOCAL __attribute__((section(".task_local")))

struct context {
    int s0, s1, s2, s3, s4, s5, s6, s7, gp;
    void *sp, *fp;
    int (*ra)(void *);
};

enum task_state {
    TASK_STATE_EMPTY,
    TASK_STATE_NEW,
    TASK_STATE_READY,
    TASK_STATE_RUNNING,
    TASK_STATE_SOFT_BLOCKED,
    TASK_STATE_HARD_BLOCKED,
    TASK_STATE_ERROR
};

enum block_reason {
    BLOCK_REASON_UNBLOCKED,
    BLOCK_REASON_CONSOLE_RX,
    BLOCK_REASON_SW,
    BLOCK_REASON_TIMER
};

struct task_info {
    enum task_state state;
    struct task_info *parent;
    unsigned int depth;
    struct context __attribute__((aligned(4))) context;
    enum block_reason reason;
    unsigned int block_data;
    unsigned int unblock_info;
    int (*block_fn)(struct task_info *, unsigned int);
};

enum task_size {
    TASK_SIZE_SMALL,
    TASK_SIZE_LARGE
};

enum {
    TOTAL_STACK_SPACE = 0x4000,
    TASK_SLOT_SIZE = 0x100,
    TOTAL_STACK_SLOTS = (TOTAL_STACK_SPACE / TASK_SLOT_SIZE),
    MAX_TASKS = 16,
};

struct task_attributes {
    enum task_size size;
};

void init_tasks(void);

void context_save(struct context *save, void *ra);
void context_switch(struct context *restore, void (*on_exit)());
int create_task(int (*fn)(void *), struct task_attributes attributes);
int schedule_task();

void scheduler_loop();

void task_exit();

void kill_task(struct task_info *task);

void block_task(struct task_info *task, int (*block_fn)(struct task_info *, unsigned int), enum block_reason reason, unsigned int data);
void unblock_tasks(enum block_reason reason, unsigned int info);

int task_count();
unsigned int task_stack_free();
void * task_stack_start(struct task_info * task);
unsigned int task_stack_allocation(struct task_info * task);
struct task_info * task_stack_owner(void * sp);

void handler_console_rx();
void handler_sw_edge();
void handler_timer_b2();
void handler_timer_b3();
void handler_timer_b4();
void handler_timer_b5();

#endif /* TASK_H */
