#include <TXLib.h>
#include <stdio.h>

#define Max_size_t_value 1000000000
#define MATAN 666
enum stack_errors 
{
    no_errors,
    null_pointer_to_structure,
    null_pointer_to_data, 
    capacity_was_not_assigned,
    size_was_not_assigned, 
    no_valid_campacity, no_valid_size, 
    memory_not_reallocated, 
    size_more_capacity,
    size_in_pop_is_null
};

typedef int stack_value;
const stack_value canary_l =  987149;
const stack_value canary_r = 12648430;
struct stack 
{
    stack_value* data;
    ssize_t size;
    ssize_t capacity;
    size_t error;
};

stack_errors stack_push(stack* borizsche_kolbassischkhzsche, stack_value jopa);
stack_errors stack_creator(stack*, ssize_t);
stack_errors stack_pop(stack*, stack_value*);
stack_errors stack_err(stack); //*
void stack_dumb(stack* stack);
stack_errors widen_memory(stack_value** data, ssize_t capacity);
void print_data(stack_value* data, ssize_t capacity);
void stack_destructor(stack* stack);



int main()
{
    stack stk1 = {0,0,0,0};
    stack_creator(&stk1, 5);
    print_data(stk1.data, stk1.capacity);

    stack_value value = 10;
    stack_push(&stk1, value);
    stack_dumb(&stk1);
    print_data(stk1.data, stk1.capacity);
    
    value = 20;
    stack_push(&stk1, value);
    stack_dumb(&stk1);
    
    stack_value pop_value = 0;
    stack_pop(&stk1, &pop_value);
    stack_dumb(&stk1);
    printf("pop_value1 = %d\n", pop_value);


    stack_pop(&stk1, &pop_value);
    printf("pop_value2 = %d\n", pop_value);
    stack_dumb(&stk1);
    print_data(stk1.data, stk1.capacity);   
    
    
    stack_destructor(&stk1);
    return 0;
}

stack_errors stack_creator(stack* stk, ssize_t capacity)
{
    if (stack_err(*stk))
        {
        printf("Error in stack_creator\n");
        return stack_err(*stk);
        }
    
    stk->data = (stack_value*)calloc(capacity + 2, sizeof(stack_value));
    
    stk->capacity = capacity;
    if (stk->capacity != capacity)
        return capacity_was_not_assigned;
    
    stk->size = 0;
    if (stack_err(*stk))
    {
        printf("Error after creating stack\n");
        return(stack_err(*stk));
    }
    stk->data[0] = canary_l;
    stk->data[capacity+1] = canary_r;
    return no_errors;
}


stack_errors stack_push(stack* stk, stack_value value)
{
    if (stack_err(*stk))
        printf("Error in stack_push\n");
    
    if (stk->size == stk->capacity)
    {
        //stk->data = (stack_value*)realloc(stk->data, (stk->capacity*2 + 2)sizeof(stack_value));
        widen_memory(&(stk->data), stk->capacity);
        stk->capacity *= 2;
        stk->data[stk->capacity + 1] = canary_r;
        if (!stk->data)
        {
            printf("Memory for the stack was not reallocated. Go and fuck you away.\n");
            return memory_not_reallocated;
        }  
    }
    
    stk->data[stk->size+1] = value;
    stk->size++;
    return no_errors;
}


stack_errors stack_pop(stack* stk, stack_value* pop_value)
{
    if (stack_err(*stk))
        {
            printf("Error before stack_pop\n");
            return stack_err(*stk);
        }
    if (stk->size == 0)
    {
        printf("Stack size = 0");
        return size_in_pop_is_null;
    }
    *pop_value = stk->data[stk->size];
    stk->data[stk->size] = MATAN;
    stk->size--;
    
    if (stack_err(*stk))
    {
        printf("Error After stack_pop\n");
        return stack_err(*stk);
    }

    return no_errors;
}

stack_errors stack_err(stack stk)
{
    if (!(&stk))
        return null_pointer_to_structure;

    if (stk.capacity < 0)
        return no_valid_campacity;
    
    if (stk.size < 0)
        return no_valid_size;
        
    if (stk.size>stk.capacity)
        return size_more_capacity;    
    
    return no_errors;
}

void stack_dumb(stack* stack)
{
    void* stack_pointer = stack;
    ssize_t size = stack->size;
    ssize_t capacity = stack->capacity;
    stack_value* data = stack->data;

    if (&stack)
    {
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
    //free(data);
    //data ++; 
    }
    else
    {
        printf("Nullptr to struct really? u r an idiot NOW???\n");
    }
}
stack_errors widen_memory(stack_value** data, ssize_t capacity)
{
    *data = (stack_value*)realloc(*data, (capacity*2 + 2)*sizeof(stack_value));
    if (!data)
        return no_errors;
    else
        return memory_not_reallocated;
    return no_errors;
}


void print_data(stack_value* data, ssize_t capacity)
{
    for(int i = 0; i<capacity+2; i++)
        printf("$%d\n", data[i]);
}

void stack_destructor(stack* stack)
{
    free(stack->data);
}