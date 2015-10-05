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
">>" { return RANGLE_RANGLE; }
">>>" { return RANGLE_RANGLE_RANGLE; }
"<=" { return LANGLE_EQUAL; }
"<" { return LANGLE; }
"<<" { return LANGLE_LANGLE; }
"==" { return EQUAL_EQUAL; }
"!=" { return BANG_EQUAL; }
"&" { return AMPERSAND; }
"|" { return VERTICAL_BAR; }
"^" { return CARET; }
"~" { return TILDE; }

 /* keywords */
"not" { return NOT; }
"and" { return AND; }
"or" { return OR; }
"xor" { return XOR; }
"is" { return IS; }
"!is" { return BANG_IS; }
"true" { return TRUE; }
"false" { return FALSE; }
"function" { return FUNCTION; }
"constant" { return CONSTANT; }
"while" { return WHILE; }
"do" { return DO; }
"if" { return IF; }
"else" { return ELSE; }
 /* "block" { return BLOCK; } */
"yield" { return YIELD; }
"for" { return FOR; }
"until" { return UNTIL; }
 /* "return" { return RETURN; }*/
"fork" { return FORK; }
"sync" { return SYNC_BOTH; }
"sync_r" { return SYNC_READ; }
"sync_w" { return SYNC_WRITE; }
"scope" { BEGIN(SCOPETYPE); return SCOPE; }
"$fail" { return HIDDEN_FAIL; }
"$trace" { return HIDDEN_TRACE; }
"$canary" { return HIDDEN_CANARY; }
<SCOPETYPE>"always" { BEGIN(INITIAL); return ALWAYS; }
<SCOPETYPE>"success" { BEGIN(INITIAL); return SUCCESS; }
<SCOPETYPE>"failure" { BEGIN(INITIAL); return FAILURE; }

 /* strings */
\"(([^"])|(\\\"))*\" { yylval.text = strdup(escape_string(yytext)); return STRING; }

 /* numeric constants */
'\\n' { yylval.llu = '\n'; return NUMERIC; }
'\\r' { yylval.llu = '\r'; return NUMERIC; }
'\\t' { yylval.llu = '\t'; return NUMERIC; }
'\\0' { yylval.llu = '\0'; return NUMERIC; }
'\\e' { yylval.llu = 0x1B; return NUMERIC; }
'.' { yylval.llu = yytext[1]; return NUMERIC; }
 /*
0[0-9a-fA-F]+h { yylval.llu = strtoull(yytext, NULL, 16); return NUMERIC; }
0[01]+b { yylval.llu = strtoull(yytext, NULL, 2); return NUMERIC; }
[0-9]+(d)? { yylval.llu = strtoull(yytext, NULL, 10); return NUMERIC; }
 */
0x[0-9a-fA-F]+ { yylval.llu = strtoull(yytext+2, NULL, 16); return NUMERIC; }
0b[01]+ { yylval.llu = strtoull(yytext+2, NULL, 2); return NUMERIC; }
0o[0-7]+ { yylval.llu = strtoull(yytext+2, NULL, 8); return NUMERIC; }
0d[0-9]+ { yylval.llu = strtoull(yytext+2, NULL, 10); return NUMERIC; }
[0-9]+ { yylval.llu = strtoull(yytext, NULL, 10); return NUMERIC; }

 /* textual identifiers */
[a-zA-Z][a-zA-Z0-9_]* { yylval.text = strdup(yytext); return IDENT; }
_(_)?[a-zA-Z][a-zA-Z0-9_]* { yylval.text = strdup(yytext); return IDENT; }

 /* whitespace */
[ \t\n]+

 /* specific errors */
_+ { flex_error(yytext, yyleng); }

 /* catch-all */
. { flex_error(yytext, yyleng); }

%%

