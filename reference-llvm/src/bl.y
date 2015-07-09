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
%token DOT SEMI COMMA EQUAL
%token FUNCTION WHILE IF ELSE BLOCK YIELD RETURN FORK SCOPE ALWAYS SUCCESS FAILURE

%left OR
%left XOR
%left AND
%left EQUAL_EQUAL
%left LANGLE RANGLE LANGLE_EQUAL RANGLE_EQUAL
%left PLUS MINUS
%left STAR FSLASH PERCENT
%right NOT
%precedence UNARY

%%

file: %empty
    | function_def file
    | global_def file
    ;

function_def: FUNCTION IDENT LPAREN args RPAREN LBRACE body RBRACE
            ;

global_def: IDENT EQUAL constant_expression SEMI
          ;

args: %empty
    | IDENT
    | args COMMA IDENT
    ;

body: statement
    | body statement
    ;

statement: expression SEMI
         | assign_expression SEMI
         | if_statement
         | while_statement
         | scope_statement
         | block_statement SEMI
         | yield_statement SEMI
         | return_statement SEMI
         | fork_statement SEMI
         ;

if_statement: IF LPAREN expression RPAREN LBRACE body RBRACE else_statement
            ;

else_statement: %empty
              | ELSE LBRACE body RBRACE
              | ELSE if_statement
                ;

while_statement: WHILE LPAREN expression RPAREN LBRACE body RBRACE
               ;

scope_statement: SCOPE scope_type LBRACE body RBRACE
               ;

scope_type: ALWAYS
          | SUCCESS
          | FAILURE
          ;

block_statement: BLOCK IDENT
               ;

yield_statement: YIELD
               ;

return_statement: RETURN
                | RETURN expression
                ;

fork_statement: FORK IDENT
              ;

assign_expression: IDENT EQUAL STRING
                 | IDENT EQUAL expression
                 ;

expression: atom
          | LPAREN expression RPAREN
          | expression OR expression 
          | expression XOR expression
          | expression AND expression
          | expression EQUAL_EQUAL expression
          | expression LANGLE expression
          | expression RANGLE expression
          | expression LANGLE_EQUAL expression
          | expression RANGLE_EQUAL expression
          | expression PLUS expression
          | expression MINUS expression
          | expression STAR expression
          | expression FSLASH expression
          | expression PERCENT expression
          | NOT expression %prec UNARY
          | MINUS expression %prec UNARY
          | PLUS expression %prec UNARY
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

atom: IDENT
    | NUMERIC
    | function_call
    ;

function_call: IDENT LPAREN params RPAREN
             ;

params: %empty
      | expression
      | STRING
      | args COMMA expression
      | args COMMA STRING
      ;

%%
