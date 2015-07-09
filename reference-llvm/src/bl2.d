import llvmd.core;

import std.stdio;

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

void main() {

}

