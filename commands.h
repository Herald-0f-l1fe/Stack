#ifndef COM
#define COM

enum commands
{
    PUSH = 0,
    ADD = 1,
    SUB = 2,
    MUL = 3,
    DIV = 4,
    SQVRT = 5,
    OUTP = 6,
    HLT = 7, 
    NO_COM = 8,
    JMP = 9,
    JB = 10,
    JBE = 11,
    JA = 12,
    JAE = 13,
    JE = 14, 
    JNE = 15,
    DISKR = 16,
    SQUARE = 17,
    PUSHREG = 33,
    POPREG = 38,
};

#endif