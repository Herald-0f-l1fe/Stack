//#include <TXLib.h>
#include <stdio.h>
#include <math.h>
#include "everything_about_stack.h"
#include "stack_protection.h"
#include "stack_operations.h"


stack_errors stack_push(stk_t* stk, stack_value value)
{
    if (stack_err(stk))
        printf("Error in stack_push\n");
    
    if (stk->size == stk->capacity)
    {   
        widen_memory(stk);

        if (!stk->data)
        {
            printf("Memory for the stack was not reallocated. Go and fuck you away.\n");
            return memory_not_reallocated;
        }  
    }
    
    stk->data[stk->size+1] = value;
    stk->size++;

    if (stack_err(stk))
        printf("Error in stack_push\n");

    return no_errors;
}

stack_errors stack_pop(stk_t* stk, stack_value* pop_value)
{
    if (stack_err(stk))
    {
        printf("Error before stack_pop\n");
        return stack_err(stk);
    }


    if (stk->size == 0)
    {
        printf("Stack size = 0\n");
        stk->error |= size_in_pop_is_null;
        stack_err(stk);
        return errors_in_stack;
    }


    *pop_value = stk->data[stk->size];
    stk->data[stk->size] = MATAN;
    stk->size--;
    
    if (stack_err(stk))
    {
        printf("Error After stack_pop\n");
        return stack_err(stk);
    }

    return no_errors;
}

stack_errors widen_memory(stk_t* stk)
{
    stack_value* data = (stack_value*)realloc(stk->data, (stk->capacity*2 + 2)*sizeof(stack_value));
    if (data)
    {
        stk->data = data;
        stk->capacity *= 2;
        stk->data[stk->capacity + 1] = Сanary_r;
        return no_errors;
    }
    else
    {
        stk->error |= memory_not_reallocated;
        return memory_not_reallocated;
    }
    return no_errors;
}

void stack_destructor(stk_t* stack)
{
    free(stack->data);
}

stack_errors stack_creator(stk_t* stk, ssize_t capacity)
{

    if(!stk)
        return null_pointer_to_structure;
    
    stk->data = (stack_value*)calloc(capacity + 2, sizeof(stack_value));

    if(!stk->data)
    {
        stk->error |= data_is_null;
        return data_is_null;
    }

    stk->capacity = capacity;

    if (stk->capacity != capacity)
    {
        stk->error |= capacity_was_not_assigned;
        return capacity_was_not_assigned;
    }

    stk->size = 0;
    stk->data[0] = Сanary_l;
    stk->data[capacity + 1] = Сanary_r;
    for (size_t i = 1; i <= stk -> capacity; i++) {
        stk -> data[i] = MATAN;
    }
    if (stack_err(stk))
    {
        printf("Error after creating stack\n");
        return errors_in_stack;
    }
    return no_errors;
}
