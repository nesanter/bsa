#ifndef RUNTIME_H
#define RUNTIME_H

typedef int (*driver_write_fn)(int val, char *str);
typedef int (*driver_read_fn)();
typedef int (*driver_block_fn)();
typedef int (*driver_write_addr_fn)(int val, int addr);
typedef int (*driver_read_addr_fn)(int addr);

typedef void (*handler_t)(void);
void runtime_set_vector_table_entry(unsigned int entry, handler_t handler);

#endif /* RUNTIME_H */
