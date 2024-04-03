#ifndef STACK_H
#define STACK_H

#include "akinator.h"
#include "..\..\err_codes.h"

typedef struct Node* elem_t;

static const elem_t poison = NULL;

struct Stack
{
    elem_t* data = NULL;
    size_t capacity = 0;
    size_t size = 0;
};

err stack_pop(struct Stack* stk, elem_t* pop_el);

err stack_dump(struct Stack* stk, int LINE = 0, const char* stk_name = NULL, const char* file_name = NULL, const char* func_name = NULL);

err stack_ctor(struct Stack* stk, size_t capacity);

err stack_push(struct Stack* stk, const elem_t* x);

err stack_dtor(struct Stack* stk);

#endif //STACK_H
