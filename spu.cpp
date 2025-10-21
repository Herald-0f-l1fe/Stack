#include "everything_about_stack.h"
#include "stack_protection.h"
#include "stack_operations.h"
#include "spu_func.h"

#define XXXL(func) if(func){spu_dump(spu, __FILE__, __FUNCTION__, __LINE__); return 0;}

#define STACK_SIZE 10
#define REG_SIZE 8
#define RAM_SIZE 100

int* code_creator(FILE* fp, size_t* size);
int code_calculator(sput_t* spu);
results spu_creator(sput_t* spu, const char* read_file_name, stack_t* stk1, stack_t* ret);
void spu_destructor(sput_t* spu);

int spu_dump(sput_t* spu, const char* file, const char* func, int line);
void code_dump(sput_t* spu);
void reg_dump(sput_t* spu);


ssize_t find_cmd_pointer(int cmd, size_t pc);
int new_code_calculator(sput_t* spu);


int main()
{
    const char* str = "test.txt";
    sput_t spu = {0};
    stack_t stk1 = {0};
    stack_t stk_ret ={0};

    spu_creator(&spu, str, &stk1, &stk_ret);
    
    new_code_calculator(&spu);
    ON_DEBUG(spu_dump(&spu, __FILE__, __FUNCTION__, __LINE__);)
    spu_destructor(&spu);

    return 0;
}

int* code_creator(FILE* fp, size_t* size)
{
    int code_size = 10;
    *size = 0;
    int* code = (int*)calloc((size_t)code_size, sizeof(int)); // check
    int code_pointer = 0; 

    ON_DEBUG(printf("Start creating bytecode\n");)

    while(fscanf(fp,"%d", &code[code_pointer++]) != EOF)
    {
        if (code_pointer >= code_size - 1)
        {
            code_size *= 2;
            code = (int*) realloc(code, (size_t)code_size*sizeof(int)); // check
        }
        (*size)++;
    }
    code[code_pointer] = HLT;

    ON_DEBUG(printf("Finish creating bytecode\n");)

    return code;
}

int code_calculator(sput_t* spu)
{
    stack_value value = 0, pop_value1 = 0, pop_value2 = 0;
    stack_t* stk = spu->stack;
    int com = spu->code[spu->pc++];
    int reg_namb = 0;
    while(com != HLT)
    {
        switch (com) //в массиве с командами указатель на функцию, поэтому проход
        {
        case PUSH:
            value = spu->code[spu->pc++];
            XXXL(stack_push(stk, value));
            break;
        case ADD:
            XXXL(stack_pop(stk, &pop_value1));
            XXXL(stack_pop(stk, &pop_value2));
            XXXL(stack_push(stk, pop_value2 + pop_value1));
            break;
        case SUB:
            XXXL(stack_pop(stk, &pop_value1));
            XXXL(stack_pop(stk, &pop_value2));
            XXXL(stack_push(stk, pop_value2 - pop_value1));
            break;
        case MUL:
            XXXL(stack_pop(stk, &pop_value1));
            XXXL(stack_pop(stk, &pop_value2));
            XXXL(stack_push(stk, pop_value2 * pop_value1));
            break;
        case DIV:
            XXXL(stack_pop(stk, &pop_value1));
            XXXL(stack_pop(stk, &pop_value2));
            XXXL(stack_push(stk, pop_value2 / pop_value1));
            break;
        case SQVRT:
            XXXL(stack_pop(stk, &pop_value1));
            XXXL(stack_push(stk, (int)sqrt(pop_value1)));
            break;
        case OUTP:
            XXXL(stack_pop(stk, &pop_value1));
            printf("Out value is %d\n", pop_value1);
            break;
        case PUSHREG:
            reg_namb = spu->code[spu->pc++];
            XXXL(stack_push(stk, spu->regs[reg_namb]));
            break;
        case POPREG:
            reg_namb = spu->code[spu->pc++];
            XXXL(stack_pop(stk, &(spu->regs[reg_namb])));
            break;
        case JMP:
            spu->pc = (size_t)spu->code[spu->pc];
            break;
        case JB:
            XXXL(stack_pop(stk, &pop_value1));
            XXXL(stack_pop(stk, &pop_value2));
            if(pop_value2<pop_value1)
                spu->pc = (size_t)spu->code[spu->pc];
            else
                spu->pc++;
            break;
        case JBE:
            XXXL(stack_pop(stk, &pop_value1));
            XXXL(stack_pop(stk, &pop_value2));
            if(pop_value2 <= pop_value1)
                spu->pc = (size_t)spu->code[spu->pc];
            else
                spu->pc++;
            break;
        case JA:
            XXXL(stack_pop(stk, &pop_value1));
            XXXL(stack_pop(stk, &pop_value2));
            if(pop_value2 > pop_value1)
                spu->pc = (size_t)spu->code[spu->pc];
            else
                spu->pc++;
            break;
        case JAE:
            XXXL(stack_pop(stk, &pop_value1));
            XXXL(stack_pop(stk, &pop_value2));
            if(pop_value2 >= pop_value1)
                spu->pc = (size_t)spu->code[spu->pc];
            else
                spu->pc++;
            break;
        case JE:
            XXXL(stack_pop(stk, &pop_value1));
            XXXL(stack_pop(stk, &pop_value2));
            if(pop_value2 == pop_value1)
                spu->pc = (size_t)spu->code[spu->pc];
            else
                spu->pc++;
            break;
        case JNE:
            XXXL(stack_pop(stk, &pop_value1));
            XXXL(stack_pop(stk, &pop_value2));
            if(pop_value2 != pop_value1)
                spu->pc = (size_t)spu->code[spu->pc];
            else
                spu->pc++;
            break;
        case CALL:
            XXXL(stack_push(spu->ret, (int)spu->pc + 1));
            spu->pc = (size_t)spu->code[spu->pc];
            break;
        case RET:
            XXXL(stack_pop(spu->ret, &pop_value1));
            spu->pc = (size_t)pop_value1;
            break;
        case IN:
            scanf("%d", &value);
            XXXL(stack_push(stk, value));
            break;
        default:
            printf("I don't know this command\n");
        }
        com = spu->code[spu->pc++];
    }
    return 0;
}

results spu_creator(sput_t* spu, const char* read_file_name, stack_t* stk1, stack_t* ret)
{
    stack_creator(stk1, STACK_SIZE);
    stack_creator(ret, STACK_SIZE);

    spu->ret = ret;
    spu->stack = stk1;
    spu->regs = (int*) calloc(REG_SIZE, sizeof(int)); // check

    FILE* fp = fopen(read_file_name, "r");
    if (fp == nullptr)
    {
        printf("File %s wasn't opened\n", read_file_name); //func load
        return FAIL;
    }
    spu->size = 0;
    spu->code = code_creator(fp, &spu->size);

    spu->RAM = (stack_value*) calloc(RAM_SIZE, sizeof(stack_value)); // check

    return SUCCESS;
    
}

void spu_destructor(sput_t* spu)
{
    stack_destructor(spu->stack);
    stack_destructor(spu->ret);

    free(spu->code);
    free(spu->regs);
}

int spu_dump(sput_t* spu, const char* file, const char* func, int line)
{
    printf("ProcessorDump called from %s:%d in function %s()\n", file, line, func);
    
    printf("spu [%p]\n", spu);
    printf(".size = %lu,\n.code = %p\n", spu -> size, spu -> code);
    
    code_dump(spu);
    reg_dump(spu);

    stack_dump(spu -> stack, file, line, func);
    stack_dump(spu -> ret, file, line, func);

    return 0;
}

int new_code_calculator(sput_t* spu)
{
    int cmd = spu->code[spu->pc];
    while(cmd != HLT)
    {
        ON_DEBUG(printf("Command number is %d, pc is %lu\n", cmd, spu->pc);
        spu_dump(spu, __FILE__, __FUNCTION__, __LINE__);)

        ssize_t cmd_ip = find_cmd_pointer(cmd, spu->pc);

        SPU_commands_info[cmd_ip].function (cmd, spu);

        cmd = spu->code[spu->pc];
    
    }


    return SUCCESS;
}

ssize_t find_cmd_pointer(int cmd, size_t pc)
{
    for (size_t i = 0; i < SPU_number_of_com; i++)
    {
        if (cmd == SPU_commands_info[i].namber)
            return (ssize_t) i;
    }
    printf("I don't know this command, cmd_namber is %d pc == %lu\n", cmd, pc);
    return FAIL;
}

void code_dump(sput_t* spu)
{
    printf("---------spu bytecode--------\n");
    printf("-----------------------------\n");
    for (size_t i = 0; i < spu->size; i++)
    {
        printf("%d ", spu->code[i]);
    }
    printf("\n-------------------------------\n");
    printf("--------END OF BYTECODE--------\n");
}

void reg_dump(sput_t* spu)
{
    printf("---------spu registrs--------\n");
    printf("-----------------------------\n");
    for (int i = 0; i < 8; i++)
    {
        printf("Reg[%d] = %d\n", i, spu->regs[i]);
    }
    printf("---------end registrs--------\n");
    printf("-----------------------------\n");   
}