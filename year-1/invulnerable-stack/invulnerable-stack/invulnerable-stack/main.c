
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <stdio.h>
#include <math.h>

#define ISTACK_ELEM_T double
#include "istack/istack.h"
#undef ISTACK_ELEM_T

#define ISTACK_ELEM_T int
#include "istack/istack.h"

int main(int argc, const char * argv[]) {
    istack_t stack;
    istack_t stack_b;
    
    istack_err_t err = ISTACK_OK;
    
    err = istack_request_int(&stack, 2);
    printf("0x%x\n", err);
    
    err = istack_request_int(&stack_b, 2);
    printf("0x%x\n", err);
    
    istack_push_int(stack, 1);
    istack_push_int(stack, 2);
    istack_push_int(stack, 3);
    istack_push_int(stack, 4);
    istack_push_int(stack, 5);
    istack_push_int(stack, 6);
    istack_push_int(stack, 7);
    istack_push_int(stack, 8);
    
    istack_push_int(stack_b, 0xDEADBEEF);
    istack_push_int(stack_b, 0xABADBABE);
    istack_push_int(stack_b, 0x8BADF00D);
    istack_push_int(stack_b, 0x0DEDBEDA);
    istack_push_int(stack_b, 0xBABABABA);
    istack_push_int(stack_b, 0xF000000C);
    istack_push_int(stack_b, 0xBEBEBEBE);
    istack_push_int(stack_b, 0xF7B37A32);
    
    istack_reference_list_dump_int();
    
    ISTACK_ELEM_T elem = 0;

    err = istack_pop_int(stack, &elem);
    printf("0x%x, %d\n", err, elem);

    err = istack_pop_int(stack, &elem);
    printf("0x%x, %d\n", err, elem);

    err = istack_pop_int(stack, &elem);
    printf("0x%x, %d\n", err, elem);

    err = istack_pop_int(stack, &elem);
    printf("0x%x, %d\n", err, elem);

    err = istack_pop_int(stack, &elem);
    printf("0x%x, %d\n", err, elem);

    err = istack_pop_int(stack, &elem);
    printf("0x%x, %d\n", err, elem);
    
    err = istack_pop_int(stack, &elem);
    printf("0x%x, %d\n", err, elem);
    
    err = istack_pop_int(stack, &elem);
    printf("0x%x, %d\n", err, elem);
    
    istack_destroy_int(stack);
    
    err = istack_push_int(stack, 1);
    printf("0x%x, %d\n", err, elem);
    
    istack_t stack_double;
    istack_request_double(&stack_double, 12);
    
    istack_push_double(stack, M_PI);
    istack_push_double(stack, M_E);
    istack_push_double(stack, M_PI_2);
    istack_push_double(stack, M_SQRT2);
    istack_push_double(stack, M_LN2);
    istack_push_double(stack, M_SQRT1_2);
    istack_push_double(stack, M_LN10);
    istack_push_double(stack, NAN);
    
    istack_dump_double(stack);
    
    istack_destroy_double(stack_double);
    
    return 0;
}
