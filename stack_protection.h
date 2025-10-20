#ifndef PROTECT
#define PROTECT
#include "everything_about_stack.h"

int error_decoder(stack_t* stk);
stack_errors stack_err(stack_t* stk);
void print_data(stack_value* data, ssize_t capacity);
void stack_dump(stack_t* stack, const char* file , const int line, const char* func);

#endif