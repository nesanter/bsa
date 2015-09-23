#include "exception.h"
#include "task.h"

#define EH_FLAG_ALWAYS (1)
#define EH_FLAG_FAILURE (4)

extern struct task_info *current_task;

struct eh_t * current_eh_t;

void set_current_eh_t(struct eh_t *ptr) {
    current_eh_t = ptr;
}

/*
int throw_exception(int info) {
    while (current_eh_t) {
        if (current_eh_t->flags & EH_FLAG_ALWAYS) {
            current_eh_t->always_handler();
        }
        if (current_eh_t->flags & EH_FLAG_FAILURE) {
            // handle
            current_eh_t->failure_handler(info);
            return EXCEPTION_HANDLED;
        } else {
            // propogate
            current_eh_t = current_eh_t->parent;
        }
    }
    // unhandled
#if EXCEPTION_ACTION == EXCEPTION_ACTION_KILL
#elif EXCEPTION_ACTION == EXCEPTION_ACTION_AT_HANDLER
#elif EXCEPTION_ACTION == EXCEPTION_RESUME
#else
#error "EXCEPTION_ACTION must be one of EXCEPTION_ACTION_[KILL|AT_HANDLER|RESUME]"
#endif
    return EXCEPTION_UNHANDLED;
}
*/

int throw_exception(int info) {
    while (current_eh_t) {
        if (current_eh_t->flags & EH_FLAG_ALWAYS) {
            current_eh_t->always_handler();
        }
        if (current_eh_t->flags & EH_FLAG_FAILURE) {
            current_eh_t->failure_handler(info);
        }
        current_eh_t = current_eh_t->parent;
    }
    kill_task(current_task);
    return 0;
}

