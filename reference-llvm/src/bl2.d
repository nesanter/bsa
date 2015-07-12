import llvmd.core;

import std.stdio;
import std.conv;
import core.vararg;

import expression, util;
import symbol_table;

/*
void main() {
    auto mod = Module.create_with_name("my_module");
    Type[] param_types = [ Type.int_type(32), Type.int_type(32)];
    Type ret_type = Type.function_type(Type.int_type(32), param_types);
    Value sum = mod.add_function("sum", ret_type);

    auto entry = sum.append_basic_block("entry");

    auto builder = Builder.create();
    builder.position_at_end(entry);

    auto tmp = builder.add(sum.get_param(0), sum.get_param(1), "tmp");
    builder.ret(tmp);

    if (mod.verify()) {
        stderr.writeln("oops!");
        return;
    }

    mod.dump();

    mod.write_bitcode_to_file("output.bc");
}
*/

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

    void yyerror(char *s, ...) {
        writeln("Error (line ",yylineno,"): ",text(s));
    }
}

int main(string[] args) {

    init();

    if (yyparse()) {
        return 1;
    }

    current_module.dump();
    if (current_module.verify()) {
        return 1;
    }

    if (unimplemented_functions()) {
        return 1;
    }

    if (args.length > 1) {
        current_module.write_bitcode_to_file(args[1]);
    }

    return 0;
}
