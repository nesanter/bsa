#include "vm/vm.h"

struct bl_machine vm;

struct entry entry_table[SYMT_MAX_SYMBOLS];

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

    struct entry ent;
    int n;

    switch (bc.type) {
        case BCT_NOP:
            break;
        case BCT_ADD:
#ifdef VM_STACK_CHECK
            if (sz < 2)
                return 1;
#endif
#ifdef VM_TYPE_CHECK
            if (stack[sz-1].type != TOKT_NUMERIC || stack[sz-2].type != TOKT_NUMERIC)
                return 1;
#endif
            n = (int)stack[sz-1].value + (int)stack[sz-2].value;
            stack[--sz].value = (token_t)n;
            break;
        case BCT_SUBTRACT:
#ifdef VM_STACK_CHECK
            if (sz < 2)
                return 1;
#endif
#ifdef VM_TYPE_CHECK
            if (stack[sz-1].type != TOKT_NUMERIC || stack[sz-2].type != TOKT_NUMERIC)
                return 1;
#endif
            n = (int)stack[sz-2].value - (int)stack[sz-1].value;
            stack[--sz].value = (token_t)n;
            break;
        case BCT_MULTIPLY:
#ifdef VM_STACK_CHECK
            if (sz < 2)
                return 1;
#endif
#ifdef VM_TYPE_CHECK
            if (stack[sz-1].type != TOKT_NUMERIC || stack[sz-2].type != TOKT_NUMERIC)
                return 1;
#endif
            n = (int)stack[sz-1].value * (int)stack[sz-2].value;
            stack[--sz].value = (token_t)n;
            break;
        case BCT_DIVIDE:
#ifdef VM_STACK_CHECK
            if (sz < 2)
                return 1;
#endif
#ifdef VM_TYPE_CHECK
            if (stack[sz-1].type != TOKT_NUMERIC || stack[sz-2].type != TOKT_NUMERIC)
                return 1;
#endif
            n = (int)stack[sz-2].value / (int)stack[sz-1].value;
            stack[--sz].value = (token_t)n;
            break;
        case BCT_MODULO:
#ifdef VM_STACK_CHECK
            if (sz < 2)
                return 1;
#endif
#ifdef VM_TYPE_CHECK
            if (stack[sz-1].type != TOKT_NUMERIC || stack[sz-2].type != TOKT_NUMERIC)
                return 1;
#endif
            n = (int)stack[sz-2].value % (int)stack[sz-1].value;
            stack[--sz].value = (token_t)n;
            break;
        case BCT_NEGATE:
#ifdef VM_STACK_CHECK
            if (sz < 1)
                return 1;
#endif
#ifdef VM_TYPE_CHECK
            if (stack[sz-1].type != TOKT_NUMERIC)
                return 1;
#endif
            n = -(int)stack[sz-1].value;
            stack[sz-1].value = (token_t)n;
            break;
        case BCT_AND:
#ifdef VM_STACK_CHECK
            if (sz < 2)
                return 1;
#endif
#ifdef VM_TYPE_CHECK
            if (stack[sz-1].type != TOKT_NUMERIC || stack[sz-2].type != TOKT_NUMERIC)
                return 1;
#endif
            n = (int)stack[sz-1].value && (int)stack[sz-2].value;
            stack[--sz].value = (token_t)n;
            break;
        case BCT_OR:
#ifdef VM_STACK_CHECK
            if (sz < 2)
                return 1;
#endif
#ifdef VM_TYPE_CHECK
            if (stack[sz-1].type != TOKT_NUMERIC || stack[sz-2].type != TOKT_NUMERIC)
                return 1;
#endif
            n = (int)stack[sz-1].value + (int)stack[sz-2].value;
            stack[--sz].value = (token_t)n;
            break;
        case BCT_XOR:
#ifdef VM_STACK_CHECK
            if (sz < 2)
                return 1;
#endif
#ifdef VM_TYPE_CHECK
            if (stack[sz-1].type != TOKT_NUMERIC || stack[sz-2].type != TOKT_NUMERIC)
                return 1;
#endif
            n = ((int)stack[sz-1].value || (int)stack[sz-2].value) && !((int)stack[sz-1].value && (int)stack[sz-2].value);
            stack[--sz].value = (token_t)n;
            break;
        case BCT_NOT:
#ifdef VM_STACK_CHECK
            if (sz < 1)
                return 1;
#endif
#ifdef VM_TYPE_CHECK
            if (stack[sz-1].type != TOKT_NUMERIC)
                return 1;
#endif
            n = !(int)stack[sz-1].value;
            stack[sz-1].value = (token_t)n;
            break;
        case BCT_DISCARD:
#ifdef VM_STACK_CHECK
            if (sz < 1)
                return 1;
#endif
            sz--;
            break;
        case BCT_DUPLICATE:
#ifdef VM_STACK_CHECK
            if (sz < 1 || sz == VM_STACK_SIZE)
                return 1;
#endif
            stack[sz] = stack[sz-1];
            sz++;
            break;
        case BCT_RETURN:
#ifdef VM_STACK_CHECK
            if (sz < 1)
                return 1;
#endif
            while (stack[--sz].type != TOKT_ADDRESS) {
#ifdef VM_STACK_CHECK
                if (sz < 1)
                    return 1;
#endif
            }
            vm.task[active_task].ip = (unsigned char *)stack[sz--].value;
            break;
        case BCT_YIELD:
            break;
        case BCT_END_SCOPE:
            break;
        case BCT_LOAD:
#ifdef VM_STACK_CHECK
            if (sz == VM_STACK_SIZE)
                return 1;
#endif
            ent = entry_table[(unsigned int)bc.arg];
            stack[sz].value = ent.value;
            stack[sz].type = ent.type;
            sz++;
            break;
        case BCT_STORE:
#ifdef VM_STACK_CHECK
            if (sz < 1)
                return 1;
#endif
            sz--;
            ent.value = stack[sz].value;
            ent.type = stack[sz].type;
            entry_table[(unsigned int)bc.arg] = ent;
            break;
        case BCT_CONSTANT:
#ifdef VM_STACK_CHECK
            if (sz == VM_STACK_SIZE)
                return 1;
#endif
            stack[sz].value = (token_t)bc.arg;
            stack[sz].type = TOKT_NUMERIC;
            break;
        case BCT_JUMP:
            vm.tasks[active_task].ip = (unsigned char *)bc.arg;
            break;
        case BCT_BRANCH_IF_TRUE:
#ifdef VM_STACK_CHECK
            if (sz < 1)
                return 1;
#endif
#ifdef VM_TYPE_CHECK
            if (stack[sz-1].type != TOKT_NUMERIC)
                return 1;
#endif
            if (stack[--sz].value) {
                vm.tasks[active_task].ip = (unsigned char *)bc.arg;
            }
            break;
        case BCT_RAISE:
            break;
        case BCT_CALL_USER:
#ifdef VM_STACK_CHECK
            if (sz == VM_STACK_SIZE)
                return 1;
#endif
            stack[sz].value = vm.tasks[active_task].ip;
            stack[sz].type = TOKT_ADDRESS;
            sz++;
            vm.tasks[active_task].ip = (unsigned char *)bc.arg;
            break;
        case BCT_CALL_LIB:
            break;
        case BCT_BLOCK:
            break;
        case BCT_FORK:
            break;
        case BCT_SCOPE_ALWAYS:
            break;
        case BCT_SCOPE_FAILURE:
            break;
        case BCT_SCOPE_SUCCESS:
            break;
    }

    return 0;
}

int step_machine() {

}
