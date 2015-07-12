%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include "bl.h"
%}

%union {
    uint64_t llu;
    char *text;
    unsigned long refid;
}

%token <llu> NUMERIC
%token <text> IDENT STRING
%token LBRACE RBRACE LPAREN RPAREN LBRACK RBRACK
%token DOT SEMI COMMA EQUAL POUND
%token FUNCTION WHILE DO IF ELSE BLOCK YIELD FORK SCOPE ALWAYS SUCCESS FAILURE

%precedence PAREN
%left OR
%left XOR
%left AND
%left EQUAL_EQUAL
%left LANGLE RANGLE LANGLE_EQUAL RANGLE_EQUAL
%left PLUS MINUS
%left STAR FSLASH PERCENT
%right NOT
%precedence UNARY

%type <refid> atom expression args else_statement if_statement params qualified_ident system_call

%%

file: %empty
    | function_def file
    | global_def file
    ;

function_def: function_signature LBRACE body RBRACE { function_end(); }
            ;

function_signature: FUNCTION IDENT LPAREN args RPAREN { function_begin($2, $4, 0); }
                  | FUNCTION POUND IDENT LPAREN args RPAREN { function_begin($3, $5, 1); }
                  ;

global_def: IDENT EQUAL constant_expression SEMI
          ;

args: %empty { $$ = args_empty(); }
    | IDENT { $$ = args_create($1, 0); }
    | POUND IDENT { $$ = args_create($2, 1); }
    | args COMMA IDENT { $$ = args_add($1, $3, 0); }
    | args COMMA POUND IDENT { $$ = args_add($1, $4, 1); }
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
         | block_statement SEMI
         | yield_statement SEMI
/*         | return_statement SEMI */
         | fork_statement SEMI
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

scope_statement: SCOPE scope_type LBRACE body RBRACE
               ;

scope_type: ALWAYS
          | SUCCESS
          | FAILURE
          ;

block_statement: BLOCK IDENT
               ;

yield_statement: YIELD { statement_yield(); }
               ;

/*
return_statement: RETURN { statement_return_void(); }
                | RETURN expression { statement_return_expr($2); }
                ;
*/

fork_statement: FORK IDENT { statement_fork($2); }
              ;

assign_statement: IDENT EQUAL STRING { /* do nothing */ }
                | IDENT EQUAL expression { statement_assign($1, $3); }
                ;

expression: atom { $$ = $1; }
          | LPAREN expression RPAREN %prec PAREN { $$ = $2; }
          | expression OR expression { $$ = expr_op_lor($1, $3); }
          | expression XOR expression { $$ = expr_op_lxor($1, $3); }
          | expression AND expression { $$ = expr_op_land($1, $3); }
          | expression EQUAL_EQUAL expression { $$ = expr_op_eq($1, $3); }
          | expression LANGLE expression { $$ = expr_op_lt($1, $3); }
          | expression RANGLE expression { $$ = expr_op_gt($1, $3); }
          | expression LANGLE_EQUAL expression { $$ = expr_op_lte($1, $3); }
          | expression RANGLE_EQUAL expression { $$ = expr_op_gte($1, $3); }
          | expression PLUS expression { $$ = expr_op_add($1, $3); }
          | expression MINUS expression { $$ = expr_op_sub($1, $3); }
          | expression STAR expression { $$ = expr_op_mul($1, $3); }
          | expression FSLASH expression { $$ = expr_op_sdiv($1, $3); }
          | expression PERCENT expression { $$ = expr_op_mod($1, $3); }
          | NOT expression %prec UNARY { $$ = expr_op_lnot($2); }
          | MINUS expression %prec UNARY { $$ = expr_op_neg($2); }
          | PLUS expression %prec UNARY { $$ = expr_op_pos($2); }
          ;

constant_expression: constant_atom
          | LPAREN constant_expression RPAREN
          | constant_expression OR constant_expression
          | constant_expression XOR constant_expression
          | constant_expression AND constant_expression
          | constant_expression EQUAL_EQUAL constant_expression
          | constant_expression LANGLE constant_expression
          | constant_expression RANGLE constant_expression
          | constant_expression LANGLE_EQUAL constant_expression
          | constant_expression RANGLE_EQUAL constant_expression
          | constant_expression PLUS constant_expression
          | constant_expression MINUS constant_expression
          | constant_expression STAR constant_expression
          | constant_expression FSLASH constant_expression
          | constant_expression PERCENT constant_expression
          | NOT constant_expression %prec UNARY
          | MINUS constant_expression %prec UNARY
          | PLUS constant_expression %prec UNARY
          ;

constant_atom: NUMERIC
             | STRING
             ;

atom: IDENT { $$ = expr_atom_ident($1); if (error_occured) YYABORT; }
    | NUMERIC { $$ = expr_atom_numeric($1); }
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
           | LBRACK IDENT params COMMA qualified_ident RBRACK { $$ = expr_atom_syscall($2, $5, $3); }
           ;

qualified_ident: DOT IDENT { $$ = qident_create($2); }
               | qualified_ident DOT IDENT { $$ = qident_add($1, $3); }
               ;

%%
