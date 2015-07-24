#ifndef EXCEPTION_H
#define EXCEPTION_H

#define EXCEPTION_ACTION "resume"

struct eh_t {
    struct eh_t *parent;
    unsigned int flags;
    void (*always_handler)();
    void (*success_handler)();
    void (*failure_handler)(int ex_info);
};

void set_current_eh_t(struct eh_t *ptr);
int throw_exception(int info);

#endif /* EXCEPTION_H */
