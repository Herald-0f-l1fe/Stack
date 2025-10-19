#include "enums_and_libs.h"
#include "everything_about_stack.h"
#include "stack_operations.h"

struct sput_t 
{
    int* code;
    size_t pc;
    stk_t* stack;
    stk_t* ret;
    int* regs;
    size_t size;
};

#define JWC(spu, condition)\
    jump_value = 0;\
    pop_value1 = 0, pop_value2 = 0;\
    jump_value = spu->code[++spu->pc];\
    stack_pop(spu->stack, &pop_value2);\
    stack_pop(spu->stack, &pop_value1);\
    if (condition)\
    {\
        spu->pc = jump_value;\
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
    return SUCCESS;


int func_push_spu(int cmd_namber, sput_t* spu)
{
    int push_value = 0;
    switch(cmd_namber)
    {   
        case PUSH:
            push_value = spu->code[++spu->pc];
            stack_push(spu->stack, push_value);
            spu->pc++;
            break;
    
        case PUSHREG:     
            //printf("SPU_PC is %ld\n", spu->pc);
            push_value = spu->code[++spu->pc];
            //printf("PUSHREG namber is %d\n", push_value);
            stack_push(spu->stack, spu->regs[push_value]);
            spu->pc++;
            break;
    
        case IN:
            scanf("%d", &push_value);
            stack_push(spu->stack, push_value);
            spu->pc++;
            break;
    }  
    return SUCCESS;
}
int spu_jwc_func(int jump, sput_t* spu)
{
    int jump_value = 0;
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
    return -1;
    }

}

int spu_operation_func(int cmd, sput_t* spu)
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
    }
    return FAIL;
}

int spu_out_func(int cmd, sput_t* spu)
{
    stack_value pop_value = 0;
    //printf("SPU_OUT_FUNC start\n");
    switch(cmd)
    {
    case OUTP:
        stack_pop(spu->stack, &pop_value);
        printf("Out value = %d\n", pop_value);
        spu->pc++;
        return SUCCESS;
        break;
    case POPREG:
        //printf("-----------------------------------------------------\n");
        //printf("spu->pc in popreg is %ld\n", spu->pc);
        int reg_namb = spu->code[++spu->pc];
        
        stack_pop(spu->stack, &(spu->regs[reg_namb]));
        spu->pc++;
        return SUCCESS;
        break;
    }
    printf("I don't know this command\n");
    return FAIL;
}

int spu_sqvrt_func(int cmd, sput_t* spu)
{
    stack_value pop_value = 0;
    stack_pop(spu->stack, &pop_value);
    stack_push(spu->stack, sqrt(pop_value));
    spu->pc++;
    return SUCCESS;
}

int spu_jmp_func(int cmd, sput_t* spu)
{
    spu->pc = (size_t)spu->code[spu->pc];
}

int spu_call_func(int cmd, sput_t* spu)
{
    stack_push(spu->ret, (int)spu->pc + 2);
    spu->pc = (size_t)spu->code[++spu->pc];
    return SUCCESS;
}

int spu_ret_func(int cmd, sput_t* spu)
{
    stack_value pop_value = 0;
    stack_pop(spu->ret, &pop_value);
    spu->pc = (size_t)pop_value;
    return SUCCESS;
}