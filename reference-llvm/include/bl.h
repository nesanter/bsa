//#ifndef BL_H
//#define BL_H

/*
struct {
    int value;
    int is_bool;
} tagged_int32;
*/

typedef unsigned long ref;

void yyerror(const char *s, ...);
void flex_error(char *s, int length);
int error_occured;

ref expr_atom_ident(char *s);
ref expr_atom_numeric(unsigned long n);
ref expr_atom_function_call();
ref expr_atom_bool(int is_true);
ref expr_atom_syscall(char *ident, ref qident_ref, ref params_ref);
ref expr_op_lor(ref lhs_ref, ref rhs_ref);
ref expr_op_lxor(ref lhs_ref, ref rhs_ref);
ref expr_op_land(ref lhs_ref, ref rhs_ref);
ref expr_op_bor(ref lhs_ref, ref rhs_ref);
ref expr_op_bxor(ref lhs_ref, ref rhs_ref);
ref expr_op_band(ref lhs_ref, ref rhs_ref);
ref expr_op_shl(ref lhs_ref, ref rhs);
ref expr_op_shrl(ref lhs_ref, ref rhs);
ref expr_op_shra(ref lhs_ref, ref rhs);
ref expr_op_is(ref lhs_ref, ref rhs_ref);
ref expr_op_nis(ref lhs_ref, ref rhs_ref);
ref expr_op_eq(ref lhs_ref, ref rhs_ref);
ref expr_op_neq(ref lhs_ref, ref rhs_ref);
ref expr_op_lt(ref lhs_ref, ref rhs_ref);
ref expr_op_gt(ref lhs_ref, ref rhs_ref);
ref expr_op_lte(ref lhs_ref, ref rhs_ref);
ref expr_op_gte(ref lhs_ref, ref rhs_ref);
ref expr_op_add(ref lhs_ref, ref rhs_ref);
ref expr_op_sub(ref lhs_ref, ref rhs_ref);
ref expr_op_mul(ref lhs_ref, ref rhs_ref);
ref expr_op_sdiv(ref lhs_ref, ref rhs_ref);
ref expr_op_mod(ref lhs_ref, ref rhs_ref);
ref expr_op_write(ref qident_ref, ref rhs_ref);
ref expr_op_lnot(ref lhs_ref);
ref expr_op_binv(ref lhs_ref);
ref expr_op_neg(ref lhs_ref);
ref expr_op_pos(ref lhs_ref);

void create_function_call(char *ident, ref params_ref);

void statement_empty();
void statement_expression(ref expr_ref);
void statement_assign(char *lhs, ref rhs_ref);
void statement_return_void();
void statement_return_expr(ref rhs_ref);

ref statement_if_begin(ref cond_ref);
void statement_if_break(ref ifelse_ref);
ref statement_if_end(ref ifelse_ref, ref nested_ref);
ref statement_else_terminal();

ref statement_while_begin();
ref statement_while_begin_do(ref loop_ref);
void statement_while_end(ref loop_ref);

void statement_yield();
void statement_yield_for(ref expr_ref);

enum {
    HANDLER_ALWAYS = 0,
    HANDLER_SUCCESS = 1,
    HANDLER_FAILURE = 2
};

void statement_scope(int type, char * ident);

void statement_fork(char *ident);

void statement_hidden_fail(void);
void statement_hidden_trace(void);
void statement_hidden_canary(void);

void function_begin(char *ident, unsigned long args_ref, unsigned long attr);
void statement_sync(int read, int write);

void function_end();

unsigned long attribute_value(unsigned long cur, char *ident);

unsigned long args_empty();
unsigned long args_create(char *ident, int is_object);
unsigned long args_add(unsigned long args_ref, char *ident, int is_object);

unsigned long params_empty();
unsigned long params_create(unsigned long expr_ref);
unsigned long params_create_string(char *s);
unsigned long params_add(unsigned long params_ref, unsigned long expr_ref);
unsigned long params_add_string(unsigned long params_ref, char *s);

unsigned long qident_create(char *ident);
unsigned long qident_add(unsigned long qident_ref, char *ident);

char *escape_string(char *s);

void global_create(char *ident, int value, int is_bool);
void constant_create(char *ident, int value, int is_bool);

//#endif /* BL_H */
