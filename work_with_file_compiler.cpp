//#include <TXLib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../Onegin/read_from_file_to_buffer.h"
#include "../Onegin/output_text_and_pointers_arr.h"
struct asms {
    int* byte_code;
    int* labels;
    int cnt;
    size_t pc;
    size_t capacity;
    strings* array;
};

strings* array_of_pointers_only(char*, asms*); 
void n_to_o(strings* arr, int cnt);


char* read_from_file_to_buffer(long int* size, const char* str)
{
    FILE* fp = fopen(str, "r");
    if (fp == nullptr) // NULL
        printf("File %s didn't open\n", str);

    *size = finding_file_size(str);
    //printf("meow\n");

    char* buffer = filling_the_buffer_with_text(*size, fp);

    fclose(fp);
    return buffer;
}

long int finding_file_size(const char* str)
{
    struct stat file_info = {};
    if (stat(str, &file_info) == 0) 
    {
        //printf("Size of file %ld\n", file_info.st_size);
    }
    else
    {
        printf("smth went wrong, erasing all data on hard drive...\n");
    }
    return file_info.st_size;
}

char* filling_the_buffer_with_text(long int size, FILE* fp)
{
    char* buffer = (char*) calloc((size_t)size + 2, sizeof(char));
    if (buffer == nullptr)
        {
        printf("Memory for the buffer was not allocated.\n");
        return 0;
        }
    size_t buffer_size = fread(buffer, sizeof(char), (size_t) size, fp);

    buffer[buffer_size++] = '\n';
    buffer[buffer_size++] = '\0';
    return buffer;
}

strings* array_of_pointers_only(char* buffer, asms* asm1)
{
    strings* array = (strings*)calloc((size_t)asm1->cnt, sizeof(strings));
    if (array == 0)
        printf("Memory for the only_pointer_arr was not allocated.\n");
    
    char* buffer1 = strchr(buffer, '\n');
    array[0].pointer = buffer;
    array[0].len = strchr(buffer, '\n') - array[0].pointer;
    
    for (int i = 1; i < asm1->cnt && buffer1 != NULL; i++)
    {
        while (*(buffer1 + 1) == '\n')
        {
            buffer1++;
        }
    
        array[i].pointer = buffer1 + 1;
        array[i].len = strchr(buffer1 + 1, '\n') - array[i].pointer;
        buffer1 = strchr(buffer1 + 1, '\n');
    }
    //n_to_o(array, cnt);
    return array;
}

FILE* open_output_file(const char* output_file_name)
{
    FILE* file = fopen(output_file_name, "w");
    if (file == NULL) 
    {
        printf("Failed to open file for output");
    }
        return file;
}

int line_counter(char* buffer)
{
    int cnt = 0;
    for (char* buffer1 = strchr(buffer, '\n'); buffer1 != NULL; buffer1 = strchr(buffer1 + 1, '\n'))
    {
        if (*(buffer1 + 1) != '\n')
            cnt++;
    }

    return cnt;
}

void n_to_o(strings* arr, int cnt)
{
   for (int i = 0; i < cnt; i++)
   {
        arr[i].pointer[arr[i].len - 1] = '\0';
   }
}