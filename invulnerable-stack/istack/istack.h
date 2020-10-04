
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef ISTACK_ELEM_T
#error stack.h should not be included without ISTACK_ELEM_T macro defined
#else

#ifndef istack_h
#define istack_h
#define ISTACK_DECLARATIONS_ALLOWED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>
#include "singletons/istack-singletons.h"

#undef ISTACK_DECLARATIONS_ALLOWED
#endif

#define ISTACK_DECLARATIONS_ALLOWED
#include "istack-impl.h"
#include "istack-container.h"
#include "istack-reference-list.h"
#undef ISTACK_DECLARATIONS_ALLOWED

/*
 * The following code helps to hide real reference list
 * from the user, so it becomes nearly impossible to access
 * the stack memory.
 */

static istack_reference_list ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST) = {};
static bool ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST_INIT) = false;

static istack_err_t ISTACK_OVERLOAD(create_reference_list_if_neccessary)() {
    if(!ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST_INIT)) {
        istack_err_t err = ISTACK_OVERLOAD(istack_reference_list_create)(&ISTACK_REFERENCE_LIST_TYPE);
        
        if(err == ISTACK_OK) {
            ISTACK_REFERENCE_LIST_INIT_TYPE = true;
        } else {
            return err;
        }
    }
    
    return ISTACK_OK;
}

/*
 * Invulnerable stack code starts here
 */

istack_err_t ISTACK_OVERLOAD(istack_construct)(istack_t* thou);
istack_err_t ISTACK_OVERLOAD(istack_destroy)(istack_t thou);

istack_err_t ISTACK_OVERLOAD(istack_push)(istack_t thou, ISTACK_ELEM_T element);
istack_err_t ISTACK_OVERLOAD(istack_pop)(istack_t thou, ISTACK_ELEM_T* element);
istack_err_t ISTACK_OVERLOAD(istack_top)(istack_t thou, ISTACK_ELEM_T* element);
istack_err_t ISTACK_OVERLOAD(istack_impl_validate)(ISTACK_IMPL_TYPE* thou);
istack_err_t ISTACK_OVERLOAD(istack_dump)(istack_t target);

istack_err_t ISTACK_OVERLOAD(istack_push)(istack_t thou, ISTACK_ELEM_T element) {
    
    istack_err_t error = ISTACK_OVERLOAD(create_reference_list_if_neccessary)();
    
    if(error != ISTACK_OK) {
        ISTACK_HANDLE_LIST_ERROR(error, &ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST));
        return error;
    }
    
    ISTACK_OVERLOAD(istack_container_t)* stack_container;
    
    error = ISTACK_OVERLOAD(istack_dereference)(&ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST), thou, &stack_container);
    
    if(error != ISTACK_OK) {
        ISTACK_HANDLE_LIST_ERROR(error, &ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST));
        return error;
    }
    
    error = ISTACK_OVERLOAD(istack_container_push)(stack_container, element);
    
    if(error != ISTACK_OK) {
        ISTACK_HANDLE_STACK_ERROR(error, stack_container);
    }
    
    return error;
}

istack_err_t ISTACK_OVERLOAD(istack_pop)(istack_t thou, ISTACK_ELEM_T* element) {
    
    istack_err_t error = ISTACK_OVERLOAD(create_reference_list_if_neccessary)();
    
    if(error != ISTACK_OK) {
        ISTACK_HANDLE_LIST_ERROR(error, &ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST));
        return error;
    }
    
    ISTACK_OVERLOAD(istack_container_t)* stack_container;
    
    error = ISTACK_OVERLOAD(istack_dereference)(&ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST), thou, &stack_container);
    
    if(error != ISTACK_OK) {
        ISTACK_HANDLE_LIST_ERROR(error, &ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST));
        return error;
    }
    
    error = ISTACK_OVERLOAD(istack_container_pop)(stack_container, element);
    
    if(error != ISTACK_OK) {
        ISTACK_HANDLE_STACK_ERROR(error, stack_container);
    }
    
    return error;
}

istack_err_t ISTACK_OVERLOAD(istack_top)(istack_t thou, ISTACK_ELEM_T* element) {
    
    istack_err_t error = ISTACK_OVERLOAD(create_reference_list_if_neccessary)();
    
    if(error != ISTACK_OK) {
        ISTACK_HANDLE_LIST_ERROR(error, &ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST));
        return error;
    }
    
    ISTACK_OVERLOAD(istack_container_t)* stack_container;
    
    error = ISTACK_OVERLOAD(istack_dereference)(&ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST), thou, &stack_container);
    
    if(error != ISTACK_OK) {
        ISTACK_HANDLE_LIST_ERROR(error, &ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST));
        return error;
    }
    
    error = ISTACK_OVERLOAD(istack_container_pop)(stack_container, element);
    
    if(error != ISTACK_OK) {
        ISTACK_HANDLE_STACK_ERROR(error, stack_container);
    }
    
    return ISTACK_OK;
}

istack_err_t ISTACK_OVERLOAD(istack_request)(istack_t* target, size_t capacity) {
    
    if(capacity == 0) {
        return ISTACK_ERR_ZERO_CAPACITY;
    }
    
    istack_err_t error = ISTACK_OVERLOAD(create_reference_list_if_neccessary)();
    
    if(error != ISTACK_OK) {
        ISTACK_HANDLE_LIST_ERROR(error, &ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST));
        return error;
    }
    
    ISTACK_CONTAINER_TYPE* container = ISTACK_OVERLOAD(istack_container_new)(capacity);
    
    if(!container) return ISTACK_ERR_OUT_OF_MEMORY;
    
    error = ISTACK_OVERLOAD(istack_register)(&ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST), target, container);
    
    ISTACK_HANDLE_LIST_ERROR(error, &ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST));
    
    return error;
}

istack_err_t ISTACK_OVERLOAD(istack_destroy)(istack_t target) {
    
    istack_err_t error = ISTACK_OVERLOAD(create_reference_list_if_neccessary)();
    
    if(error != ISTACK_OK) {
        ISTACK_OVERLOAD(istack_list_dump)(&ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST));
        return error;
    }
    
    ISTACK_CONTAINER_TYPE* container = NULL;
    error = ISTACK_OVERLOAD(istack_unregister)(&ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST), target, &container);
    
    if(error != ISTACK_OK) {
        ISTACK_HANDLE_LIST_ERROR(error, &ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST));
        return error;
    }
    
    ISTACK_OVERLOAD(istack_container_delete)(container);
    
    return ISTACK_OK;
}

istack_err_t ISTACK_OVERLOAD(istack_dump)(istack_t target) {

    istack_err_t error = ISTACK_OVERLOAD(create_reference_list_if_neccessary)();
    
    if(error != ISTACK_OK) {
        ISTACK_OVERLOAD(istack_list_dump)(&ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST));
        return error;
    }
    
    ISTACK_CONTAINER_TYPE* container = NULL;
    error = ISTACK_OVERLOAD(istack_unregister)(&ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST), target, &container);
    
    if(error != ISTACK_OK) {
        ISTACK_HANDLE_LIST_ERROR(error, &ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST));
        return error;
    }
    
    ISTACK_OVERLOAD(istack_container_dump)(container, 0);
    
    return ISTACK_OK;
}

istack_err_t ISTACK_OVERLOAD(istack_reference_list_dump)() {

    istack_err_t error = ISTACK_OVERLOAD(create_reference_list_if_neccessary)();
    
    if(error != ISTACK_OK) {
        ISTACK_OVERLOAD(istack_list_dump)(&ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST));
        return error;
    }
    
    ISTACK_OVERLOAD(istack_list_dump)(&ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST));
    
    return ISTACK_OK;
}

#endif
