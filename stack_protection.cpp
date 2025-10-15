//#include <TXLib.h>
#include <stdio.h>
#include <math.h>
#include "commands.h"
#include "everything_about_stack.h"
#include "stack_protection.h"


int error_decoder(stk_t* stk)
{
    if (!stk)
    {
        printf("Null pointer to structure\n");
        return 1;
    }

    size_t errors = stk->error;

    if (errors & left_canary_died)
        printf("left canary died\n");

    if (errors & right_canary_died)
        printf("right canary died\n");
    
    if (errors & right_canary_died)
        printf("right canary died\n");

    if (errors & null_pointer_to_data)
        printf("Null pointer to data\n");
    
    if (errors & capacity_was_not_assigned)
        printf("Capacity wasn't assigned\n");
    
    if (errors & size_was_not_assigned)
        printf("Size wasn't assigned\n");
    
    if (errors & no_valid_size)
        printf("Size isn't valid\n");
    
    if (errors & memory_not_reallocated)
        printf("Memory wasn't reallocated in calloc\n");

    if (errors & size_more_capacity)
        printf("Size more capacity\n");
    
    if (errors & size_in_pop_is_null)
        printf("Size in pop is null!\n");

    if (errors & data_is_null)
        printf("No valid campacity\n");
    if (errors & data_is_null)
        printf("Pointer to data is null in stack_creator\n");
    return 0;
}

stack_errors stack_err(stk_t* stk)
{
    if (!stk)
    {
        stack_dump(stk, __FILE__, __LINE__, __FUNCTION__);
        printf("Null pointer_to struct\n");
        return null_pointer_to_structure;
    }

    if (stk->data[0] != Сanary_l)
    {
        stk->error |= left_canary_died; 
        printf("left canary died\n");
    }
    if ((stk->data)[stk->capacity + 1] != Сanary_r)
    {
        stk->error |= right_canary_died; 
        printf("right canary died\n");
    }
    if (stk->capacity < 0)
    {
        stk->error |= no_valid_campacity;
        printf("no valid campacity\n");
    }
    if (stk->size < 0)
    {
        stk->error |= no_valid_size;
        printf("no valid size\n");
    }   
    if (stk->size > stk->capacity)
    {
        stk->error |= size_more_capacity;
        printf("stack_size more stack_capacity\n");    
    }
    if(stk->error != 0)
    {
        stack_dump(stk, __FILE__, __LINE__, __FUNCTION__);
        return errors_in_stack;
    }
    return no_errors;
}

void print_data(stack_value* data, ssize_t capacity)
{
    for(int i = 0; i<capacity+2; i++)
        printf("$%d\n", data[i]);
}

void stack_dump(stk_t* stack, const char* file, const int line, const char* func)
{
    printf("StackDump called from %s:%d in function %s()\n", file, line, func);
    void* stack_pointer = stack;
    

    if (!error_decoder(stack))
    {
        ssize_t size = stack->size;
        ssize_t capacity = stack->capacity;
        stack_value* data = stack->data;


        printf("stack addres = %p\n", stack_pointer);
        printf("stack size = %lld\n", size);
        printf("stack capacity = %lld\n", capacity);
        printf("stack data pointer = %p\n", data);

        printf("stack print\n");
        for (ssize_t i = 1; i < size + 1; i++)
            printf("*%d\n", data[i]);
        for (ssize_t i = size + 1; i < capacity + 1; i++)
            printf("%d\n", data[i]);

        printf("That's all, folks\n");
    }
    else
    {
        printf("Nullptr to struct really? u r an idiot NOW???\n");
    }
}