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

void error_lexical(string bad) {
    write_error_prefix();
    stderr.write("lexical error, unexpected symbol '", bad, "'");
    write_error_suffix();
    error_action();
}

void error_internal() {
    if (color_errors)
        stderr.writeln("[\033[1;31minternal error\033[0m]");
    else
        stderr.writeln("[internal error]");
}

void error_unimplemented_function(string name) {
    write_error_prefix();
    stderr.writeln("unimplemented function ",name);
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

void error_intrinsic_not_allowed_for_target(string intrinsic, string target) {
    write_error_prefix();
    stderr.write(target, " is not a valid target for system intrinsic ",intrinsic);
    write_error_suffix();
    error_action();
}

void error_intrinsic_args_not_allowed_for_target(string intrinsic, string target) {
    write_error_prefix();
    stderr.write(target, " is not a valid target for this invocation of system intrinsic ",intrinsic);
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

void error_is_op_requires_rh_boolean(string op) {
    write_error_prefix();
    stderr.write("right hand value of '", op, "' must be a boolean value");
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
    stderr.write("inconsistent number of arguments for function ",ident);
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

void error_function_shadows_different_type(string ident) {
    write_error_prefix();
    stderr.write("identifier ", ident, " was previously declared with a different type");
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

void error_incomplete_escape() {
    write_error_prefix();
    stderr.write("incomplete escaped hex literal");
    write_error_suffix();
    error_action();
}

void error_not_hex(char c) {
    write_error_prefix();
    stderr.write(c, " not in [0-9a-fA-F]");
    write_error_suffix();
    error_action();
}

void error_no_such_attribute(string attr) {
    write_error_prefix();
    stderr.write("unknown function attribute @",attr);
    write_error_suffix();
    error_action();
}

void error_handler_attr_unexpected() {
    write_error_prefix();
    stderr.write("unexpected function attribute @handler based on previous usage");
    write_error_suffix();
    error_action();
}

void error_handler_attr_expected() {
    write_error_prefix();
    stderr.write("expected function attribute @handler based on previous usage");
    write_error_suffix();
    error_action();
}

void error_multiple_entry() {
    write_error_prefix();
    stderr.write("multiple functions declared with attribute @entry");
    write_error_suffix();
    error_action();
}

void error_no_entry() {
    write_error_prefix();
    stderr.write("no function declared with attribute @entry");
    write_error_suffix();
    error_action();
}

void error_lib_with_entry() {
    write_error_prefix();
    stderr.write("entry function present but '-lib' specified");
    write_error_suffix();
    error_action();
}

void error_entry_with_arguments() {
    write_error_prefix();
    stderr.write("entry function declared with arguments");
    write_error_suffix();
    error_action();
}

void error_scope_forbidden() {
    write_error_prefix();
    stderr.write("scope statements are forbidden in this context");
    write_error_suffix();
    error_action();
}

void error_scope_not_handler() {
    write_error_prefix();
    stderr.write("scope statements may only reference @handler functions");
    write_error_suffix();
    error_action();
}

void error_symbol_of_different_type(string s) {
    write_error_prefix();
    stderr.write("incompatible symbol exists");
    write_error_suffix();
    error_action();
}

void error_previously_declared(string s) {
    write_error_prefix();
    stderr.write("multiple declarations for ", s);
    write_error_suffix();
    error_action();
}

void warn_boolean_parameter() {
    if (!enable_all_warnings)
        return;
    write_warn_prefix();
    stderr.write("boolean parameter will be passed as numeric");
    write_error_suffix();
}

void warn_boolean_return() {
    if (!enable_all_warnings)
        return;
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

void warn_globals_deprecated() {
    write_warn_prefix();
    stderr.write("globals have been deprecated");
    write_error_suffix();
}

