import std.stdio;
import std.c.stdlib;
import core.vararg;

import bl2, expression;

void generic_error() {
    if (dump_on_error) {
        current_module.dump();
    }
    exit(1);
}
