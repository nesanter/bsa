#ifndef BYTECODE_H
#define BYTECODE_H

typedef unsigned char bytecode_type;
const bytecode_type BCT_NOP               = 0;
const bytecode_type BCT_ADD               = 1;
const bytecode_type BCT_SUBTRACT          = 2;
const bytecode_type BCT_MULTIPLY          = 3;
const bytecode_type BCT_DIVIDE            = 4;
const bytecode_type BCT_MODULO            = 5;
const bytecode_type BCT_NEGATE            = 6;
const bytecode_type BCT_AND               = 7;
const bytecode_type BCT_OR                = 8;
const bytecode_type BCT_XOR               = 9;
const bytecode_type BCT_NOT               = 10;
const bytecode_type BCT_DISCARD           = 11
const bytecode_type BCT_DUPLICATE         = 12;
const bytecode_type BCT_RETURN            = 13;
const bytecode_type BCT_YIELD             = 14;
const bytecode_type BCT_LOAD              = 128;
const bytecode_type BCT_STORE             = 129;
const bytecode_type BCT_CONSTANT          = 130;
const bytecode_type BCT_JUMP              = 131;
const bytecode_type BCT_BRANCH_IF_TRUE    = 132;
const bytecode_type BCT_RAISE             = 133;
const bytecode_type BCT_CALL_USER         = 134;
const bytecode_type BCT_CALL_LIB          = 135;
const bytecode_type BCT_BLOCK             = 136;
const bytecode_type BCT_FORK              = 137;
const bytecode_type BCT_SCOPE_ALWAYS      = 138;
const bytecode_type BCT_SCOPE_FAILURE     = 139;
const bytecode_type BCT_SCOPE_SUCCESS     = 140;

struct bytecode {
    bytecode_type type;
    void *arg;
};

#endif /* BYTECODE_H */
