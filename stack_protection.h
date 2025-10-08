#ifndef PROTECT
#define PROTECT
#include "everything_about_stack.h"

int error_decoder(stack* stk);
stack_errors stack_err(stack* stk);
void print_data(stack_value* data, ssize_t capacity);
void stack_dump(stack* stack, const char* file , const int line );

#endif