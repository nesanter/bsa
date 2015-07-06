#include "vm/vm.h"

struct bl_machine vm;

int read_bytecode(unsigned char *bytecode_data, struct bytecode * bc) {
    bytecode_type bct = *((bytecode_type)bytecode_data);
    bc->type = bct;
    if (bct > 127) {
        bc->arg = *((void**)(bytecode_data+sizeof(bytecode_type)));
        return sizeof(void*) + sizeof(bytecode_type);
    } else {
        bc->arg = 0;
        return sizeof(bytecode_type);
    }
}

void init_task(struct bl_task *task) {
    task->status = TASK_EMPTY;
    task->stack_size = 0;
}

void init_machine(unsigned char *prog, unsigned int length) {
    vm.program = prog;
    vm.program_length = length;
    vm.active_task = -1;
    
    for (unsigned int i = 0 ; i < VM_TASK_MAX; i++) {
        init_task(&vm.tasks[i]);
    }
}

void select_task() {
    for (unsigned int i = 0 ; i < VM_TASK_MAX; i++) {
        if (vm.tasks[i].status == TASK_READY) {
            vm.active_task = i;
        } else if (vm.tasks[i].status == TASK_BLOCK) {
            if (vm.tasks[i].block_reason.type == BLOCKT_WEAK) {
                vm.tasks[i].status = TASK_READY;
            } else if (vm.tasks[i].block_reason.type == BLOCKT_HARD && *vm.tasks[i].block_reason.block == 0) {
                vm.tasks[i].status = TASK_READY;
                vm.active_task = i;
            }
        }
    }
}

int create_task(unsigned char *ip) {
    for (unsigned int i = 0; i < VM_TASK_MAX; i++) {
        if (vm.tasks[i].status == TASK_DEAD) {
            vm.tasks[i].status = TASK_READY;
            vm.tasks[i].stack_size = 0;
            vm.tasks[i].ip = ip;
            return 0;
        }
    }
    return 1;
}

int step_task() {
    struct bytecode bc;
    vm.tasks[vm.active_task].ip += read_bytecode(vm.tasks[vm.active_task].ip, &bc);

    unsigned int sz = vm.tasks[vm.active_task].stack_size;
    bl_stack_token *stack = vm.tasks[vm.active_task].stack;

    switch (bc.type) {
        case BCT_NOP:
            break;
        case BCT_ADD:
            int n = (int)stack[sz-1].value + (int)stack[sz-2].value;
            stack[--sz].value = n;
            break;
        case BCT_SUBTRACT:
            int n = (int)stack[sz-2].value - (int)stack[sz-1].value;
            stack[--sz].value = n;
            break;
        case BCT_MULTIPLY:
            int n = (int)stack[sz-1].value * (int)stack[sz-2].value;
            stack[--sz].value = n;
            break;
        case BCT_DIVIDE:
            int n = (int)stack[sz-2].value / (int)stack[sz-1].value;
            stack[--sz].value = n;
            break;
        case BCT_MODULO:
            int n = (int)stack[sz-2].value % (int)stack[sz-1].value;
            stack[--sz].value = n;
            break;
        case BCT_NEGATE:
            int n = -(int)stack[sz-1].value;
            stack[sz-1].value = n;
            break;
        case BCT_AND:
            int n = (int)stack[sz-1].value && (int)stack[sz-2].value;
            stack[--sz].value = n;
            break;
        case BCT_OR:
            int n = (int)stack[sz-1].value + (int)stack[sz-2].value;
            stack[--sz].value = n;
            break;
        case BCT_XOR:
            int n = ((int)stack[sz-1].value || (int)stack[sz-2].value) && !((int)stack[sz-1].value && (int)stack[sz-2].value);
            stack[--sz].value = n;
            break;
        case BCT_NOT:
            int n = !(int)stack[sz-1].value;
            stack[sz-1].value = n;
            break;
        case BCT_DISCARD:
            sz--;
            break;
        case BCT_DUPLICATE:
            stack[sz] = stack[sz-1];
            sz++;
            break;
        case BCT_RETURN:
            break;
        case BCT_YIELD:
            break;
        case BCT_LOAD:
            stack[sz++] = (int)bc.
    }
}

int step_machine() {

}
