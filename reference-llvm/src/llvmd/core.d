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

        struct LLVMOpaqueContext;
        alias LLVMOpaqueContext* LLVMContextRef;

        LLVMContextRef LLVMGetGlobalContext();
}

class Module {
    protected LLVMModuleRef mod;

    protected this(LLVMModuleRef mod) {
        this.mod = mod;
    }

    string error;

    /*
    ~this() {
        LLVMDisposeModule(mod);
    }
    */

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
        if (n && err) {
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
        LLVMTypeRef LLVMStructCreateNamed(LLVMContextRef C, const char * Name);
        LLVMTypeRef LLVMPointerType(LLVMTypeRef ElementType, uint AddressSpace);
        LLVMTypeRef LLVMVoidType();

        void LLVMStructSetBody(LLVMTypeRef StructTy, LLVMTypeRef *ElementTypes, uint ElementCount, int Packed);

        LLVMTypeRef LLVMGetReturnType(LLVMTypeRef FunctionTy);

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

    static Type named_struct_type(string name) {
        return new Type(LLVMStructCreateNamed(LLVMGetGlobalContext(), toStringz(name)));
    }

    static Type pointer_type(Type base) {
        return new Type(LLVMPointerType(base.type, 0));
    }

    static Type void_type() {
        return new Type(LLVMVoidType());
    }

    void set_body(Type[] element_types, bool packed) {
        LLVMTypeRef[] els = new LLVMTypeRef[](element_types.length);
        foreach (i, et; element_types)
            els[i] = et.type;

        LLVMStructSetBody(type, els.ptr, cast(uint)els.length, packed ? 1 : 0);
    }

    Type get_return_type() {
        return new Type(LLVMGetReturnType(type));
    }

    void dump() {
        LLVMDumpType(type);
    }

    bool same(Type other) {
        return type == other.type;
    }
}

extern (C) {
    protected:
       struct LLVMOpaqueValue;
       alias LLVMOpaqueValue* LLVMValueRef;

       LLVMValueRef LLVMGetParam(LLVMValueRef Fn, uint index);

       LLVMValueRef LLVMConstInt(LLVMTypeRef IntTy, ulong N, int SignExtend);
       LLVMValueRef LLVMConstString(const char *Str, uint Length, uint DontNullTerminate);

       LLVMValueRef LLVMAddGlobal(LLVMModuleRef M, LLVMTypeRef Ty, const char * Name);
       LLVMValueRef LLVMConstNull(LLVMTypeRef Ty);

       void LLVMReplaceAllUsesWith(LLVMValueRef OldVal, LLVMValueRef NewVal);

       LLVMTypeRef LLVMTypeOf(LLVMValueRef Val);

       const(char *) LLVMGetAsString(LLVMValueRef c, size_t *Out);
       int LLVMIsConstantString(LLVMValueRef c);

       int LLVMIsConstant(LLVMValueRef Val);

       LLVMValueRef LLVMConstGEP(LLVMValueRef ConstantVal, LLVMValueRef *ConstantIndices, uint NumIndices);

       LLVMValueRef LLVMGetNextInstruction(LLVMValueRef Inst);

       LLVMValueRef LLVMGetOperand(LLVMValueRef Val, uint Index);
       uint LLVMGetNumOperands(LLVMValueRef Val);
       void LLVMSetOperand(LLVMValueRef User, uint Index, LLVMValueRef Val);

       void LLVMDumpValue(LLVMValueRef Val);

       void LLVMSetInitializer(LLVMValueRef GlobalVar, LLVMValueRef ConstantVal);

       LLVMValueRef LLVMAddAlias(LLVMModuleRef M, LLVMTypeRef Ty, LLVMValueRef Aliasee, const char * Name);

       void LLVMSetLinkage(LLVMValueRef Global, LLVMLinkage Linkage);

       enum LLVMLinkage {
           LLVMExternalLinkage,
           LLVMAvailableExternallyLinkage,
           LLVMLinkOnceAnyLinkage,
           LLVMLinkOnceODRLinkage,
           LLVMLinkOnceODRAutoHideLinkage,
           LLVMWeakLinkAnyLinkage,
           LLVMWeakODRLinkage,
           LLVMAppendingLinkage,
           LLVMInternalLinkage,
           LLVMPrivateLinkage,
           LLVMDLLImportLinkage,
           LLVMDLLExportLinkage,
           LLVMExternalWeakLinkage,
           LLVMGhostLinkage,
           LLVMCommonLinkage,
           LLVMLinkerPrivateLinkage,
           LLVMLinkerPrivateWeakLinkage
       }

       LLVMOpcode LLVMGetInstructionOpcode(LLVMValueRef Inst);

       enum LLVMOpcode {
           /* Terminator Instructions */
           LLVMRet            = 1,
           LLVMBr             = 2,
           LLVMSwitch         = 3,
           LLVMIndirectBr     = 4,
           LLVMInvoke         = 5,
           /* removed 6 due to API changes */
           LLVMUnreachable    = 7,

           /* Standard Binary Operators */
           LLVMAdd            = 8,
           LLVMFAdd           = 9,
           LLVMSub            = 10,
           LLVMFSub           = 11,
           LLVMMul            = 12,
           LLVMFMul           = 13,
           LLVMUDiv           = 14,
           LLVMSDiv           = 15,
           LLVMFDiv           = 16,
           LLVMURem           = 17,
           LLVMSRem           = 18,
           LLVMFRem           = 19,

           /* Logical Operators */
           LLVMShl            = 20,
           LLVMLShr           = 21,
           LLVMAShr           = 22,
           LLVMAnd            = 23,
           LLVMOr             = 24,
           LLVMXor            = 25,

           /* Memory Operators */
           LLVMAlloca         = 26,
           LLVMLoad           = 27,
           LLVMStore          = 28,
           LLVMGetElementPtr  = 29,

           /* Cast Operators */
           LLVMTrunc          = 30,
           LLVMZExt           = 31,
           LLVMSExt           = 32,
           LLVMFPToUI         = 33,
           LLVMFPToSI         = 34,
           LLVMUIToFP         = 35,
           LLVMSIToFP         = 36,
           LLVMFPTrunc        = 37,
           LLVMFPExt          = 38,
           LLVMPtrToInt       = 39,
           LLVMIntToPtr       = 40,
           LLVMBitCast        = 41,
           LLVMAddrSpaceCast  = 60,

           /* Other Operators */
           LLVMICmp           = 42,
           LLVMFCmp           = 43,
           LLVMPHI            = 44,
           LLVMCall           = 45,
           LLVMSelect         = 46,
           LLVMUserOp1        = 47,
           LLVMUserOp2        = 48,
           LLVMVAArg          = 49,
           LLVMExtractElement = 50,
           LLVMInsertElement  = 51,
           LLVMShuffleVector  = 52,
           LLVMExtractValue   = 53,
           LLVMInsertValue    = 54,

           /* Atomic operators */
           LLVMFence          = 55,
           LLVMAtomicCmpXchg  = 56,
           LLVMAtomicRMW      = 57,

           /* Exception Handling Operators */
           LLVMResume         = 58,
           LLVMLandingPad     = 59
       }
}

class Value {
    protected LLVMValueRef val;
    protected this(LLVMValueRef val) {
        this.val = val;
    }

    static Value create_const_int(Type type, ulong n) {
        return new Value(LLVMConstInt(type.type, n, 0));
    }

    static Value create_string(string s) {
        return new Value(LLVMConstString(toStringz(s), cast(uint)s.length, 1));
    }

    static Value create_global_variable(Module m, Type type, string s = null) {
        return new Value(LLVMAddGlobal(m.mod, type.type, toStringz(s)));
    }

    static Value create_const_null(Type t) {
        return new Value(LLVMConstNull(t.type));
    }

    static Value add_alias(Module m, Type type, Value aliasee, string name = null) {
        return new Value(LLVMAddAlias(m.mod, type.type, aliasee.val, toStringz(name)));
    }

    static void replace_all(Value old, Value replacement) {
        LLVMReplaceAllUsesWith(old.val, replacement.val);
    }

    BasicBlock append_basic_block(string name) {
        return new BasicBlock(LLVMAppendBasicBlock(val, toStringz(name)));
    }

    Value next_instruction() {
        auto iref = LLVMGetNextInstruction(val);
        if (iref is null) {
            return null;
        } else {
            return new Value(iref);
        }
    }

    Value get_param(uint index) {
        return new Value(LLVMGetParam(val, index));
    }

    Value get_operand(uint index) {
        return new Value(LLVMGetOperand(val, index));
    }

    uint get_num_operands() {
        return LLVMGetNumOperands(val);
    }

    void set_operand(uint index, Value newval) {
        LLVMSetOperand(val, index, newval.val);
    }

    @property Type type() {
        return new Type(LLVMTypeOf(val));
    }

    string as_string() {
        size_t n;
        return text(LLVMGetAsString(val, &n));
    }

    Value const_gep(Value[] indices) {
        LLVMValueRef[] rawvals = new LLVMValueRef[](indices.length);
        foreach (i, ind; indices)
            rawvals[i] = ind.val;
        return new Value(LLVMConstGEP(val, rawvals.ptr, cast(uint)rawvals.length));
    }

    void set_initializer(Value v) {
        LLVMSetInitializer(val, v.val);
    }

    void add_incoming(Value[] vals, BasicBlock[] blocks) {
        assert(vals.length == blocks.length);
        LLVMValueRef[] rawvals = new LLVMValueRef[](vals.length);
        LLVMBasicBlockRef[] rawblocks = new LLVMBasicBlockRef[](blocks.length);

        foreach (i; 0 .. vals.length) {
            rawvals[i] = vals[i].val;
            rawblocks[i] = blocks[i].basic_block;
        }

        LLVMAddIncoming(val, rawvals.ptr, rawblocks.ptr, cast(uint)rawvals.length);
    }

    void set_internal_linkage() {
        LLVMSetLinkage(val, LLVMLinkage.LLVMInternalLinkage);
    }

    bool is_constant_string() {
        return LLVMIsConstantString(val) != 0;
    }

    bool is_const() {
        return LLVMIsConstant(val) != 0;
    }

    bool same(Value other) {
        return val == other.val;
    }

    void dump() {
        LLVMDumpValue(val);
    }

    bool is_phi() {
        return LLVMGetInstructionOpcode(val) == LLVMOpcode.LLVMPHI;
    }
}

extern (C) {
    protected:
        struct LLVMOpaqueBasicBlock;
        alias LLVMOpaqueBasicBlock* LLVMBasicBlockRef;

        LLVMBasicBlockRef LLVMAppendBasicBlock(LLVMValueRef Fn, const char *Name);

        LLVMValueRef LLVMGetFirstInstruction(LLVMBasicBlockRef BB);
}

class BasicBlock {
    protected static BasicBlock[] all_bbs;
    static ulong bb_count;
    ulong index;

    protected LLVMBasicBlockRef basic_block;

    protected this(LLVMBasicBlockRef basic_block) {
        BasicBlock.all_bbs ~= this;
        this.basic_block = basic_block;
        this.index = bb_count++;
    }

    Value first_instruction() {
        auto inst = LLVMGetFirstInstruction(basic_block);
        return new Value(inst);
    }

    static BasicBlock[] all() {
        return all_bbs;
    }
}


extern (C) {
    protected:
        struct LLVMOpaqueBuilder;
        alias LLVMOpaqueBuilder* LLVMBuilderRef;

        LLVMBuilderRef LLVMCreateBuilder();
        void LLVMPositionBuilderAtEnd(LLVMBuilderRef Builder, LLVMBasicBlockRef Block);
        void LLVMPositionBuilderBefore(LLVMBuilderRef Builder, LLVMValueRef Inst);
        void LLVMDisposeBuilder(LLVMBuilderRef Builder);

        LLVMValueRef LLVMBuildAnd(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS, const char *Name);
        LLVMValueRef LLVMBuildOr(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS, const char *Name);
        LLVMValueRef LLVMBuildAdd(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS, const char *Name);
        LLVMValueRef LLVMBuildSub(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS, const char *Name);
        LLVMValueRef LLVMBuildMul(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS, const char *Name);
        LLVMValueRef LLVMBuildSDiv(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS, const char *Name);
        LLVMValueRef LLVMBuildSRem(LLVMBuilderRef, LLVMValueRef LHS, LLVMValueRef RHS, const char *Name);
        LLVMValueRef LLVMBuildICmp(LLVMBuilderRef, LLVMIntPredicate Pred, LLVMValueRef LHS, LLVMValueRef RHS, const char *Name);
        LLVMValueRef LLVMBuildNeg(LLVMBuilderRef, LLVMValueRef LHS, const char *Name);

        LLVMValueRef LLVMBuildSelect(LLVMBuilderRef, LLVMValueRef If, LLVMValueRef Then, LLVMValueRef Else, const char *Name);

        LLVMValueRef LLVMBuildGlobalStringPtr(LLVMBuilderRef B, const char *Str, const char *Name);

        LLVMValueRef LLVMBuildRet(LLVMBuilderRef, LLVMValueRef V);
        LLVMValueRef LLVMBuildRetVoid(LLVMBuilderRef);

        LLVMValueRef LLVMBuildBr(LLVMBuilderRef, LLVMBasicBlockRef Dest);
        LLVMValueRef LLVMBuildCondBr(LLVMBuilderRef, LLVMValueRef If, LLVMBasicBlockRef Then, LLVMBasicBlockRef Else);
        LLVMValueRef LLVMBuildCall(LLVMBuilderRef, LLVMValueRef Fn, LLVMValueRef *Args, uint NumArgs, const char *Name);

        LLVMValueRef LLVMBuildLoad(LLVMBuilderRef, LLVMValueRef PointerVal, const char * Name);
        void LLVMBuildStore(LLVMBuilderRef, LLVMValueRef Val, LLVMValueRef Ptr);

        LLVMValueRef LLVMBuildStructGEP(LLVMBuilderRef, LLVMValueRef Pointer, uint Idx, const char * Name);
        LLVMValueRef LLVMBuildAlloca(LLVMBuilderRef, LLVMTypeRef Ty, const char * Name);

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

    /*
    ~this() {
        LLVMDisposeBuilder(builder);
    }
    */

    static Builder create() {
        return new Builder(LLVMCreateBuilder());
    }

    void position_at_end(BasicBlock block) {
        LLVMPositionBuilderAtEnd(builder, block.basic_block);
    }

    void position_before(Value inst) {
        LLVMPositionBuilderBefore(builder, inst.val);
    }

    Value and(Value lhs, Value rhs, string name = null) {
        return new Value(LLVMBuildAnd(builder, lhs.val, rhs.val, toStringz(name)));
    }

    Value or(Value lhs, Value rhs, string name = null) {
        return new Value(LLVMBuildOr(builder, lhs.val, rhs.val, toStringz(name)));
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

    Value global_string_ptr(string s, string name = null) {
        return new Value(LLVMBuildGlobalStringPtr(builder, toStringz(s), toStringz(name)));
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

    Value call(Value fn, Value[] params, string name = null) {
        LLVMValueRef[] raw_vals = new LLVMValueRef[](params.length);
        foreach (i, p; params) {
            raw_vals[i] = p.val;
        }
        return new Value(LLVMBuildCall(builder, fn.val, raw_vals.ptr, cast(uint)raw_vals.length, toStringz(name)));
    }

    Value load(Value ptr, string name = null) {
        return new Value(LLVMBuildLoad(builder, ptr.val, toStringz(name)));
    }

    void store(Value val, Value ptr) {
        LLVMBuildStore(builder, val.val, ptr.val);
    }

    Value alloca(Type type, string name = null) {
        return new Value(LLVMBuildAlloca(builder, type.type, toStringz(name)));
    }

    Value struct_gep(Value ptr, uint index, string name = null) {
        return new Value(LLVMBuildStructGEP(builder, ptr.val, index, toStringz(name)));
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


