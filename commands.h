#ifndef COM
#define COM
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include </usr/include/strings.h>
enum commands
{
    PUSH = 0,
    IN,
    ADD,
    SUB,
    MUL,
    DIV,
    SQVRT,
    OUTP,
    HLT, 
    NO_COM,
    JMP,
    JB,
    JBE,
    JA,
    JAE,
    JE, 
    JNE,
    DISKR,
    CALL,
    LABEL,
    RET,
    PUSHREG = 33,
    POPREG = 38,
};

#endif