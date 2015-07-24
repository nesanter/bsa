#include "exception.h"

struct eh_t * current_eh_t;

void set_current_eh_t(void *ptr) {
    current_eh_t = ptr;
}

int throw_exception(int info) {
    if (current_eh_t) {
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
    } else {
        // unhandled
#if EXCEPTION_ACTION == "kill"
#elif EXCEPTION_ACTION == "enter_at_handler"
#elif EXCEPTION_ACTION == "resume"
#else
#error "EXCEPTION_ACTION must be one of [kill, enter_at_handler, resume]"
#endif
        return EXCEPTION_UNHANDLED;
    }
}

