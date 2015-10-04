#ifndef EXCEPTION_H
#define EXCEPTION_H

#define EXCEPTION_ACTION_RESUME (0)
#define EXCEPTION_ACTION_AT_HANDLER (1)
#define EXCEPTION_ACTION_KILL (2)

#define EXCEPTION_ACTION EXCEPTION_ACTION_RESUME

#define EXCEPTION_HANDLED (0)
#define EXCEPTION_UNHANDLED (1)

struct eh_t {
    struct eh_t *parent;
    char *ident;
    unsigned int flags;
    void (*always_handler)();
    void (*success_handler)();
    void (*failure_handler)(int ex_info);
};

void set_current_eh_t(struct eh_t *ptr);
int throw_exception(int info);
void throw_global_exception(int info);

#endif /* EXCEPTION_H */
