#include "enums_and_libs.h"
#include "everything_about_stack.h"
#include "stack_operations.h"


struct sput_t 
{
    int* code;
    size_t pc;
    stack_t* stack;
    stack_t* ret;
    int* regs;
    size_t size;
};

results func_push_spu(int cmd_namber, sput_t* spu);

results spu_jwc_func(int jump, sput_t* spu);

results spu_operation_func(int cmd, sput_t* spu);

results spu_out_func(int cmd, sput_t* spu);

results spu_sqvrt_func(int cmd, sput_t* spu);

results spu_jmp_func(int cmd, sput_t* spu);

results spu_call_func(int cmd, sput_t* spu);

results spu_ret_func(int cmd, sput_t* spu);

#define JWC(spu, condition)\
    jump_value = 0;\
    pop_value1 = 0, pop_value2 = 0;\
    jump_value = spu->code[++spu->pc];\
    \
    stack_pop(spu->stack, &pop_value2);\
    stack_pop(spu->stack, &pop_value1);\
    \
    if (condition)\
    {\
        spu->pc = (size_t)jump_value;\
    }\
    else\
    {\
        spu->pc++;\
    }\
    return SUCCESS;

#define OPERATION(spu, operate)\
    \
    pop_value1 = 0, pop_value2 = 0;\
    \
    stack_pop(spu->stack, &pop_value2);\
    stack_pop(spu->stack, &pop_value1);\
    \
    stack_push(spu->stack, pop_value1 operate pop_value2);\
    spu->pc++;\
    \
    return SUCCESS;


results func_push_spu(int cmd_namber, sput_t* spu)
{
    int push_value = 0;
    switch (cmd_namber)
    {   
        case PUSH:
            push_value = spu->code[++spu->pc];

            stack_push(spu->stack, push_value);
            spu->pc++;
            
            return SUCCESS;
            break;
    
        case PUSHREG:     
            ON_DEBUG(printf("SPU_PC is %lu\n", spu->pc);)
            push_value = spu->code[++spu->pc];

            ON_DEBUG(printf("PUSHREG namber is %d\n", push_value);)
            stack_push(spu->stack, spu->regs[push_value]);
            spu->pc++;

            return SUCCESS;
            break;
    
        case IN:
            scanf("%d", &push_value);

            stack_push(spu->stack, push_value);
            spu->pc++;

            return SUCCESS;
            break;

        default:
            return FAIL;
    }  
    
}
results spu_jwc_func(int jump, sput_t* spu)
{
    ssize_t jump_value = 0;
    stack_value pop_value1 = 0, pop_value2 = 0;
    switch (jump)
    {
        case JB:
            JWC(spu, pop_value1<pop_value2);
            break;
        case JBE:
            JWC(spu, pop_value1<=pop_value2);
            break;
        case JA:
            JWC(spu, pop_value1>pop_value2);
            break;
        case JAE:
            JWC(spu, pop_value1>=pop_value2);
            break;
        case JE:
            JWC(spu, pop_value1==pop_value2);
            break;
        case JNE:
            JWC(spu, pop_value1!=pop_value2);
            break;
        default:
            return FAIL;
    }
}

results spu_operation_func(int cmd, sput_t* spu)
{  
    stack_value pop_value1 = 0, pop_value2 = 0;
    switch(cmd)
    {
        case ADD:
            OPERATION(spu, +);
            break;
        case SUB:
            OPERATION(spu, -);
            break;
        case MUL:
            OPERATION(spu, *);
            break;
        case DIV:
            OPERATION(spu, /);
            break;
        default:
            return FAIL;
    }
    return FAIL;
}

results spu_out_func(int cmd, sput_t* spu)
{
    stack_value pop_value = 0;
    ON_DEBUG(printf("SPU_OUT_FUNC start\n");)
    int reg_namb = -1;

    switch (cmd)
    {
        case OUTP:
            stack_pop(spu->stack, &pop_value);

            printf("Out value = %d\n", pop_value);
            spu->pc++;

            return SUCCESS;
            break;

        case POPREG:
            ON_DEBUG(printf("-----------------------------------------------------\n");
            printf("spu->pc in popreg is %lu\n", spu->pc);)
            reg_namb = spu->code[++spu->pc];
            
            stack_pop(spu->stack, &(spu->regs[reg_namb]));
            spu->pc++;

            return SUCCESS;
            break;

        default:
            printf("I don't know this command\n");

            return FAIL;
            break;
    }
    return FAIL;
}

results spu_sqvrt_func(int /*cmd*/, sput_t* spu)
{
    stack_value pop_value = 0;
    stack_pop(spu->stack, &pop_value);

    stack_push(spu->stack, (stack_value)sqrt(pop_value));
    spu->pc++;

    return SUCCESS;
}

results spu_jmp_func(int /*cmd*/, sput_t* spu)
{
    spu->pc = (size_t)spu->code[spu->pc];
    return SUCCESS;
}

results spu_call_func(int /*cmd*/, sput_t* spu)
{
    stack_push(spu->ret, (stack_value)spu->pc + 2);

    size_t temp = (size_t)spu->code[++spu->pc];
    spu->pc = temp;

    return SUCCESS;
}

results spu_ret_func(int /*cmd*/, sput_t* spu)
{
    stack_value pop_value = 0;

    stack_pop(spu->ret, &pop_value);
    spu->pc = (size_t)pop_value;

    return SUCCESS;
}