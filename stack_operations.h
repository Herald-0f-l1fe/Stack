#ifndef OPERATIONS
#define OPERATIONS
#include "everything_about_stack.h"
#define MATAN 666
stack_errors stack_push(stk_t* stk, stack_value value);
stack_errors stack_creator(stk_t* stk, ssize_t capacity);
stack_errors stack_pop(stk_t* stk, stack_value* pop_value);
stack_errors widen_memory(stk_t* stk);
void stack_destructor(stk_t* stack);


#endif