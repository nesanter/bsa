#ifndef BL_H
#define BL_H

void yyerror(char *s, ...);
enum ExprTokenType {
    ETOKT_OPER,
    ETOKT_IDENT,
    ETOKT_NUMERIC,
    ETOKT_FUNC
};

struct ExprToken {
    ExprTokenType type;
    char * text;
};

#endif /* BL_H */
