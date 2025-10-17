#ifndef COM
#define COM
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include </usr/include/strings.h>
enum commands
{
    PUSH,
    IN,
    ADD,
    SUB,
    MUL,
    DIV,
    SQVRT,
    OUTP,
    HLT, 
    NO_COM,
    LABEL,
    JMP,
    JB,
    JBE,
    JA,
    JAE,
    JE, 
    JNE,
    CALL,
    RET,    
    PUSHREG = 32,
    POPREG = 39,
};

enum tips
{
    TYPE_NONE, 
    TYPE_CHAR,
    TYPE_DIGIT,
    TYPE_LABEL,
    TYPE_STR,
};


struct label_t
{
    char* name;
    int point; // size_t
};
int label_size = 10;    

struct com_check_t 
{
    const char* name;
    commands namber;
    tips arg_type;
};

com_check_t commands_info[] = 
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

size_t number_of_com = sizeof(commands_info)/sizeof(commands_info[0]);

#endif