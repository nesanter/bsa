import llvmd.core;

import std.getopt;
import std.stdio;
import std.conv;
import core.vararg;

import expression, util, errors, channels;
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

    void yyerror(const char *s, ...) {
        error_yyerror(text(s));
//        stderr.writeln("Error (\033[1;34mline ",yylineno,"\033[0m): \033[1;31m",text(s), "\033[0m");
    }

    void flex_error(char *c, int length) {
        error_lexical(to!string(c[0..length]));
    }
}

bool dump_on_error = false;
bool color_errors = true;
bool conditional_definitions_are_errors = false;
bool enable_all_warnings = false;

int main(string[] args) {

    string output_file;
    string manifest_file;
    bool print_ir;
    bool no_entry;
    bool delay_implement;
    bool help;

    try {
        getopt(args,
               "o", &output_file,
               "m", &manifest_file,
               "d", &print_ir,
               "show-error-ir", &dump_on_error,
               "color", &color_errors,
               "strict",  &conditional_definitions_are_errors,
               "lib", &no_entry,
               "delayed-implementation", &delay_implement,
               "trace-support", &include_trace_names,
               "warn", &enable_all_warnings,
               "channel-unsafe", &channel_unsafe,
               "channel-fifo-size", &channel_fifo_size,
               "channel-listener-max", &channel_max_listeners,
               "help", &help
        );
    } catch (Exception e) {
        stderr.writeln(e.msg);
        return 1;
    }

    if (help) {
        writeln("muc -- the \u03BC compiler -- \u00A9 Noah Santer 2015");
        writeln("options:");
        writeln("    -o\t\t\t\toutput file [print to stdout]");
        writeln("    -m\t\t\t\tmanifest file [no manifest]");
        writeln("    -d\t\t\t\tprint LLVM ir [false]");
        writeln("    --show-error-ir\t\tprint LLVM ir on error [false]");
        writeln("    --color\t\t\tenable colors [true]");
        writeln("    --strict\t\t\tstrict handling of warnings [false]");
        writeln("    --lib\t\t\tcompile without @entry function [false]");
        writeln("    --delayed-implementation\tunimplemented functions are not errors [false]");
        writeln("    --trace-support\t\tsupport runtime $trace [false]");
        writeln("    --warn\t\t\tenable all warnings [false]");
        writeln("Extension options:");
        writeln("    --channel-unsafe\t\tenable unsafe channel operations [false]");
        writeln("    --channel-fifo-size\t\tchange channel FIFO size [", channel_fifo_size, "]");
        writeln("    --channel-listener-max\t\tchange listener channel maximum [", channel_max_listeners, "]");
        return 2;
    }

    init(manifest_file);

    if (yyparse()) {
        return 1;
    }

    if (print_ir)
        current_module.dump();

    if (current_module.verify()) {
        if (!print_ir && dump_on_error)
            current_module.dump();
        error_internal();
//        stderr.writeln("[internal compiler error]");
        return 1;
    }

    if (!delay_implement && unimplemented_functions()) {
        return 1;
    }

    if (entry_function is null && !no_entry) {
        error_no_entry();
    } else if (entry_function !is null && no_entry) {
        error_lib_with_entry();
    }

    if (output_file.length > 0) {
        current_module.write_bitcode_to_file(output_file);
    }

    return 0;
}
