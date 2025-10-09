#include "commands.h"
#include "../Onegin/read_from_file_to_buffer.h"
#include "../Onegin/output_text_and_pointers_arr.h"

#define VERSION 2
struct asms {
    int* byte_code;
    int* labels;
    int cnt;
    size_t pc;
    size_t capacity;
    strings* array;
};
int command_check(char* com, int line);
strings* array_of_pointers_only(char*, asms*); 
void widen_memory_for_byte_code(int** byte_code, size_t* capacity, size_t pc);
strings* read_file(const char* file_name, long int* file_size, asms* asm1);
void compiler(asms* asm1);
int diskr(int* byte_code, size_t* capacity, int pc);
void byte_code_to_file(const char* output_file_name, int* byte_code, size_t pc);


int main()
{
    asms asm1 = {0, 0, 0, 0, 0, 0};
    long int file_size = 0;
    const char* str = "open_file2.asm";
    const char* output_file_name = "test.txt";
    asm1.pc = 0;
    asm1.capacity = 1000000;
    asm1.byte_code = (int*) calloc(asm1.capacity, sizeof(int));
    asm1.cnt = 0;
    int lab[10] = {-1}; 
    asm1.labels = lab;

    asm1.array = read_file(str, &file_size, &asm1);
    compiler(&asm1);
    byte_code_to_file(output_file_name, asm1.byte_code, asm1.pc);
    free(asm1.byte_code);
    free(asm1.array[0].pointer);
    free(asm1.array);
    return 0;
}

int command_check(char* com, int line)
{
    int com_namb = 8;
    if(!strcmp(com, "HLT"))
        com_namb = HLT;

    else if(!strcmp(com, "PUSH"))
        com_namb = PUSH;

    else if(!strcmp(com, "ADD"))
        com_namb = ADD;

    else if(!strcmp(com, "SUB"))
        com_namb = SUB;

    else if(!strcmp(com, "MUL"))
        com_namb = MUL;

    else if(!strcmp(com, "DIV"))
        com_namb = DIV;

    else if(!strcmp(com, "SQVRT"))
        com_namb = SQVRT;

    else if(!strcmp(com, "OUT"))
        com_namb = OUTP;

    else if(!strcmp(com, "PUSHREG"))
        com_namb = PUSHREG;

    else if(!strcmp(com, "POPREG"))
        com_namb = POPREG;

    else if(!strcmp(com, "JMP"))
        com_namb = JMP;

    else if(!strcmp(com, "JB"))
        com_namb = JB;

    else if(!strcmp(com, "JBE"))
        com_namb = JBE;

    else if(!strcmp(com, "JA"))
        com_namb = JA;

    else if(!strcmp(com, "JAE"))
        com_namb = JAE;

    else if(!strcmp(com, "JE"))
        com_namb = JE;

    else if(!strcmp(com, "JNE"))
        com_namb = JNE;

    else if(!strcmp(com, "DISKR"))
        com_namb = DISKR;
    else if(com[0] == ':')
        return LABEL;
    else
        printf("I don't know this command, line %d\n", line);

    com = {0};
    return com_namb;
}


void widen_memory_for_byte_code(int** byte_code, size_t* capacity, size_t pc)
{
    if (pc >= *capacity - 3)
    {
        *capacity *= 2;
        *byte_code = (int*) realloc(*byte_code, *capacity*sizeof(int));
        if (!*byte_code)
        {
            printf("Memory for byte_code not reallocated\n");
        }
    }
}

strings* read_file(const char* file_name, long int* file_size, asms* asm1)
{
    char* buffer = read_from_file_to_buffer(file_size, file_name);
    asm1->cnt = line_counter(buffer);
    strings* array = array_of_pointers_only(buffer, asm1);
    return array;
}

void compiler(asms* asm1)
{
    for (int i = 0; i < asm1->cnt; i++)
    {
        int pc = (int) asm1->pc;
        char command[20] = {0};
        int value = 1234567;
        char cvalue = 0;
        int com_count = sscanf(asm1->array[i].pointer,"%s %d", command, &value);
        printf("%s %d %d\n", command, value, com_count);
        int com_namb = command_check(command, i);
        if (com_count == 2)
        {
            //widen_memory_for_byte_code(&byte_code, capacity, (size_t)pc);
            asm1->byte_code[pc++] = com_namb;
            asm1->byte_code[pc++] = value;
        }
        else if (com_namb == PUSHREG || com_namb == POPREG)
        {
            sscanf(asm1->array[i].pointer,"%s %c", command, &cvalue);
            //widen_memory_for_byte_code(&byte_code, capacity, (size_t)pc);
            asm1->byte_code[pc++] = com_namb;
            asm1->byte_code[pc++] = (int)cvalue;            
        }
        else if(com_namb == DISKR)
        {
            for (int j = 0; j < pc; j++)
                printf("%d ", asm1->byte_code[j]);
            pc = diskr(asm1->byte_code, &(asm1->capacity), pc);
        }
        else if(com_namb == LABEL)
        {
            sscanf(asm1->array[i].pointer, ":%d", &value);
            asm1->labels[value] = pc;
        }
        else if(command[0] == 'J')
        {
            if (sscanf(asm1->array[i].pointer, "%*s :%d", &value) == 1)
            {
                value = asm1->labels[value];
            }
            asm1->byte_code[pc++] = com_namb;
            asm1->byte_code[pc++] = value;
        }
        else
        {
            //widen_memory_for_byte_code(&byte_code, capacity, (size_t) pc);
            asm1->byte_code[pc++] = com_namb;
        }
        asm1->pc = (size_t)pc;
    }
    
} 

int diskr(int* byte_code, size_t* capacity, int pc)
{
    
    //widen_memory_for_byte_code(&byte_code, capacity, *pc);
    printf("--------%d-----%d-----\n", pc, *capacity);
    byte_code[pc++] = POPREG;
    byte_code[pc++] = int('C');

    //widen_memory_for_byte_code(&byte_code, capacity, *pc);
    byte_code[pc++] = POPREG;
    byte_code[pc++] = int('B');
    
    //widen_memory_for_byte_code(&byte_code, capacity, *pc);
    byte_code[pc++] = POPREG;
    byte_code[pc++] = int('A');
    
    //widen_memory_for_byte_code(&byte_code, capacity, *pc);
    byte_code[pc++] = PUSHREG;
    byte_code[pc++] = int('B');
    
    //widen_memory_for_byte_code(&byte_code, capacity, *pc);
    byte_code[pc++] = PUSHREG;
    byte_code[pc++] = int('B');
    
    //widen_memory_for_byte_code(&byte_code, capacity, *pc);
    byte_code[pc++] = MUL;
    byte_code[pc++] = PUSHREG;
    
    //widen_memory_for_byte_code(&byte_code, capacity, *pc);
    byte_code[pc++] = int('A');
    byte_code[pc++] = PUSHREG;
    
    //widen_memory_for_byte_code(&byte_code, capacity, *pc);
    byte_code[pc++] = int('C');
    byte_code[pc++] = PUSH;
    
    //widen_memory_for_byte_code(&byte_code, capacity, *pc);
    byte_code[pc++] = 4;
    byte_code[pc++] = MUL;
    
    //widen_memory_for_byte_code(&byte_code, capacity, *pc);
    byte_code[pc++] = MUL;
    byte_code[pc++] = SUB;
    printf("======%d ======", pc);
    return pc;
}


void byte_code_to_file(const char* output_file_name, int* byte_code, size_t pc)
{
    FILE* fp = open_output_file(output_file_name);
    for (int i = 0; i <= (int)pc; i++)
    {
        fprintf(fp, "%d ", byte_code[i]);
    }
    fclose(fp);
}
