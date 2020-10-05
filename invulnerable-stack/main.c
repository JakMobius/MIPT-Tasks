
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <stdio.h>
#include <math.h>

#define ISTACK_VALIDATION_LEVEL 0
#define ISTACK_ELEM_T double
#include "istack/istack.h"
#undef ISTACK_ELEM_T

#define ISTACK_ELEM_T int
#include "istack/istack.h"

int main(int argc, const char * argv[]) {
    istack_t stack;
    istack_request_double(&stack, 4);
    
    istack_container_t_double* container;
    istack_dereference_double(&ISTACK_REFERENCE_LIST_double, stack, &container);
    
    double a;
    
    istack_push_double(stack, 12);
    istack_push_double(stack, 12);
    istack_pop_double(stack, &a);
    
//    for(int i = 0; i < ISTACK_IMPL_AMOUNT; i++) {
//        container -> impl_list[i] -> size = 2;
//    }
    
    istack_dump_double(stack);
    return 0;
}
