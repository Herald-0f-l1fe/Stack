//#include <TXLib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "commands.h"
#include "everything_about_stack.h"
#include "stack_protection.h"
#include "stack_operations.h"

#define Max_size_t_value 100000000
#define IS_work(func) if(func){return 0;}
//#define dump(x) stack_dump(x, __FILE__, __LINE__)


struct sput 
{
    int* code;
    size_t pc;
    stack* stk; //поменять название структуры на stkt, а stk на stack
    int* regs;
};

void calculator(stack* stack);
int* code_creator(FILE* fp);
void code_calculator(sput* spu);



int main()
{
    stack stk1 = {0,0,0,0};
    stack_creator(&stk1, 5);
    const char* str = "test.txt";
    FILE* fp = fopen(str, "r");

    if (fp == nullptr) // NULL
        printf("File %s didn't open\n", str);
    int* regs = (int*) calloc(8, sizeof(int));

    int* code = code_creator(fp);

    sput spu = {code, 0, &stk1, regs};

    code_calculator(&spu);
    stack_destructor(&stk1);
    
    return 0;
}

void calculator(stack* stk)
{
    char command[10] = {};
    stack_value value = 0, pop_value1 = 0, pop_value2 = 0; 
    scanf("%s", command);
    while(strcmp(command, "HLT"))
    {
        if(!strcmp(command,"PUSH"))
        {
            scanf("%d", &value);
            stack_push(stk, value);
        }
        else if(!strcmp(command,"ADD"))
        {
            stack_pop(stk, &pop_value1);
            stack_pop(stk, &pop_value2);
            stack_push(stk, pop_value2 + pop_value1);
        }
        
        else if(!strcmp(command,"SUB"))
        {
            stack_pop(stk, &pop_value1);
            stack_pop(stk, &pop_value2);
            stack_push(stk, pop_value2 - pop_value1);
        }

        else if(!strcmp(command,"MUL"))
        {
            stack_pop(stk, &pop_value1);
            stack_pop(stk, &pop_value2);
            stack_push(stk, pop_value2 * pop_value1);
        }

        else if(!strcmp(command,"DIV"))
        {
            stack_pop(stk, &pop_value1);
            stack_pop(stk, &pop_value2);
            stack_push(stk, pop_value2 / pop_value1);
        }

        else if(!strcmp(command,"SQVRT"))
        {
            stack_pop(stk, &pop_value1);
            stack_push(stk, sqrt((int)pop_value1));
        }

        else if(!strcmp(command,"OUT"))
        {
            stack_pop(stk, &pop_value1);
            printf("Out value is %d\n", pop_value1);
        }

        else
        {
            printf("I don't know this command");
        }
        scanf("%s", command);
    }
}



int* code_creator(FILE* fp)
{
    int code_size = 10;
    int* code = (int*)calloc(code_size, sizeof(int));
    int code_pointer = 0; 
    while(fscanf(fp,"%d", &code[code_pointer++]) != EOF)
    {
        if (code_pointer == code_size)
        {
            code_size *= 2;
            code = (int*) realloc(code, code_size*sizeof(int));
        }
    }
    code[code_pointer] = HLT;

    return code;
}





void code_calculator(sput* spu)
{
    stack_value value = 0, pop_value1 = 0, pop_value2 = 0;
    stack* stk = spu->stk;
    int com = spu->code[spu->pc++];
    int reg_namb = 0;
    while(com != HLT)
    {
        switch (com)
        {
        case PUSH:
            value = spu->code[spu->pc++];
            stack_push(stk, value);
            break;
        case ADD:
            stack_pop(stk, &pop_value1);
            stack_pop(stk, &pop_value2);
            stack_push(stk, pop_value2 + pop_value1);
            break;
        case SUB:
            stack_pop(stk, &pop_value1);
            stack_pop(stk, &pop_value2);
            stack_push(stk, pop_value2 - pop_value1);
            break;
        case MUL:
            stack_pop(stk, &pop_value1);
            stack_pop(stk, &pop_value2);
            stack_push(stk, pop_value2 * pop_value1);
            break;
        case DIV:
            stack_pop(stk, &pop_value1);
            stack_pop(stk, &pop_value2);
            stack_push(stk, pop_value2 / pop_value1);
            break;
        case SQVRT:
            stack_pop(stk, &pop_value1);
            stack_push(stk, sqrt(pop_value1));
            break;
        case OUTP:
            stack_pop(stk, &pop_value1);
            printf("Out value is %d\n", pop_value1);
            break;
        case PUSHREG:
            reg_namb = spu->code[spu->pc++] -(int)'A';
            stack_push(stk, spu->regs[reg_namb]);
            break;
        case POPREG:
            reg_namb = spu->code[spu->pc++] - (int)'A';
            stack_pop(stk, &(spu->regs[reg_namb]));
            break;
        case JMP:
            spu->pc = spu->code[spu->pc];
            break;
        case JB:
            stack_pop(stk, &pop_value1);
            stack_pop(stk, &pop_value2);
            if(pop_value2<pop_value1)
                spu->pc = spu->code[spu->pc];
            else
                spu->pc++;
            break;
        case JBE:
            stack_pop(stk, &pop_value1);
            stack_pop(stk, &pop_value2);
            if(pop_value2 <= pop_value1)
                spu->pc = spu->code[spu->pc];
            else
                spu->pc++;
            break;
        case JA:
            stack_pop(stk, &pop_value1);
            stack_pop(stk, &pop_value2);
            if(pop_value2 > pop_value1)
                spu->pc = spu->code[spu->pc];
            else
                spu->pc++;
            break;
        case JAE:
            stack_pop(stk, &pop_value1);
            stack_pop(stk, &pop_value2);
            if(pop_value2 >= pop_value1)
                spu->pc = spu->code[spu->pc];
            else
                spu->pc++;
            break;
        case JE:
            stack_pop(stk, &pop_value1);
            stack_pop(stk, &pop_value2);
            if(pop_value2 == pop_value1)
                spu->pc = spu->code[spu->pc];
            else
                spu->pc++;
            break;
        case JNE:
            stack_pop(stk, &pop_value1);
            stack_pop(stk, &pop_value2);
            if(pop_value2 != pop_value1)
                spu->pc = spu->code[spu->pc];
            else
                spu->pc++;
            break;
        default:
            printf("I don't know this command\n");
        }
        com = spu->code[spu->pc++];
    }
}