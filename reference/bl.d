import std.stdio;
import core.vararg;
import std.conv;
import core.exception;

extern (C) {
    extern __gshared int yylineno;

    alias void* YY_BUFFER_STATE;
    alias size_t yy_size_t;
    YY_BUFFER_STATE yy_create_buffer(FILE* file, int size);
    void yy_switch_to_buffer(YY_BUFFER_STATE new_buffer);
    void yy_delete_buffer(YY_BUFFER_STATE buffer);
    void yypush_buffer_state(YY_BUFFER_STATE buffer);
    void yypop_buffer_state();
    void yy_flush_buffer(YY_BUFFER_STATE buffer);
    YY_BUFFER_STATE yy_scan_string(const char* str);
    YY_BUFFER_STATE yy_scan_bytes(const char* bytes, int len);
    YY_BUFFER_STATE yy_scan_buffer(char *base, yy_size_t size);

    int yyparse();
    extern __gshared FILE* yyin;

    enum ExprTokenType {
        OPER,
        IDENT,
        NUMERIC,
        FUNC
    }

    struct ExprToken {
        ExprTokenType type;
        char * text;
    }
}

extern (C) {
    void yyerror(char *s, ...) {
        writeln("Error (line ",yylineno,"): ",text(s));
    }

    ExprToken create_expr_token(ExprTokenType type, char * text) {
        return ExprToken(type, text);
    }

}

mixin template ReferenceHandler() {
    private static typeof(this)[] references;

    private bool has_refid = false;
    private ulong refid;

    ulong reference(this T)() {
        if (!has_refid) {
            references ~= this;
            has_refid = true;
            refid = references.length - 1;
        }
        return refid;
    }

    static typeof(this) lookup(ulong id) {
        try {
            return references[id];
        } catch (RangeError e) {
            return null;
        }
    }
}

enum SymbolType {
    VARIABLE,
    FUNCTION
}

class Symbol {
    mixin ReferenceHandler;

    string identifier;
    SymbolType type;
}

enum ExpressionNodeType {
    OPERATOR,
    IDENTIFIER,
    NUMERIC_LITERAL,
    STRING_LITERAL
}

class Expression {
    mixin ReferenceHandler;

    ExpressionNodeType type;
    string text;
    long value;
}

void main() {
//    File f = File("example-obj.dobj", "r");
//    yyin = f.getFP();
    writeln(yyparse());
}

