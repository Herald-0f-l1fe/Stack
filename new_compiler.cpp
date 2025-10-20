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

    ON_DEBUG((asm1.str_labels);)

    byte_code_to_file(output_file_name, asm1.byte_code, asm1.pc);
   
    asm_destructor(&asm1);
    return 0;
}


strings* read_file(const char* file_name, long int* file_size, asms* asm1)
{
    char* buffer = read_from_file_to_buffer(file_size, file_name);
    
    asm1->cnt = (size_t) line_counter(buffer); 

    strings* array = array_of_pointers_only(buffer, asm1);    
    

    return array;
}

results compiler(asms* asm1)
{
    asm1->pc = 0;
    char command[MAX_CMD_SIZE] = {};
    int value = 0;
    char svalue[MAX_STR_ARG_SIZE] = {};
    
    for (size_t i = 0; i < asm1->cnt; i++) 
    {
        size_t com_namb = new_command_check(command, i, &value, svalue, asm1);
        if(com_namb == LABEL)
        {
            if (label_create(svalue, asm1->str_labels, asm1->pc))
                ON_DEBUG(printf("LABEL IS CREATE\n");)  

            continue;   
        }

        if (svalue == 0)
        {
            printf("NULLPTR to svalue\n");
            return FAIL;
        }   

        command_to_bytecode(asm1, com_namb, &value, svalue);


    }
    return SUCCESS;

}

results byte_code_to_file(const char* output_file_name, int* byte_code, size_t pc)
{
    FILE* fp = open_output_file(output_file_name);
    
    if (fp == 0)
        return FAIL;

    for (size_t i = 0; i < pc; i++)
    {
        fprintf(fp, "%d ", byte_code[i]);  // fwrite is the best solve to binary files
    }

    fclose(fp);

    return SUCCESS;
}

size_t new_command_check(char* command, size_t line, int* value, char* svalue, asms* asm1) 
{
    sscanf(asm1->array[line].pointer, "%s", command);

    ON_DEBUG(printf("%s\n", command);)

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


    printf("I don't know this command, line %zu\n", line);


    return NO_COM;
}

label_t* label_init(label_t* str_labels)
{
    for (size_t i = 0; i < label_size; i++)
    {
        str_labels[i].name = (char*) calloc(MAX_STR_ARG_SIZE, sizeof(char)); 
        
        ON_DEBUG(printf("LABEL IS CREATED\n");)
        
        str_labels[i].point = -1;

        
        if (str_labels[i].name == nullptr)
            printf("NULLPTR to name of label");
    }

    return str_labels;
}

results label_create(char* name, label_t* str_labels, size_t pc)
{
    if (str_labels == 0)
    {
        printf("Nullptr to labels arr \n");
        return FAIL;
    }
    
    for (size_t i = 0; i < label_size; i++)
    {
        if (str_labels[i].name[0] == '\0' && label_check(name, str_labels) == -1)
        {
            ON_DEBUG(printf("LABEL %s IN ARR\n", str_labels[i].name);)

            strcpy(str_labels[i].name, name);
            str_labels[i].point = (ssize_t)pc;

            return SUCCESS;
        }
    }

    return FAIL;
}

int label_check(char* name, label_t* str_labels)
{
    for (size_t i = 0; i < label_size; i++)
    {
        if (!strcmp(str_labels[i].name, name))
            return (int)str_labels[i].point;
    }

    return FAIL;
}

void label_destructor(label_t* str_labels)
{
    for (size_t i = 0; i < label_size; i++)
    {
        free(str_labels[i].name); 
    }

    free(str_labels);
}

results asm_creator(asms* asm1, const char* str, long int file_size)
{
    asm1->pc = 0;
    asm1->cnt = 0;
    asm1->array = read_file(str, &file_size, asm1);

    asm1->byte_code = (int*) calloc(asm1->cnt * 2, sizeof(int));
    PRP(asm1->byte_code);

    label_t* str_labels = (label_t*) calloc(label_size, sizeof(label_t)); // Where is chek on calloc?
    PRP(str_labels);

    asm1->str_labels = label_init(str_labels);

    return SUCCESS;
}

void lab_dump(label_t* str_labels)
{
    for(size_t i = 0; i < label_size; i++)
        printf("label[%s] = %ld\n", str_labels[i].name, str_labels[i].point);
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
    if (ASM_commands_info[com_namb].arg_type == TYPE_LABEL)
            *value = label_check(svalue, asm1->str_labels);

    asm1->byte_code[asm1->pc++] = ASM_commands_info[com_namb].namber;

    if (ASM_commands_info[com_namb].arg_type != TYPE_NONE)
        asm1->byte_code[asm1->pc++] = *value;
}