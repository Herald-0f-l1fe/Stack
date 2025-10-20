#ifndef SPU_FUNC
#define SPU_FUNC

#include "enums_and_libs.h"
#include "everything_about_stack.h"

struct sput_t 
{
    int* code;
    size_t pc;
    stack_t* stack;
    stack_t* ret;
    int* regs;
    size_t size;
};


typedef results (*func_t)(int cmd, sput_t* spu);

struct com_check_t 
{
    const char* name;
    commands namber;
    tips arg_type;
    func_t function;
};


results func_push_spu(int cmd_namber, sput_t* spu);

results spu_jwc_func(int jump, sput_t* spu);

results spu_operation_func(int cmd, sput_t* spu);

results spu_out_func(int cmd, sput_t* spu);

results spu_sqvrt_func(int cmd, sput_t* spu);

results spu_jmp_func(int cmd, sput_t* spu);

results spu_call_func(int cmd, sput_t* spu);

results spu_ret_func(int cmd, sput_t* spu);

com_check_t SPU_commands_info[] = 
{
    {"PUSH", PUSH, TYPE_DIGIT, func_push_spu},
    {"IN", IN, TYPE_NONE, func_push_spu},
    {"ADD", ADD, TYPE_NONE, spu_operation_func},
    {"SUB", SUB, TYPE_NONE, spu_operation_func},
    {"MUL", MUL, TYPE_NONE, spu_operation_func},
    {"DIV", DIV, TYPE_NONE, spu_operation_func},
    {"SQVRT", SQVRT, TYPE_NONE, spu_sqvrt_func},
    {"OUT", OUTP, TYPE_NONE, spu_out_func},
    {"JMP", JMP, TYPE_LABEL, spu_jmp_func},
    {"JB", JB, TYPE_LABEL, spu_jwc_func},
    {"JA", JA, TYPE_LABEL, spu_jwc_func},
    {"JBE", JBE, TYPE_LABEL, spu_jwc_func},
    {"JAE", JAE, TYPE_LABEL, spu_jwc_func},
    {"JE", JE, TYPE_LABEL, spu_jwc_func},
    {"JNE", JNE, TYPE_LABEL, spu_jwc_func},
    {"CALL", CALL, TYPE_LABEL, spu_call_func},
    {"RET", RET, TYPE_NONE, spu_ret_func},
    {"PUSHREG", PUSHREG, TYPE_STR, func_push_spu},
    {"POPREG", POPREG, TYPE_STR, spu_out_func},
};

size_t SPU_number_of_com = sizeof(SPU_commands_info)/sizeof(SPU_commands_info[0]);






#endif