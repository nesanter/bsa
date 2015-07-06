#ifndef VM_H
#define VM_H

#include "vm/bytecode.h"

const unsigned int VM_TASK_MAX = 8;
const unsigned int VM_STACK_SIZE = 128;
const unsigned int SYMT_MAX_SYMBOLS = 256;

struct bytecode read_bytecode(unsigned char *bytecode_data, unsigned int length);

typedef int bl_token_type;
typedef void* token_t;
const bl_token_type TOKT_UNDEFINED = -1;
const bl_token_type TOKT_NUMERIC = 0;
const bl_token_type TOKT_OBJECT = 1;
const bl_token_type TOKT_ADDRESS = 2;

struct bl_stack_token {
    bl_token_type type;
    token_t value;
}

typedef int bl_block_type;
const bl_block_type BLOCKT_NONE = 0;
const bl_block_type BLOCKT_WEAK = 1;
const bl_block_type BLOCKT_HARD = 2;

struct bl_block_reason {
    bl_block_type type;
    int *block;
};

typedef int bl_task_status;
const bl_task_status TASK_DEAD = 0;
const bl_task_status TASK_READY = 1;
const bl_task_status TASK_ACTIVE = 2;
const bl_task_status TASK_BLOCK = 3;
const bl_task_status TASK_ERROR = -1;

struct bl_task {
    bl_task_status status;
    struct bl_block_reason block_reason;
    bl_stack_token stack[VM_STACK_SIZE];
    unsigned int stack_size;
    unsigned char *ip;
};

struct bl_machine {
    unsigned char *program;
    unsigned int program_length;

    struct bl_task tasks[VM_TASK_MAX];
    int active_task;
};

struct entry {
    bl_token_type type;
    token_t value;
};


#endif /* VM_H */
