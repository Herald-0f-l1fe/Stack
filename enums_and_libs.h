#ifndef ENUMS_AND_LIBS
#define ENUMS_AND_LIBS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
//#define DEBUG
#include "DEBUG.h"


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

enum results
{
    FAIL = -1,
    SUCCESS = 1,
};

enum tips
{
    TYPE_NONE, 
    TYPE_CHAR,
    TYPE_DIGIT,
    TYPE_LABEL,
    TYPE_STR,
};

#endif