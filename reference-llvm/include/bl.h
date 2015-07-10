#ifndef BL_H
#define BL_H

void yyerror(char *s, ...);
int error_occured;

unsigned long expr_atom_ident(char *s);
unsigned long expr_atom_numeric(unsigned long n);
unsigned long expr_op_lor(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_lxor(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_land(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_e(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_lt(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_gt(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_lte(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_gte(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_add(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_sub(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_mul(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_div(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_mod(unsigned long lhs_ref, unsigned long rhs_ref);
unsigned long expr_op_lnot(unsigned long lhs_ref);
unsigned long expr_op_neg(unsigned long lhs_ref);
unsigned long expr_op_pos(unsigned long lhs_ref);


#endif /* BL_H */
