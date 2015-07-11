module llvmd.core;

import std.conv;
import std.string;
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

extern (C) {
    protected:
        struct LLVMOpaqueModule;
        alias LLVMOpaqueModule* LLVMModuleRef;
        LLVMModuleRef LLVMModuleCreateWithName(const char *ModuleID);
        void LLVMDumpModule(LLVMModuleRef M);
        const(char *) LLVMPrintModuleToString(LLVMModuleRef M);
        void LLVMSetTarget(LLVMModuleRef M, const char *Triple);
        const(char *) LLVMGetTarget(LLVMModuleRef M);
        LLVMValueRef LLVMAddFunction(LLVMModuleRef M, const char *Name, LLVMTypeRef FunctionTy);

        int LLVMWriteBitcodeToFile(LLVMModuleRef M, const char *Path);

        enum LLVMVerifierFailureAction {
            LLVMAbortProcessAction,
            LLVMPrintMessageAction,
            LLVMReturnStatusAction
        }

        int LLVMVerifyModule(LLVMModuleRef M, LLVMVerifierFailureAction Action, char **OutMessage);
        void LLVMDisposeModule(LLVMModuleRef M);
}

class Module {
    protected LLVMModuleRef mod;

    protected this(LLVMModuleRef mod) {
        this.mod = mod;
    }

    string error;

    ~this() {
        LLVMDisposeModule(mod);
    }

    static Module create_with_name(string name) {
        return new Module(LLVMModuleCreateWithName(toStringz(name)));
    }

    void set_target(string triple) {
        LLVMSetTarget(mod, toStringz(triple));
    }

    string get_target() {
        return text(LLVMGetTarget(mod));
    }

    Value add_function(string name, Type fn_type) {
        return new Value(LLVMAddFunction(mod, toStringz(name), fn_type.type));
    }

    bool write_bitcode_to_file(string fname) {
        return LLVMWriteBitcodeToFile(mod, toStringz(fname)) != 0;
    }

    bool verify() {
        char *err;
        int n = LLVMVerifyModule(mod, LLVMVerifierFailureAction.LLVMReturnStatusAction, &err);
        if (err) {
            error = text(err);
            stderr.writeln(error);
        }
        if (n)
            return true;
        else
            return false;
    }

    // output
    void dump() {
        LLVMDumpModule(mod);
    }

    override string toString() {
        return text(LLVMPrintModuleToString(mod));
    }
}

extern (C) {
    protected:
        struct LLVMOpaqueType;
        alias LLVMOpaqueType* LLVMTypeRef;

        LLVMTypeRef LLVMIntType(uint NumBits);
        LLVMTypeRef LLVMFunctionType(LLVMTypeRef ReturnType, LLVMTypeRef *ParamTypes, uint ParamCount, int IsVarArg);
        LLVMTypeRef LLVMStructType(LLVMTypeRef *ElementTypes, uint ElementCount, int Packed);

        void LLVMDumpType(LLVMTypeRef Ty);
}

class Type {
    protected LLVMTypeRef type;
    protected this(LLVMTypeRef type) {
        this.type = type;
    }

    static Type int_type(uint numbits) {
        return new Type(LLVMIntType(numbits));
    }

    static Type function_type(Type return_type, Type[] param_types) {
        LLVMTypeRef[] params = new LLVMTypeRef[](param_types.length);
        foreach (i,pt; param_types)
            params[i] = pt.type;

        return new Type(LLVMFunctionType(return_type.type, params.ptr, cast(uint)params.length, 0));
    }

    static Type struct_type(Type[] element_types, bool packed) {
        LLVMTypeRef[] els = new LLVMTypeRef[](element_types.length);
        foreach (i, et; element_types)
            els[i] = et.type;

        return new Type(LLVMStructType(els.ptr, cast(uint)els.length, packed ? 1 : 0));
    }

    void dump() {
        LLVMDumpType(type);
    }
}

extern (C) {
    protected:
       struct LLVMOpaqueValue;
       alias LLVMOpaqueValue* LLVMValueRef;

       LLVMValueRef LLVMGetParam(LLVMValueRef Fn, uint index);

       LLVMValueRef LLVMConstInt(LLVMTypeRef IntTy, ulong N, int SignExtend);

       void LLVMDumpValue(LLVMValueRef Val);
}

class Value {
    protected LLVMValueRef val;
    protected this(LLVMValueRef val) {
        this.val = val;
    }

    static Value create_const_int(Type type, ulong n) {
        return new Value(LLVMConstInt(type.type, n, 0));
    }

    BasicBlock append_basic_block(string name) {
        return new BasicBlock(LLVMAppendBasicBlock(val, toStringz(name)));
    }

    Value get_param(uint index) {
        return new Value(LLVMGetParam(val, index));
    }

    void dump() {
        LLVMDumpValue(val);
    }
}

extern (C) {
    protected:
        struct LLVMOpaqueBasicBlock;
        alias LLVMOpaqueBasicBlock* LLVMBasicBlockRef;

        LLVMBasicBlockRef LLVMAppendBasicBlock(LLVMValueRef Fn, const char *Name);
}

class BasicBlock {
    protected LLVMBasicBlockRef basic_block;

    protected this(LLVMBasicBlockRef basic_block) {
        this.basic_block = basic_block;
    }
}


extern (C) {
    protected:
        struct LLVMOpaqueBuilder;
        alias LLVMOpaqueBuilder* LLVMBuilderRef;

        LLVMBuilderRef LLVMCreateBuilder();
        void LLVMPositionBuilderAtEnd(LLVMBuilderRef Builder, LLVMBasicBlockRef Block);
        void LLVMDisposeBuilder(LLVMBuilderRef Builder);

        LLVMValueRef LLVMBuildAdd(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS, const char *Name);
        LLVMValueRef LLVMBuildSub(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS, const char *Name);
        LLVMValueRef LLVMBuildMul(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS, const char *Name);
        LLVMValueRef LLVMBuildSDiv(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS, const char *Name);
        LLVMValueRef LLVMBuildSRem(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS, const char *Name);
        LLVMValueRef LLVMBuildICmp(LLVMBuilderRef, LLVMIntPredicate Pred, LLVMValueRef LHS, LLVMValueRef RHS, const char *Name);
        LLVMValueRef LLVMBuildNeg(LLVMBuilderRef, LLVMValueRef LHS, const char *Name);

        LLVMValueRef LLVMBuildSelect(LLVMBuilderRef, LLVMValueRef If, LLVMValueRef Then, LLVMValueRef Else, const char *Name);

        LLVMValueRef LLVMBuildRet(LLVMBuilderRef, LLVMValueRef V);
        LLVMValueRef LLVMBuildRetVoid(LLVMBuilderRef);

        LLVMValueRef LLVMBuildBr(LLVMBuilderRef, LLVMBasicBlockRef Dest);
        LLVMValueRef LLVMBuildCondBr(LLVMBuilderRef, LLVMValueRef If, LLVMBasicBlockRef Then, LLVMBasicBlockRef Else);

        LLVMValueRef LLVMBuildPhi(LLVMBuilderRef, LLVMTypeRef T, const char *Name);

        void LLVMAddIncoming(LLVMValueRef PhiNode, LLVMValueRef *IncomingValues, LLVMBasicBlockRef *IncomingBlocks, uint count);

        enum LLVMIntPredicate {
            LLVMIntEQ = 32,
            LLVMIntNE,
            LLVMIntUGT,
            LLVMIntUGE,
            LLVMIntULT,
            LLVMIntULE,
            LLVMIntSGT,
            LLVMIntSGE,
            LLVMIntSLT,
            LLVMIntSLE
        }
}

class Builder {
    protected LLVMBuilderRef builder;

    protected this(LLVMBuilderRef builder) {
        this.builder = builder;
    }

    ~this() {
        LLVMDisposeBuilder(builder);
    }

    static Builder create() {
        return new Builder(LLVMCreateBuilder());
    }

    void position_at_end(BasicBlock block) {
        LLVMPositionBuilderAtEnd(builder, block.basic_block);
    }

    
    Value add(Value lhs, Value rhs, string name = null) {
        return new Value(LLVMBuildAdd(builder, lhs.val, rhs.val, toStringz(name)));
    }

    Value sub(Value lhs, Value rhs, string name = null) {
        return new Value(LLVMBuildSub(builder, lhs.val, rhs.val, toStringz(name)));
    }

    Value mul(Value lhs, Value rhs, string name = null) {
        return new Value(LLVMBuildMul(builder, lhs.val, rhs.val, toStringz(name)));
    }

    Value sdiv(Value lhs, Value rhs, string name = null) {
        return new Value(LLVMBuildSDiv(builder, lhs.val, rhs.val, toStringz(name)));
    }

    Value srem(Value lhs, Value rhs, string name = null) {
        return new Value(LLVMBuildSRem(builder, lhs.val, rhs.val, toStringz(name)));
    }

    Value neg(Value lhs, string name = null) {
        return new Value(LLVMBuildNeg(builder, lhs.val, toStringz(name)));
    }

    Value icmp_eq(Value lhs, Value rhs, string name = null) {
        return new Value(LLVMBuildICmp(builder, LLVMIntPredicate.LLVMIntEQ, lhs.val, rhs.val, toStringz(name)));
    }

    Value icmp_ne(Value lhs, Value rhs, string name = null) {
        return new Value(LLVMBuildICmp(builder, LLVMIntPredicate.LLVMIntNE, lhs.val, rhs.val, toStringz(name)));
    }

    Value icmp_slt(Value lhs, Value rhs, string name = null) {
        return new Value(LLVMBuildICmp(builder, LLVMIntPredicate.LLVMIntSLT, lhs.val, rhs.val, toStringz(name)));
    }

    Value icmp_slte(Value lhs, Value rhs, string name = null) {
        return new Value(LLVMBuildICmp(builder, LLVMIntPredicate.LLVMIntSLE, lhs.val, rhs.val, toStringz(name)));
    }

    Value icmp_sgt(Value lhs, Value rhs, string name = null) {
        return new Value(LLVMBuildICmp(builder, LLVMIntPredicate.LLVMIntSGT, lhs.val, rhs.val, toStringz(name)));
    }

    Value icmp_sgte(Value lhs, Value rhs, string name = null) {
        return new Value(LLVMBuildICmp(builder, LLVMIntPredicate.LLVMIntSGE, lhs.val, rhs.val, toStringz(name)));
    }

    Value icmp_ult(Value lhs, Value rhs, string name = null) {
        return new Value(LLVMBuildICmp(builder, LLVMIntPredicate.LLVMIntULT, lhs.val, rhs.val, toStringz(name)));
    }

    Value icmp_ulte(Value lhs, Value rhs, string name = null) {
        return new Value(LLVMBuildICmp(builder, LLVMIntPredicate.LLVMIntULE, lhs.val, rhs.val, toStringz(name)));
    }

    Value icmp_ugt(Value lhs, Value rhs, string name = null) {
        return new Value(LLVMBuildICmp(builder, LLVMIntPredicate.LLVMIntUGT, lhs.val, rhs.val, toStringz(name)));
    }

    Value icmp_ugte(Value lhs, Value rhs, string name = null) {
        return new Value(LLVMBuildICmp(builder, LLVMIntPredicate.LLVMIntUGE, lhs.val, rhs.val, toStringz(name)));
    }

    Value select(Value test, Value if_true, Value if_false, string name = null) {
        return new Value(LLVMBuildSelect(builder, test.val, if_true.val, if_false.val, toStringz(name)));
    }

    Value ret(Value v) {
        return new Value(LLVMBuildRet(builder, v.val));
    }

    Value ret_void() {
        return new Value(LLVMBuildRetVoid(builder));
    }

    Value br(BasicBlock dest) {
        return new Value(LLVMBuildBr(builder, dest.basic_block));
    }

    Value cond_br(Value cond, BasicBlock if_true, BasicBlock if_false) {
        return new Value(LLVMBuildCondBr(builder, cond.val, if_true.basic_block, if_false.basic_block));
    }

    Value phi(Type type, string name = null) {
        return new Value(LLVMBuildPhi(builder, type.type, toStringz(name)));
    }

    Value make_phi(Type type, Value[] vals, BasicBlock[] blocks) {
        assert(vals.length == blocks.length);
        LLVMValueRef[] rawvals = new LLVMValueRef[](vals.length);
        LLVMBasicBlockRef[] rawblocks = new LLVMBasicBlockRef[](blocks.length);

        foreach (i; 0 .. vals.length) {
            rawvals[i] = vals[i].val;
            rawblocks[i] = blocks[i].basic_block;
        }

        auto res = phi(type);
        LLVMAddIncoming(res.val, rawvals.ptr, rawblocks.ptr, cast(uint)rawvals.length);
        return res;
    }
}


