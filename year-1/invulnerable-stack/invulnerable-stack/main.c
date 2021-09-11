
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <stdio.h>
#include <math.h>

#define ISTACK_VALIDATION_LEVEL 4
#define ISTACK_ELEM_T int
#include "istack/istack.h"
//#undef ISTACK_ELEM_T
#undef ISTACK_VALIDATION_LEVEL

int main(int argc, const char * argv[]) {
    istack_t stack = {0};
    ISTACK_OVERLOAD(istack_request)(&stack, 2);
    
    ISTACK_OVERLOAD(istack_push)(stack, 4);
    
    //istack_dump_double(stack);
    
    ISTACK_OVERLOAD(istack_reference_list_dump)();
    
    // ниже живёт утка
    
    //    istack_container_t_double* container;
    //    istack_dereference_double(&ISTACK_REFERENCE_LIST_double, stack, &container);
    //
    //    double a;
    //
    //    istack_push_double(stack, 10);
    //    istack_push_double(stack, 11);
    //    istack_push_double(stack, 12);
    //    istack_push_double(stack, 13);
    //    istack_push_double(stack, 14);
    //    istack_push_double(stack, 15);
    //    istack_pop_double(stack, &a);
    //
    
    // *кряк*
    
    //    for(int i = 0; i < ISTACK_IMPL_AMOUNT / 2; i++) {
    //        container -> impl_list[i] -> size = 2;
    //    }
    
    return 0;
}
