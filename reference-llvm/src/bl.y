%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include "bl.h"

    // forward declare to suppress error
    int yylex();
%}

%union {
    uint64_t llu;
    int32_t d;
    char *text;
    unsigned long refid;
}

%define parse.error verbose

%token <llu> NUMERIC
%token <text> IDENT STRING
%token LBRACE RBRACE LPAREN RPAREN LBRACK RBRACK
%token DOT SEMI COMMA EQUAL AT
%token FUNCTION WHILE DO IF ELSE YIELD FORK SYNC_BOTH SYNC_READ SYNC_WRITE
%token SCOPE ALWAYS SUCCESS FAILURE
%token TRUE FALSE

%precedence PAREN
%left IS BANG_IS
%left OR
%left XOR
%left AND
%left VERTICAL_BAR
%left CARET
%left AMPERSAND
%left EQUAL_EQUAL BANG_EQUAL
%left LANGLE RANGLE LANGLE_EQUAL RANGLE_EQUAL
%left LANGLE_LANGLE RANGLE_RANGLE RANGLE_RANGLE_RANGLE
%left PLUS MINUS
%left STAR FSLASH PERCENT
%right NOT TILDE
%precedence UNARY

%type <llu> attributes
%type <refid> atom expression args else_statement if_statement params params_s qualified_ident system_call
%type <d> constant_atom constant_expression scope_type

%%

file: %empty
    | function_def file
    | global_def file
    ;

function_def: function_signature LBRACE body RBRACE { function_end(); }
            ;

function_signature: FUNCTION IDENT LPAREN args RPAREN { function_begin($2, $4, 0); }
                  | FUNCTION attributes IDENT LPAREN args RPAREN { function_begin($3, $5, $2); }
/*                  | FUNCTION POUND IDENT LPAREN args RPAREN { function_begin($3, $5, 1); } */
                  ;

attributes: AT IDENT { $$ = attribute_value(0, $2); }
          | attributes AT IDENT { $$ = attribute_value($1, $3); }
          ;

global_def: IDENT EQUAL constant_expression SEMI { global_create($1, $3); }
          ;

args: %empty { $$ = args_empty(); }
    | IDENT { $$ = args_create($1, 0); }
/*    | POUND IDENT { $$ = args_create($2, 1); } */
    | args COMMA IDENT { $$ = args_add($1, $3, 0); }
/*    | args COMMA POUND IDENT { $$ = args_add($1, $4, 1); } */
    ;

body: statement
    | body statement
    ;

statement: SEMI /* "empty" statement */
         | expression SEMI { statement_expression($1); }
         | assign_statement SEMI
         | if_statement
         | while_statement
         | scope_statement
/*         | block_statement SEMI */
         | yield_statement SEMI
/*         | return_statement SEMI */
         | fork_statement SEMI
         | sync_statement SEMI
         ;

if_statement: IF LPAREN expression RPAREN { $<refid>$ = statement_if_begin($3); } LBRACE body RBRACE { statement_if_break($<refid>5); } else_statement { $$ = statement_if_end($<refid>5, $10); }
            ;

else_statement: %empty { $$ = statement_else_terminal(); }
              | ELSE LBRACE body RBRACE { $$ = statement_else_terminal(); }
              | ELSE if_statement { $$ = $2; }
              ;

/*
while_statement: WHILE LPAREN expression RPAREN { $<refid>$ = statement_while_begin($3); } LBRACE body RBRACE { statement_while_end($<refid>5); }
               ;
*/

while_statement: WHILE { $<refid>$ = statement_while_begin(); } LBRACE body RBRACE { $<refid>$ = statement_while_begin_do($<refid>2); } DO LBRACE body RBRACE { statement_while_end($<refid>6); }
               ;

/*
scope_statement: SCOPE LPAREN scope_type RPAREN { statement_scope_begin($3); } LBRACE body RBRACE { statement_scope_end(); }
               ;
*/

scope_statement: SCOPE LPAREN scope_type RPAREN IDENT { statement_scope($3, $5); }
               ;

scope_type: ALWAYS { $$ = HANDLER_ALWAYS; }
          | SUCCESS { $$ = HANDLER_SUCCESS; }
          | FAILURE { $$ = HANDLER_FAILURE; }
          ;

/*
block_statement: BLOCK IDENT
               ;
*/

yield_statement: YIELD { statement_yield(); }
               ;

/*
return_statement: RETURN { statement_return_void(); }
                | RETURN expression { statement_return_expr($2); }
                ;
*/

fork_statement: FORK IDENT { statement_fork($2); }
              ;

sync_statement: SYNC_BOTH { statement_sync(1, 1); }
              | SYNC_READ { statement_sync(1, 0); }
              | SYNC_WRITE { statement_sync(0, 1); }
              ;

assign_statement: IDENT EQUAL expression { statement_assign($1, $3); }
                ;

expression: atom { $$ = $1; }
          | LPAREN expression RPAREN %prec PAREN { $$ = $2; }
          | expression OR expression { $$ = expr_op_lor($1, $3); }
          | expression XOR expression { $$ = expr_op_lxor($1, $3); }
          | expression AND expression { $$ = expr_op_land($1, $3); }
          | expression VERTICAL_BAR expression { expr_op_bor($1, $3); }
          | expression CARET expression { expr_op_bxor($1, $3); }
          | expression AMPERSAND expression { expr_op_band($1, $3); }
          | expression IS expression { $$ = expr_op_is($1, $3); }
          | expression BANG_IS expression { $$ = expr_op_nis($1, $3); }
          | expression EQUAL_EQUAL expression { $$ = expr_op_eq($1, $3); }
          | expression BANG_EQUAL expression { $$ = expr_op_neq($1, $3); }
          | expression LANGLE expression { $$ = expr_op_lt($1, $3); }
          | expression RANGLE expression { $$ = expr_op_gt($1, $3); }
          | expression LANGLE_EQUAL expression { $$ = expr_op_lte($1, $3); }
          | expression RANGLE_EQUAL expression { $$ = expr_op_gte($1, $3); }
          | expression LANGLE_LANGLE expression { $$ = expr_op_shl($1, $3); }
          | expression RANGLE_RANGLE expression { $$ = expr_op_shrl($1, $3); }
          | expression RANGLE_RANGLE_RANGLE expression { $$ = expr_op_shra($1, $3); }
          | expression PLUS expression { $$ = expr_op_add($1, $3); }
          | expression MINUS expression { $$ = expr_op_sub($1, $3); }
          | expression STAR expression { $$ = expr_op_mul($1, $3); }
          | expression FSLASH expression { $$ = expr_op_sdiv($1, $3); }
          | expression PERCENT expression { $$ = expr_op_mod($1, $3); }
          | NOT expression { $$ = expr_op_lnot($2); }
          | TILDE expression { $$ = expr_op_binv($2); }
          | MINUS expression %prec UNARY { $$ = expr_op_neg($2); }
          | PLUS expression %prec UNARY { $$ = expr_op_pos($2); }
          ;

constant_expression: constant_atom { $$ = $1; }
          | LPAREN constant_expression RPAREN { $$ = $2; }
          | constant_expression OR constant_expression { $$ = (($1 != 0) || ($3 != 0)) ? 1 : 0; }
          | constant_expression XOR constant_expression { $$ = ((($1 != 0) ? 1 : 0) != (($3 != 0) ? 1 : 0)) ? 1 : 0; }
          | constant_expression AND constant_expression { $$ = (($1 != 0) && ($3 != 0)) ? 1 : 0; }
          | constant_expression EQUAL_EQUAL constant_expression { $$ = ($1 == $3) ? 1 : 0; }
          | constant_expression LANGLE constant_expression { $$ = ($1 < $3) ? 1 : 0; }
          | constant_expression RANGLE constant_expression { $$ = ($1 > $3) ? 1 : 0; }
          | constant_expression LANGLE_EQUAL constant_expression { $$ = ($1 <= $3) ? 1 : 0; }
          | constant_expression RANGLE_EQUAL constant_expression { $$ = ($1 >= $3) ? 1 : 0; }
          | constant_expression PLUS constant_expression { $$ = $1 + $3; }
          | constant_expression MINUS constant_expression { $$ = $1 - $3; }
          | constant_expression STAR constant_expression { $$ = $1 * $3; }
          | constant_expression FSLASH constant_expression { $$ = $1 / $3; }
          | constant_expression PERCENT constant_expression { $$ = $1 % $3; }
          | NOT constant_expression %prec UNARY { $$ = ($2 != 0) ? 0 : 1; }
          | MINUS constant_expression %prec UNARY { $$ = -$2; }
          | PLUS constant_expression %prec UNARY { $$ = $2; }
          ;

constant_atom: NUMERIC { $$ = $1; }
             ;

atom: IDENT { $$ = expr_atom_ident($1); if (error_occured) YYABORT; }
    | NUMERIC { $$ = expr_atom_numeric($1); }
    | TRUE { $$ = expr_atom_bool(1); }
    | FALSE { $$ = expr_atom_bool(0); }
    | function_call { $$ = expr_atom_function_call(); }
    | system_call { $$ = $1; }
    ;

function_call: IDENT LPAREN RPAREN { create_function_call($1, params_empty()); }
             | IDENT LPAREN params RPAREN { create_function_call($1, $3); }
             ;

params: expression { $$ = params_create($1); }
/*      | STRING */
      | params COMMA expression { $$ = params_add($1, $3); }
/*      | params COMMA STRING */
      ;

system_call: LBRACK IDENT qualified_ident RBRACK { $$ = expr_atom_syscall($2, $3, params_empty()); }
           | LBRACK IDENT params_s COMMA qualified_ident RBRACK { $$ = expr_atom_syscall($2, $5, $3); }
           ;

params_s: expression { $$ = params_create($1); }
        | STRING { $$ = params_create_string($1); }
        | params_s COMMA expression { $$ = params_add($1, $3); }
        | params_s COMMA STRING { $$ = params_add_string($1, $3); }
        ;

qualified_ident: DOT IDENT { $$ = qident_create($2); }
               | qualified_ident DOT IDENT { $$ = qident_add($1, $3); }
               ;

%%
