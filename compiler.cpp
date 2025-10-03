#include <TXLib.h>
#include <stdio.h>
#include "commands.h"
#include <ctype.h>
#include "..\Onegin\read_from_file_to_buffer.h"
#include "..\Onegin\output_text_and_pointers_arr.h"



char* sort_buffer(char* buffer, long int size);
int command_check(char* command, int* pos_in_command);

int main()
{
    long int size = 0;
    const char* open_file = "open_file.txt";
    char* buffer = read_from_file_to_buffer(&size, open_file);
    char* s_buff = sort_buffer(buffer, size);
    printf("%s\n", s_buff);
    const char* output_file = "test.txt";
    FILE* fp = open_output_file(output_file);
    fputs(s_buff, fp);
    fclose(fp);
    free(buffer);
    free(s_buff);
    return 0;
}

char* sort_buffer(char* buffer, long int size)
{
    char* sort_buf = (char*)calloc(size+1, sizeof(char));
    char command[10] = {};
    int pos_in_command = 0;
    for(int i = 0, j = 0; i < size + 1;i++)
    {
        while(isalpha(buffer[i]))
        {
            command[pos_in_command++] = buffer[i++];
        }
        if (pos_in_command)
        {
            int com_namb = command_check(command, &pos_in_command);
            if(com_namb != 8)
            {
                sort_buf[j++] = (char)(com_namb + '0');
                sort_buf[j++] = ' ';
                com_namb = 8;
            }
            else
                printf("I don't know this command\n");
        }
        bool d = false;
        while(isdigit(buffer[i]))
        {
            sort_buf[j++] = buffer[i++];
            d = true;
        }
        if (d)
        {
            sort_buf[j++] = ' ';
            d = false;
        }
    }
    return sort_buf;
}


int command_check(char* command, int* pos_in_command)
{
    int com_namb = 8;
    char* com = (char*)calloc(*pos_in_command, sizeof(char));
    for (int i = 0; i<*pos_in_command; i++)
        com[i] = command[i];
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
    command = {0};
    *pos_in_command = 0;
    return com_namb;
}