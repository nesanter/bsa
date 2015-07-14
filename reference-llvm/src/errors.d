import std.stdio;
import std.c.stdlib;
import core.vararg;

import bl2, expression;

void error_action() {
    if (dump_on_error) {
        current_module.dump();
    }
    exit(1);
}

void conditionally_defined_action() {
    if (conditional_definitions_are_errors) {
        error_action();
    }
}

void write_error_prefix() {
    if (color_errors)
        stderr.write("Error: \033[1;31m");
    else
        stderr.write("Error: ");
}

void write_warn_prefix() {
    if (color_errors)
        stderr.write("Warning: \033[1;33m");

    else
        stderr.write("Warning: ");
}

void write_error_suffix() {
    if (color_errors)
        stderr.writeln("\033[0m (\033[1;34mline ",yylineno, "\033[0m)");
    else
        stderr.writeln(" (line ",yylineno,")");
}

void error_yyerror(string msg) {
    write_error_prefix();
    stderr.write(msg);
    write_error_suffix();
}

void error_internal() {
    if (color_errors)
        stderr.writeln("[\033[1;31minternal error\033[0m]");
    else
        stderr.writeln("[internal error]");
}

void error_unimplemented_function(string name) {
    write_error_prefix();
    stderr.write("unimplemented function ",name);
}

void error_unknown_identifier(string ident) {
    write_error_prefix();
    stderr.write(ident, " nonexistant");
    write_error_suffix();
    error_action();
}

void error_unknown_intrinsic(string ident) {
    write_error_prefix();
    stderr.write(ident, "no such system intrinsic ",ident);
    write_error_suffix();
    error_action();
}

void error_no_manifest() {
    write_error_prefix();
    stderr.write("no manifest loaded");
    write_error_suffix();
    error_action();
}

void error_not_in_manifest(string name) {
    write_error_prefix();
    stderr.write("no such target ",name);
    write_error_suffix();
    error_action();
}

void error_intrinsic_parameter_count(string intrinsic) {
    write_error_prefix();
    stderr.write("incorrect number of parameters for system intrinsic ",intrinsic);
    write_error_suffix();
    error_action();
}

void error_intrinsic_unhandled_string(string intrinsic) {
    write_error_prefix();
    stderr.write("system intrinsic ",intrinsic, " does not accept strings");
    write_error_suffix();
    error_action();
}

void error_logical_op_requires_booleans(string op) {
    write_error_prefix();
    stderr.write("'", op, "' must be used with boolean values");
    write_error_suffix();
    error_action();
}

void error_arithmetic_op_requires_numerics(string op) {
    write_error_prefix();
    stderr.write("'", op, "' must be used with numeric values");
    write_error_suffix();
    error_action();
}

void error_inconsistent_function(string ident) {
    write_error_prefix();
    stderr.write("inconsistent number of parameters for function ",ident);
    write_error_suffix();
    error_action();
}

void error_indeterminate_statement_type() {
    write_error_prefix();
    stderr.write("statement may result in either boolean or numeric");
    write_error_suffix();
    error_action();
}

void error_fork_function_with_arguments(string ident) {
    write_error_prefix();
    stderr.write("cannot fork function with arguments ", ident);
    write_error_suffix();
    error_action();
}

void error_function_mismatch_with_prior_use(string ident) {
    write_error_prefix();
    stderr.write("declaration of function ",ident," does not match prior usage");
    write_error_suffix();
    error_action();
}

void error_shadowing(string ident) {
    write_error_prefix();
    stderr.write(ident, " shadows");
    write_error_suffix();
    error_action();
}

void error_unknown_escape_sequence(char c) {
    write_error_prefix();
    stderr.write("unknown escape sequence \\",c);
    write_error_suffix();
    error_action();
}

void warn_boolean_parameter() {
    write_warn_prefix();
    stderr.write("boolean parameter will be passed as numeric");
    write_error_suffix();
}

void warn_boolean_return() {
    write_warn_prefix();
    stderr.write("boolean function will evaluate to a numeric value");
    write_error_suffix();
}

void warn_conditionally_defined(string ident) {
    write_warn_prefix();
    stderr.write(ident, " is conditionally defined");
    write_error_suffix();
    conditionally_defined_action();
}

void warn_unexpected_error() {
    write_warn_prefix();
    stderr.write("the following error is unexpected");
    write_error_suffix();
}


