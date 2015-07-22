#ifndef BL_H
#define BL_H

void yyerror(char *s, ...);
int error_occured;

unsigned long expr_atom_ident(char *s);
unsigned long expr_atom_numeric(unsigned long n);
unsigned long expr_atom_function_call();
unsigned long expr_atom_bool(int is_true);
unsigned long expr_atom_syscall(char *ident, unsigned long qident_ref, unsigned long params_ref);
unsigned long expr_op_lor(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_lxor(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_land(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_eq(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_lt(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_gt(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_lte(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_gte(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_add(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_sub(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_mul(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_sdiv(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_mod(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_write(unsigned long qident_ref, unsigned long rhs_ref);
unsigned long expr_op_lnot(unsigned long lhs_ref);
unsigned long expr_op_neg(unsigned long lhs_ref);
unsigned long expr_op_pos(unsigned long lhs_ref);

void create_function_call(char *ident, unsigned long params_ref);

void statement_expression(unsigned long expr_ref);
void statement_assign(char *lhs, unsigned long rhs_ref);
void statement_return_void();
void statement_return_expr(unsigned long rhs_ref);

unsigned long statement_if_begin(unsigned long cond_ref);
void statement_if_break(unsigned long ifelse_ref);
unsigned long statement_if_end(unsigned long ifelse_ref, unsigned long long nested_ref);
unsigned long statement_else_terminal();

unsigned long statement_while_begin();
unsigned long statement_while_begin_do(unsigned long loop_ref);
void statement_while_end(unsigned long loop_ref);

void statement_yield();

void statement_fork(char *ident);

void function_begin(char *ident, unsigned long args_ref, int returns_object);
void function_end();

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

void global_create(char *ident, int value);

#endif /* BL_H */
