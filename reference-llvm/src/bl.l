%option noyywrap yylineno

%{
    #include "bl.h"
    #include "bl.tab.h"
%}

%%

 /* comments */
#.*$

 /* special symbols */
"{" { return LBRACE; }
"}" { return RBRACE; }
"(" { return LPAREN; }
")" { return RPAREN; }
"[" { return LBRACK; }
"]" { return RBRACK; }
"." { return DOT; }
";" { return SEMI; }
"," { return COMMA; }
"=" { return EQUAL; }
"#" { return POUND; }

 /* operators */
"+" { return PLUS; }
"-" { return MINUS; }
"*" { return STAR; }
"%" { return PERCENT; }
"/" { return FSLASH; }
">=" { return LANGLE_EQUAL; }
">" { return LANGLE; }
"<=" { return RANGLE_EQUAL; }
"<" { return RANGLE; }
"==" { return EQUAL_EQUAL; }

 /* keywords */
"not" { return NOT; }
"and" { return AND; }
"or" { return OR; }
"xor" { return XOR; }
"function" { return FUNCTION; }
"while" { return WHILE; }
"if" { return IF; }
"else" { return ELSE; }
"block" { return BLOCK; }
"yield" { return YIELD; }
 /* "return" { return RETURN; }*/
"fork" { return FORK; }
"scope" { return SCOPE; }
"always" { return ALWAYS; }
"success" { return SUCCESS; }
"failure" { return FAILURE; }

 /* strings */
\"[^"].*\" { yylval.text = strdup(yytext); return STRING; }

 /* numeric constants */
[0-9a-fA-F]+h { yylval.llu = strtoull(yytext, NULL, 16); return NUMERIC; }
[01]+b { yylval.llu = strtoull(yytext, NULL, 2); return NUMERIC; }
[0-9]+(d)? { yylval.llu = strtoull(yytext, NULL, 10); return NUMERIC; }

 /* textual identifiers */
[a-zA-Z][a-zA-Z0-9._]* { yylval.text = strdup(yytext); return IDENT; }

 /* whitespace */
[ \t\n]+

 /* catch-all */
. { fprintf(stderr, "Lexical error: unexpected symbol (line %d)\n", yylineno); }

%%

