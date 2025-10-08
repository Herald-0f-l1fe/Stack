#ifndef OPERATIONS
#define OPERATIONS
#include "everything_about_stack.h"
#define MATAN 666
stack_errors stack_push(stack* stk, stack_value value);
stack_errors stack_creator(stack* stk, ssize_t capacity);
stack_errors stack_pop(stack* stk, stack_value* pop_value);
stack_errors widen_memory(stack* stk);
void stack_destructor(stack* stack);


#endif