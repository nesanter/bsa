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

void statement_sync(int read, int write);

void function_begin(char *ident, ref args_ref, ref attr);

void function_end();

ref attribute_value(ref cur, char *ident);

ref args_empty();
ref args_create(char *ident, int is_object);
ref args_add(ref args_ref, char *ident, int is_object);

ref params_empty();
ref params_create(ref expr_ref);
ref params_create_string(char *s);
ref params_add(ref params_ref, ref expr_ref);
ref params_add_string(ref params_ref, char *s);

ref qident_create(char *ident);
ref qident_add(ref qident_ref, char *ident);

char *escape_string(char *s);

void global_create(char *ident, int value, int is_bool);
void constant_create(char *ident, int value, int is_bool);

ref expr_atom_tree_is_null(ref tree_ref);
ref expr_atom_tree_value(ref tree_ref);
ref expr_atom_tree(char *ident);
ref expr_atom_tree_sub(char *ident, ref tree_sub_ref);

void statement_tree_sub_assign(char *ident, ref tree_sub_ref, ref expr_ref);
void statement_tree_value_assign(ref tree_ref, ref expr_ref);
void statement_tree_create(ref tree_ref, ref tree_initializer_ref);
void statement_tree_release(ref tree_ref);

ref tree_sub_create(unsigned long child);
ref tree_sub_add(ref tree_sub_ref, unsigned long child);

int tree_selector(char *ident);

ref tree_initializer_null();
ref tree_initializer(ref expr_ref, int left_is_subtree, ref left_tree_ref, int right_is_subtree, ref right_tree_ref);

//#endif /* BL_H */
