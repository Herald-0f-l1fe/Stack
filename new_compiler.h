#ifndef COMPILER
#define COMPILER
#include "../Onegin/read_from_file_to_buffer.h"
#include "../Onegin/output_text_and_pointers_arr.h"
#include "enums_and_libs.h"


#define MAX_CMD_SIZE 20
#define MAX_STR_ARG_SIZE 32
#define NUM_LAB 10

struct label_t
{
    char* name;
    ssize_t point;
};
size_t label_size = 10;

struct asms 
{
    int* byte_code;
    label_t* str_labels;
    size_t cnt; 
    size_t pc;
    strings* array;
};


struct com_check_t 
{
    const char* name;
    commands namber;
    tips arg_type;
};


com_check_t ASM_commands_info[] = 
{
    {"PUSH", PUSH, TYPE_DIGIT},
    {"IN", IN, TYPE_NONE},
    {"ADD", ADD, TYPE_NONE},
    {"SUB", SUB, TYPE_NONE},
    {"MUL", MUL, TYPE_NONE},
    {"DIV", DIV, TYPE_NONE},
    {"SQVRT", SQVRT, TYPE_NONE},
    {"OUT", OUTP, TYPE_NONE},
    {"HLT", HLT, TYPE_NONE},
    {"NO_COM", NO_COM, TYPE_NONE},
    {"LABEL", LABEL, TYPE_NONE},
    {"JMP", JMP, TYPE_LABEL},
    {"JB", JB, TYPE_LABEL},
    {"JA", JA, TYPE_LABEL},
    {"JBE", JBE, TYPE_LABEL},
    {"JAE", JAE, TYPE_LABEL},
    {"JE", JE, TYPE_LABEL},
    {"JNE", JNE, TYPE_LABEL},
    {"CALL", CALL, TYPE_LABEL},
    {"RET", RET, TYPE_NONE},
    {"PUSHREG", PUSHREG, TYPE_STR},
    {"POPREG", POPREG, TYPE_STR},
};

size_t ASM_number_of_com = sizeof(ASM_commands_info)/sizeof(ASM_commands_info[0]);

struct reg_names
{
    const char* reg_name;
    size_t reg_value;
};

reg_names registrs[] = {{"AX", 0}, {"BX", 1}, {"CX", 2}, {"DX", 3}};




results asm_creator(asms* asm1, const char* str, long int file_size); 
void asm_destructor(asms* asm1);   
strings* array_of_pointers_only(char*, asms*); 
strings* read_file(const char* file_name, long int* file_size, asms* asm1);

results assmbler(asms* asm1);
int command_check(char* com, int line);
results compiler(asms* asm1);
size_t new_command_check(char* com, size_t line, int* value, char* svalue, asms* asm1);
void make_arg(int* value, char* svalue, char* string, size_t i);
size_t give_reg_namb(const char* reg_name);

void command_to_bytecode(asms* asm1, size_t com_namb, int* value, char* svalue);
results byte_code_to_file(const char* output_file_name, int* byte_code, size_t pc);




label_t* label_init(label_t* str_labels);
results label_create(char* name, label_t* str_labels, size_t pc);
int label_check(char* name, label_t* str_labels);
void label_destructor(label_t* str_labels);
void lab_dump(label_t* str_labels);



#endif