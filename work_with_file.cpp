#include <TXLib.h>
#include <stdio.h>
#include <string.h>
#include "..\Onegin\read_from_file_to_buffer.h"
#include "..\Onegin\output_text_and_pointers_arr.h"




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
    char* buffer = (char*) calloc(size + 2, sizeof(char));
    if (buffer == nullptr)
        {
        printf("Memory for the buffer was not allocated.\n");
        return 0;
        }
    size_t buffer_size = fread(buffer, sizeof(char), size, fp);

    buffer[buffer_size++] = '\n';
    buffer[buffer_size++] = '\0';
    return buffer;
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