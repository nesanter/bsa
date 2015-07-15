#ifndef RUNTIME_H
#define RUNTIME_H

typedef void (*handler_t)(void);
void runtime_set_vector_table_entry(unsigned int entry, handler_t handler);

#endif /* RUNTIME_H */
