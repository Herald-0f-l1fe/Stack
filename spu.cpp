#include "everything_about_stack.h"
#include "stack_protection.h"
#include "stack_operations.h"
#include "spu_func.h"

#define Max_size_t_value 100000000
#define XXXL(func) if(func){spu_dump(spu, __FILE__, __FUNCTION__, __LINE__); return 0;}


int* code_creator(FILE* fp, size_t* size);
int code_calculator(sput_t* spu);
void spu_creator(sput_t* spu, const char* read_file_name, stk_t* stk1, stk_t* ret);
void spu_destructor(sput_t* spu);

int spu_dump(sput_t* spu, const char* file, const char* func, int line);
void code_dump(sput_t* spu);
void reg_dump(sput_t* spu);


int find_cmd_pointer(int cmd, size_t pc);
int new_code_calculator(sput_t* spu);


int main()
{
    const char* str = "test.txt";
    sput_t spu = {0, 0, 0, 0, 0, 0};
    stk_t stk1 = {0, 0, 0, 0};
    stk_t stk_ret ={0, 0, 0, 0};
    spu_creator(&spu, str, &stk1, &stk_ret);
    
    new_code_calculator(&spu);
    //spu_dump(&spu, __FILE__, __FUNCTION__, __LINE__);
    spu_destructor(&spu);

    return 0;
}

int* code_creator(FILE* fp, size_t* size)
{
    int code_size = 10;
    *size = 0;
    int* code = (int*)calloc((size_t)code_size, sizeof(int));
    int code_pointer = 0; 
    printf("Start creating bytecode\n");
    while(fscanf(fp,"%d", &code[code_pointer++]) != EOF)
    {
        if (code_pointer >= code_size - 1)
        {
            code_size *= 2;
            code = (int*) realloc(code, (size_t)code_size*sizeof(int));
        }
        (*size)++;
    }
    code[code_pointer] = HLT;

    return code;
}

int code_calculator(sput_t* spu)
{
    stack_value value = 0, pop_value1 = 0, pop_value2 = 0;
    stk_t* stk = spu->stack;
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

void spu_creator(sput_t* spu, const char* read_file_name, stk_t* stk1, stk_t* ret)
{
    stack_creator(stk1, 10);
    stack_creator(ret, 10);
    spu->ret = ret;
    spu->stack = stk1;
    spu->regs = (int*) calloc(8, sizeof(int));

    FILE* fp = fopen(read_file_name, "r");
    if (fp == nullptr) // NULL
        printf("File %s wasn't opened\n", read_file_name); //func load
    
    spu->size = 0;
    spu->code = code_creator(fp, &spu->size);
    
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
    return 0;
}

int new_code_calculator(sput_t* spu)
{
    int cmd = spu->code[spu->pc];
    while(cmd != HLT)
    {
        //printf("Command number is %d, pc is %d\n", cmd, spu->pc);
        //spu_dump(spu, __FILE__, __FUNCTION__, __LINE__);
        int cmd_ip = find_cmd_pointer(cmd, spu->pc);
        SPU_commands_info[cmd_ip].function (cmd, spu);
        cmd = spu->code[spu->pc];
    
    }
    return SUCCESS;
}

int find_cmd_pointer(int cmd, size_t pc)
{
    for (size_t i = 0; i < SPU_number_of_com; i++)
    {
        if (cmd == SPU_commands_info[i].namber)
            return i;
    }
    printf("I don't know this command, cmd_namber is %d pc == %ld\n", cmd, pc);
    return -1;
}

void code_dump(sput_t* spu)
{
    printf("---------spu bytecode--------\n");
    printf("-----------------------------\n");
    for (int i = 0; i < spu->size; i++)
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