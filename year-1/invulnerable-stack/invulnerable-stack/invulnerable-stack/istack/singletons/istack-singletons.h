
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef istack_singletons_h
#ifndef ISTACK_DECLARATIONS_ALLOWED
#error this file cannot be included directly, use #include "istack.h" instead.
#else

#define istack_singletons_h

#include "istack-pointer-check.h"

#define ISTACK_OVERLOAD_TYPE(name, type) name##_##type
#define ISTACK_OVERLOAD_TYPE_WRAP(name, type) ISTACK_OVERLOAD_TYPE(name, type)
#define ISTACK_OVERLOAD(name) ISTACK_OVERLOAD_TYPE_WRAP(name, ISTACK_ELEM_T)

#define ISTACK_IMPL_TYPE ISTACK_OVERLOAD(istack_impl_t)
#define ISTACK_CONTAINER_TYPE ISTACK_OVERLOAD(istack_container_t)

#define ISTACK_REFERENCE_LIST_TYPE ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST)
#define ISTACK_REFERENCE_LIST_INIT_TYPE ISTACK_OVERLOAD(ISTACK_REFERENCE_LIST_INIT)

#define ISTACK_BEGIN_POINTER_CHECKS istack_setup_signal_handling()
#define ISTACK_TYPED_POINTER_CHECK(pointer, type) \
    if(!istack_pointer_valid(pointer, sizeof(type))) {\
        istack_restore_signal_handling(); \
        return ISTACK_CORRUPT_INVALID_POINTER; \
    }
#define ISTACK_TYPED_POINTER_VALIDITY(pointer, type) istack_pointer_valid(pointer, sizeof(type))
#define ISTACK_END_POINTER_CHECKS istack_restore_signal_handling()

#define ISTACK_PRINT_POINTER_VALUE(pointer, format, type)\
    if(ISTACK_TYPED_POINTER_VALIDITY(pointer, type)) {\
        printf(format "\n", *pointer);\
    } else {\
        printf("failed to read memory at %p\n", pointer);\
    }
#define ISTACK_PRINT_DOUBLE_POINTER(pointer) \
    if(ISTACK_TYPED_POINTER_VALIDITY(pointer, void**)) {\
        printf("%p (%s)\n", *pointer, ISTACK_TYPED_POINTER_VALIDITY(*pointer, void*) ? "valid" : "invalid");\
    } else {\
        printf("failed to read memory at %p\n", pointer);\
    }

#define ISTACK_ELEM_T_STRINGIFY(name) #name
#define ISTACK_ELEM_T_TO_STRING(name) ISTACK_ELEM_T_STRINGIFY(name)
#define ISTACK_ELEM_T_STRING ISTACK_ELEM_T_TO_STRING(ISTACK_ELEM_T)

#define ISTACK_HANDLE_LIST_ERROR(error, list)\
    if(istack_err_is_corruption(error)) {\
        ISTACK_OVERLOAD(istack_list_dump)(list);\
    }
#define ISTACK_HANDLE_STACK_ERROR(error, container)\
    if(istack_err_is_corruption(error)) {\
        ISTACK_OVERLOAD(istack_container_dump)(container, 0);\
    }

/*
 Begin of non-duplicating declarations
 */

typedef long long istack_t;
const size_t ISTACK_IMPL_AMOUNT = 10000;
const size_t ISTACK_MAX_DISPLAYABLE_STACK_ITEMS = 20;

typedef enum istack_err_t istack_err_t;
typedef struct istack_reference_list istack_reference_list;

enum istack_err_t {
    ISTACK_OK                                 = 0x00,
    ISTACK_ERR_OUT_OF_MEMORY                  = 0x01,
    ISTACK_ERR_NULL                           = 0x02,
    ISTACK_ERR_STACK_UNDERFLOW                = 0x03,
    ISTACK_ERR_NOT_EXIST                      = 0x04,
    ISTACK_ERR_DESCRIPTOR_LIST_DUPLICATE      = 0x05,
    ISTACK_CORRUPT_HASH_CHECK_FAILED          = 0x10,
    ISTACK_CORRUPT_POISON_LEAK                = 0x20,
    ISTACK_CORRUPT_SIZE_GREATER_THAN_CAPACITY = 0x30,
    ISTACK_CORRUPT_CLONE_INCONSISTENCY        = 0x40,
    ISTACK_CORRUPT_DESCRIPTOR_LIST_FAILURE    = 0x50,
    ISTACK_CORRUPT_INVALID_POINTER            = 0x60
};

const char* ISTACK_ERR_T_DESCRIPTIONS[] = {
    "ISTACK_OK",
    "ISTACK_ERR_OUT_OF_MEMORY",
    "ISTACK_ERR_NULL",
    "ISTACK_ERR_STACK_UNDERFLOW",
    "ISTACK_ERR_NOT_EXIST",
    "ISTACK_ERR_DESCRIPTOR_LIST_DUPLICATE",
    "ISTACK_CORRUPT_HASH_CHECK_FAILED",
    "ISTACK_CORRUPT_POISON_LEAK",
    "ISTACK_CORRUPT_SIZE_GREATER_THAN_CAPACITY",
    "ISTACK_CORRUPT_CLONE_INCONSISTENCY",
    "ISTACK_CORRUPT_DESCRIPTOR_LIST_FAILURE",
    "ISTACK_CORRUPT_INVALID_POINTER"
};

const char* istack_err_get_description(istack_err_t err) {
    if(err == ISTACK_OK) return ISTACK_ERR_T_DESCRIPTIONS[0];
    if(err == ISTACK_ERR_OUT_OF_MEMORY) return ISTACK_ERR_T_DESCRIPTIONS[1];
    if(err == ISTACK_ERR_NULL) return ISTACK_ERR_T_DESCRIPTIONS[2];
    if(err == ISTACK_ERR_STACK_UNDERFLOW) return ISTACK_ERR_T_DESCRIPTIONS[3];
    if(err == ISTACK_ERR_NOT_EXIST) return ISTACK_ERR_T_DESCRIPTIONS[4];
    if(err == ISTACK_ERR_DESCRIPTOR_LIST_DUPLICATE) return ISTACK_ERR_T_DESCRIPTIONS[5];
    if(err == ISTACK_CORRUPT_HASH_CHECK_FAILED) return ISTACK_ERR_T_DESCRIPTIONS[6];
    if(err == ISTACK_CORRUPT_POISON_LEAK) return ISTACK_ERR_T_DESCRIPTIONS[7];
    if(err == ISTACK_CORRUPT_SIZE_GREATER_THAN_CAPACITY) return ISTACK_ERR_T_DESCRIPTIONS[8];
    if(err == ISTACK_CORRUPT_CLONE_INCONSISTENCY) return ISTACK_ERR_T_DESCRIPTIONS[9];
    if(err == ISTACK_CORRUPT_DESCRIPTOR_LIST_FAILURE) return ISTACK_ERR_T_DESCRIPTIONS[10];
    if(err == ISTACK_CORRUPT_INVALID_POINTER) return ISTACK_ERR_T_DESCRIPTIONS[11];
    return "((null))";
}

static uint64_t istack_pointer_hash(void* pointers, size_t amount) {
    uint64_t result = 0;
    uint64_t shift = 3;
    
    for(int i = 0; i < amount; i++) {
        result ^= *((uint64_t*)pointers + i);
        result = (result << shift) | (result >> (sizeof(uint64_t) * 8 - shift));
    }
    
    return result;
}

struct istack_reference_list {
    void** istack_list;
    istack_t* istack_indices;
    int list_length;
    int list_capacity;
    uint64_t list_hash;
    uint64_t entry_hash;
};

_Bool istack_err_is_failure(istack_err_t err) {
    return err & 0x0F;
}

_Bool istack_err_is_corruption(istack_err_t err) {
    return err & 0xF0;
}

static uint64_t istack_reference_list_hash(istack_reference_list* list) {
    uint64_t result = 0xCAFECAFEBABEBABE;
    uint64_t istack_list_hash = istack_pointer_hash(&list -> istack_list, 1);
    uint64_t istack_indices_hash = istack_pointer_hash(&list -> istack_list, 1);
    
    istack_list_hash = (istack_list_hash << 4) | (istack_list_hash >> 60);
    istack_indices_hash = (istack_indices_hash << 10) | (istack_indices_hash >> 54);
    result ^= istack_list_hash;
    result ^= istack_indices_hash;
    result ^= (0xCAFEBABE - list -> list_length);
    
    result = result << 13 | result >> 51;
    
    result ^= (0xDEADBEEF - list -> list_capacity);
    
    return result;
}

static uint64_t istack_reference_entry_hash(istack_reference_list* list) {
    uint64_t result = 0xABADBABE8BADF00D;
    uint64_t istack_list_hash = istack_pointer_hash(list -> istack_list, list -> list_length);
    uint64_t istack_indices_hash = 0xFACEFEED;
    
    for(int i = 0; i < list -> list_length; i++) {
        istack_indices_hash ^= (uint64_t) list -> istack_indices[i];
        istack_indices_hash = istack_indices_hash << 15 | istack_indices_hash >> 49;
    }
    
    istack_list_hash = (istack_list_hash << 7) | (istack_list_hash >> 57);
    istack_indices_hash = (istack_indices_hash << 17) | (istack_indices_hash >> 47);
    result ^= istack_list_hash;
    result ^= istack_indices_hash;
    
    return result;
}

void istack_print_padding(int length) {
    while(length--) putchar(' ');
}

#endif
#endif
