#ifndef PROTECT
#define PROTECT
#include "everything_about_stack.h"

int error_decoder(stk_t* stk);
stack_errors stack_err(stk_t* stk);
void print_data(stack_value* data, ssize_t capacity);
void stack_dump(stk_t* stack, const char* file , const int line, const char* func);

#endif