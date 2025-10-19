#include "new_compiler.h"
#include "enums_and_libs.h"

int main()
{
    asms asm1 = {};
    long int file_size = 0;
    const char* str = "open_file.asm"; 
    const char* output_file_name = "test.txt"; 

    asm_creator(&asm1, str, file_size); //typedef int arg_t -> todo in header
    
    compiler(&asm1);
    compiler(&asm1);
    lab_dump(asm1.str_labels);

    byte_code_to_file(output_file_name, asm1.byte_code, asm1.pc);
   
    asm_destructor(&asm1);
    return 0;
}

int command_check(char* com, int line)
{
    int com_namb = NO_COM;
    if (!strcmp(com, "HLT"))
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

    else if(!strcmp(com, "RET"))
        com_namb = RET;

    else if (!strcmp(com, "CALL"))
        com_namb = CALL;
    
    else if (!strcmp(com, "IN"))
        com_namb = IN;

    else if(com[0] == ':')
        com_namb = LABEL;
    else
        printf("I don't know this command, line %d\n", line);
    
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
    asm1->cnt = (size_t)line_counter(buffer); // можно сделать внутри read_from_file_to_buffer
    strings* array = array_of_pointers_only(buffer, asm1);
    return array;
}

void compiler(asms* asm1)
{
    asm1->pc = 0;

    char command[20] = {0};
    int value = 0;
    char svalue[32] = {};
    
    for (size_t i = 0; i < asm1->cnt; i++) 
    {
        size_t com_namb = new_command_check(command, i, &value, svalue, asm1);
        if(com_namb == LABEL)
        {
            if (label_create(svalue, asm1->str_labels, asm1->pc))
                printf("LABEL IS CREATE\n");  
            continue;   
        }

        command_to_bytecode(asm1, com_namb, &value, svalue);
    }
}

void byte_code_to_file(const char* output_file_name, int* byte_code, size_t pc)
{
    FILE* fp = open_output_file(output_file_name);
    for (size_t i = 0; i <= (int)pc; i++)
    {
        fprintf(fp, "%d ", byte_code[i]);  // fwrite is the best solve to binary files
    }
    fclose(fp);
}

int new_command_check(char* command, size_t line, int* value, char* svalue, asms* asm1) 
{
    sscanf(asm1->array[line].pointer, "%s", command);
    printf("%s\n", command);
    for (size_t cmd = 0; cmd < ASM_number_of_com; cmd++)  
    {
        if (!strcasecmp(command, ASM_commands_info[cmd].name))
        {
            make_arg(value, svalue, asm1->array[line].pointer, cmd);
            return cmd;
        }
    }
    if (command[0] == ':')
    {
        sscanf(asm1->array[line].pointer, ":%s", svalue);
        return LABEL;
    }
    printf("I don't know this command, line %ld\n", line);
    return NO_COM;
}

label_t* label_init(label_t* str_labels)
{
    for (int i = 0; i < label_size; i++)
    {
        str_labels[i].name = (char*) calloc(32, sizeof(char)); // maybe [32], nahui callloc
        str_labels[i].point = -1;
    }

    return str_labels;
}

int label_create(char* name, label_t* str_labels, size_t pc)
{
    for (int i = 0; i < label_size; i++)
    {
        if (str_labels[i].name[0] == '\0' && label_check(name, str_labels) == -1)
        {
            strcpy(str_labels[i].name, name);
            str_labels[i].point = (int)pc;
            return 1; // What is meaning "1"?
        }
    }
    return 0;
}

int label_check(char* name, label_t* str_labels)
{
    for (int i = 0; i < label_size; i++)
    {
        if (!strcmp(str_labels[i].name, name))
            return str_labels[i].point;
    }

    return -1;
}

void label_destructor(label_t* str_labels)
{
    for (int i = 0; i < label_size; i++)
    {
        free(str_labels[i].name); // if you are mozahist
    }

    free(str_labels);
}

void asm_creator(asms* asm1, const char* str, long int file_size)
{
    asm1->pc = 0;
    asm1->capacity = 1000000; // define ASM_CAPACITY   + nahui 10000000
    asm1->byte_code = (int*) calloc(asm1->capacity, sizeof(int)); // Where is chek on calloc?
    asm1->cnt = 0;
    label_t* str_labels = (label_t*) calloc((size_t) label_size, sizeof(label_t)); // Where is chek on calloc?
    asm1->str_labels = label_init(str_labels);
    asm1->array = read_file(str, &file_size, asm1);
}

void lab_dump(label_t* str_labels)
{
    for(int i = 0; i < label_size; i++) // size_t
        printf("label[%s] = %d\n", str_labels[i].name, str_labels[i].point);
}

void asm_destructor(asms* asm1)
{
    free(asm1->byte_code);
    free(asm1->array[0].pointer);
    free(asm1->array);
    label_destructor(asm1->str_labels);
}

void make_arg(int* value, char* svalue, char* string, size_t i)
{
    if (ASM_commands_info[i].arg_type == TYPE_LABEL)
    {
        sscanf(string, "%*s :%s", svalue);
    }    
    else if (ASM_commands_info[i].arg_type == TYPE_STR)
    {
        sscanf(string, "%*s %s", svalue);
        *value = (int)give_reg_namb(svalue);
    }
    else if (ASM_commands_info[i].arg_type == TYPE_DIGIT)
    {
       sscanf(string, "%*s %d", value);
    }
}

size_t give_reg_namb(const char* reg_name)
{
    size_t cnt = sizeof(registrs)/sizeof(registrs[0]);
    for (size_t i = 0; i < cnt; i++)
    {
        if (!strcmp(reg_name, registrs[i].reg_name))
            return registrs[i].reg_value;
    }
    printf("Syntax error\n");
    return 0;
}

void command_to_bytecode(asms* asm1, size_t com_namb, int* value, char* svalue)
{
    if(ASM_commands_info[com_namb].arg_type == TYPE_LABEL)
            *value = label_check(svalue, asm1->str_labels);

        asm1->byte_code[asm1->pc++] = ASM_commands_info[com_namb].namber;

        if (ASM_commands_info[com_namb].arg_type != TYPE_NONE)
        {
            asm1->byte_code[asm1->pc++] = *value;
        }
}