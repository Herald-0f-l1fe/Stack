#ifndef EVERY
#define EVERY
#include <ctype.h>
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

enum stack_errors 
{
    no_errors = 0,
    errors_in_stack = 1,
    left_canary_died = 2,
    right_canary_died = 4,
    null_pointer_to_data = 8, 
    capacity_was_not_assigned = 16,
    size_was_not_assigned = 32, 
    no_valid_size = 64, 
    memory_not_reallocated = 128, 
    size_more_capacity = 256,
    size_in_pop_is_null = 512,
    no_valid_campacity = 1024, 
    null_pointer_to_structure
};


#endif