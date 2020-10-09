
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

// TODO: Recover from ISTACK_CLONE_INCONSISTENCY state
// TODO: Print real value with user-defined ISTACK_PRINT_VALUE macro
// TODO: Canary?
// TODO: Use strong hashing algorithm
// TODO: Windows port

#ifndef ISTACK_ELEM_T
#error stack.h should not be included without ISTACK_ELEM_T macro defined
#else

#ifndef ISTACK_VALIDATION_LEVEL
#define ISTACK_VALIDATION_LEVEL 9
#endif

#if (3 < ISTACK_VALIDATION_LEVEL + 0)
#ifndef ISTACK_IMPL_AMOUNT
#define ISTACK_IMPL_AMOUNT 1000
#endif
#endif

#if (2 < ISTACK_VALIDATION_LEVEL + 0)
#ifndef ISTACK_IMPL_AMOUNT
#define ISTACK_IMPL_AMOUNT 100
#endif
#define ISTACK_USE_CROSSCHECK
#endif

#if (1 < ISTACK_VALIDATION_LEVEL + 0)
#ifndef ISTACK_IMPL_AMOUNT
#define ISTACK_IMPL_AMOUNT 10
#endif
#define ISTACK_USE_POSION_CHECK
#endif

#if (0 < ISTACK_VALIDATION_LEVEL + 0)
#define ISTACK_USE_HASH
#define ISTACK_CHECK_MEMORY
#endif

#ifndef ISTACK_IMPL_AMOUNT
#define ISTACK_IMPL_AMOUNT 1
#endif

#ifdef ISTACK_CHECK_MEMORY

#define ISTACK_BEGIN_POINTER_CHECKS istack_setup_signal_handling()
#define ISTACK_TYPED_POINTER_CHECK(pointer, type) \
    if(!istack_pointer_valid(pointer, sizeof(type))) {\
        istack_restore_signal_handling(); \
        return ISTACK_CORRUPT_INVALID_POINTER; \
    }
#define ISTACK_TYPED_POINTER_VALIDITY(pointer, type) istack_pointer_valid(pointer, sizeof(type))
#define ISTACK_END_POINTER_CHECKS istack_restore_signal_handling()

#else
#define ISTACK_BEGIN_POINTER_CHECKS
#define ISTACK_TYPED_POINTER_CHECK(pointer, type) true
#define ISTACK_TYPED_POINTER_VALIDITY(pointer, type) true
#define ISTACK_END_POINTER_CHECKS
#endif

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
    
    error = ISTACK_OVERLOAD(istack_container_top)(stack_container, element);
    
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

#undef ISTACK_USE_HASH
#undef ISTACK_USE_CROSSCHECK
#undef ISTACK_USE_POSION_CHECK
#undef ISTACK_BEGIN_POINTER_CHECKS
#undef ISTACK_TYPED_POINTER_CHECK
#undef ISTACK_TYPED_POINTER_CHECK
#undef ISTACK_END_POINTER_CHECKS

#endif
