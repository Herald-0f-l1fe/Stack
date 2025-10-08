#include <TXLib.h>
#include <stdio.h>
#include "commands.h"
#include <ctype.h>
#include "..\Onegin\read_from_file_to_buffer.h"
#include "..\Onegin\output_text_and_pointers_arr.h"

int command_check(char* com, int line);
strings* array_of_pointers_only(char*, int); 

int main()
{
    long int file_size = 0;
    const char* str = "open_file.asm";
    const char* output_file_name = "test.txt";
    FILE* output_file = open_output_file(output_file_name);

    char* buffer = read_from_file_to_buffer(&file_size, str);
    if (!buffer)
        return 3;
        
    int cnt = line_counter(buffer);
    strings* array = array_of_pointers_only(buffer, cnt);

    for (int i = 0; i < cnt; i++)
    {
        char command[10] = {0};
        sscanf(array[i].pointer,"%s", command);
        int com_namb = command_check(command, i);
        if (com_namb == PUSH || com_namb == JMP || com_namb == JB || com_namb == JBE || com_namb == JA || com_namb == JAE || com_namb == JE || com_namb == JNE)
        {
            int push_value = 0;
            sscanf(array[i].pointer, "%*s %d", &push_value);
            fprintf(output_file, "%d %d ", com_namb, push_value);
        }
        else if (com_namb == PUSHREG || com_namb == POPREG)
        {
            char reg_value = 0;
            sscanf(array[i].pointer, "%*s %c", &reg_value);
            fprintf(output_file, "%d %d ", com_namb, (int)reg_value);
        }
        else if (com_namb == DISKR)
        {
            fprintf(output_file, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d ", POPREG, 'C', POPREG, 'B', POPREG, 'A', PUSHREG, 'B', PUSHREG, 'B', MUL, PUSHREG, 'A', PUSHREG, 'C', PUSH, 4, MUL, MUL, SUB);
        }
        else
        {
            fprintf(output_file, "%d ", com_namb);
        }
    }
    fclose(output_file);
    free(buffer);
    
}


int command_check(char* com, int line)
{
    int com_namb = 8;
    if(!stricmp(com, "HLT"))
        com_namb = HLT;

    else if(!stricmp(com, "PUSH"))
        com_namb = PUSH;

    else if(!stricmp(com, "ADD"))
        com_namb = ADD;

    else if(!stricmp(com, "SUB"))
        com_namb = SUB;

    else if(!stricmp(com, "MUL"))
        com_namb = MUL;

    else if(!stricmp(com, "DIV"))
        com_namb = DIV;

    else if(!stricmp(com, "SQVRT"))
        com_namb = SQVRT;

    else if(!stricmp(com, "OUT"))
        com_namb = OUTP;

    else if(!stricmp(com, "PUSHREG"))
        com_namb = PUSHREG;

    else if(!stricmp(com, "POPREG"))
        com_namb = POPREG;

    else if(!stricmp(com, "JMP"))
        com_namb = JMP;

    else if(!stricmp(com, "JB"))
        com_namb = JB;

    else if(!stricmp(com, "JBE"))
        com_namb = JBE;

    else if(!stricmp(com, "JA"))
        com_namb = JA;

    else if(!stricmp(com, "JAE"))
        com_namb = JAE;

    else if(!stricmp(com, "JE"))
        com_namb = JE;

    else if(!stricmp(com, "JNE"))
        com_namb = JNE;

    // else if(!stricmp(com, "DISKR"))
    //     com_namb = DISKR;

    else
        printf("I don't know this command, line %d\n", line);

    com = {0};
    return com_namb;
}

void create_byte_code(FILE* fp, strings* array,     )