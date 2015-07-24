%option noyywrap yylineno

%{
    #include "bl.h"
    #include "bl.tab.h"
%}

%s SCOPETYPE

%%

 /* comments */
"//".*$

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
"@" { return AT; }

 /* operators */
"+" { return PLUS; }
"-" { return MINUS; }
"*" { return STAR; }
"%" { return PERCENT; }
"/" { return FSLASH; }
">=" { return RANGLE_EQUAL; }
">" { return RANGLE; }
"<=" { return LANGLE_EQUAL; }
"<" { return LANGLE; }
"==" { return EQUAL_EQUAL; }

 /* keywords */
"not" { return NOT; }
"and" { return AND; }
"or" { return OR; }
"xor" { return XOR; }
"true" { return TRUE; }
"false" { return FALSE; }
"function" { return FUNCTION; }
"while" { return WHILE; }
"do" { return DO; }
"if" { return IF; }
"else" { return ELSE; }
"block" { return BLOCK; }
"yield" { return YIELD; }
 /* "return" { return RETURN; }*/
"fork" { return FORK; }
"sync" { return SYNC_BOTH; }
"sync_r" { return SYNC_READ; }
"sync_w" { return SYNC_WRITE; }
"scope" { BEGIN(SCOPETYPE); return SCOPE; }
<SCOPETYPE>"always" { BEGIN(INITIAL); return ALWAYS; }
<SCOPETYPE>"success" { BEGIN(INITIAL); return SUCCESS; }
<SCOPETYPE>"failure" { BEGIN(INITIAL); return FAILURE; }

 /* strings */
\"(([^"])|(\\\"))*\" { yylval.text = strdup(escape_string(yytext)); return STRING; }

 /* numeric constants */
'.' { yylval.llu = yytext[1]; return NUMERIC; }
[0-9a-fA-F]+h { yylval.llu = strtoull(yytext, NULL, 16); return NUMERIC; }
[01]+b { yylval.llu = strtoull(yytext, NULL, 2); return NUMERIC; }
[0-9]+(d)? { yylval.llu = strtoull(yytext, NULL, 10); return NUMERIC; }

 /* textual identifiers */
[a-zA-Z][a-zA-Z0-9_]* { yylval.text = strdup(yytext); return IDENT; }
_(_)?[a-zA-Z0-9]+[a-zA-Z0-9_]* { yylval.text = strdup(yytext); return IDENT; }

 /* whitespace */
[ \t\n]+

 /* specific errors */
_+ { flex_error(yytext, yyleng); }

 /* catch-all */
. { flex_error(yytext, yyleng); }

%%

