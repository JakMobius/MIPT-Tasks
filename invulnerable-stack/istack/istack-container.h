
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef ISTACK_DECLARATIONS_ALLOWED
#error this file cannot be included directly, use #include "istack.h" instead.
#else

typedef struct ISTACK_CONTAINER_TYPE ISTACK_CONTAINER_TYPE;

struct ISTACK_CONTAINER_TYPE {
    uint64_t impl_pointer_hash;
    ISTACK_IMPL_TYPE** impl_list;
};

ISTACK_CONTAINER_TYPE* ISTACK_OVERLOAD(istack_container_new)(size_t capacity);
istack_err_t ISTACK_OVERLOAD(istack_container_construct)(ISTACK_CONTAINER_TYPE* thou, size_t capacity);
void ISTACK_OVERLOAD(istack_container_destruct)(ISTACK_CONTAINER_TYPE* thou);
void ISTACK_OVERLOAD(istack_container_delete)(ISTACK_CONTAINER_TYPE* thou);
void ISTACK_OVERLOAD(istack_container_dump)(ISTACK_CONTAINER_TYPE* type, int deep);
istack_err_t ISTACK_OVERLOAD(istack_container_validate)(ISTACK_CONTAINER_TYPE* thou);

istack_err_t ISTACK_OVERLOAD(istack_container_push)(ISTACK_CONTAINER_TYPE* thou, ISTACK_ELEM_T value);
istack_err_t ISTACK_OVERLOAD(istack_container_pop)(ISTACK_CONTAINER_TYPE* thou, ISTACK_ELEM_T* value_target);
istack_err_t ISTACK_OVERLOAD(istack_container_top)(ISTACK_CONTAINER_TYPE* thou, ISTACK_ELEM_T* value_target);

void ISTACK_OVERLOAD(istack_container_dump)(ISTACK_CONTAINER_TYPE* thou, int deep) {
    if(deep == 0) printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    
    ISTACK_BEGIN_POINTER_CHECKS;
    
    bool pointer_valid = ISTACK_TYPED_POINTER_VALIDITY(thou, istack_reference_list*);
    istack_print_padding(deep); printf(" istack_container_t dump\n");
    istack_print_padding(deep); printf(" - expected value type:    " ISTACK_ELEM_T_STRING " (%lu-byte value)\n", sizeof(ISTACK_ELEM_T));
    istack_print_padding(deep); printf(" - address:                %p (%s)\n", thou, pointer_valid ? "valid" : "invalid");
    
    if(pointer_valid) {
        istack_err_t validator_result = ISTACK_OVERLOAD(istack_container_validate)(thou);
        istack_print_padding(deep); printf(" - validator verdict:      0x%x (%s)\n", validator_result, istack_err_get_description(validator_result));
        istack_print_padding(deep); printf(" - entry pointer hash:     "); ISTACK_PRINT_POINTER_VALUE((void**)&(thou -> impl_pointer_hash), "%p", uint64_t);
        istack_print_padding(deep); printf(" - stack amount:           %zu\n", ISTACK_IMPL_AMOUNT);
        
        bool impl_pointer_valid = ISTACK_TYPED_POINTER_VALIDITY(thou -> impl_list, ISTACK_IMPL_TYPE**);
        int failed_stacks = 0;
        size_t stack_length = 0;
        size_t stack_capacity = 0;
        size_t stack_min_capacity = 0;
        bool ambiguous_сapacity = false;
        bool ambiguous_size = false;
        bool ambiguous_min_capacity = false;
        
        ISTACK_IMPL_TYPE* first_stack = NULL;
        
        if(impl_pointer_valid) {
            for(int i = 0; i < ISTACK_IMPL_AMOUNT; i++) {
                ISTACK_IMPL_TYPE** each_stack = &(thou -> impl_list[i]);
                
                if(ISTACK_OVERLOAD(istack_impl_check_readability)(each_stack)) {
                    if(!first_stack) {
                        first_stack = *each_stack;
                    } else {
                        if((*each_stack) -> capacity != first_stack -> capacity) ambiguous_сapacity = true;
                        if((*each_stack) -> size != first_stack -> size) ambiguous_size = true;
                        if((*each_stack) -> min_capacity != first_stack -> min_capacity) ambiguous_min_capacity = true;
                    }
                    
                    if(stack_length < first_stack -> size) {
                        stack_length = first_stack -> size;
                    }
                    
                    if(stack_capacity < first_stack -> capacity) {
                        stack_capacity = first_stack -> capacity;
                    }
                    
                    if(stack_min_capacity < first_stack -> min_capacity) {
                        stack_min_capacity = first_stack -> min_capacity;
                    }
                } else {
                    failed_stacks++;
                }
            }
        }
        
        istack_print_padding(deep);
        printf(" - stack capacity:         ");
        
        if(!impl_pointer_valid || !first_stack) printf("unknown\n");
        else if(ambiguous_сapacity) printf("ambiguous (less or equal %zu)\n", stack_capacity);
        else printf("%zu\n", first_stack -> capacity);
        
        istack_print_padding(deep);
        printf(" - stack length:           ");
        
        if(!impl_pointer_valid || !first_stack) printf("unknown\n");
        else if(ambiguous_сapacity) printf("ambiguous (less or equal %zu)\n", stack_length);
        else printf("%zu\n", first_stack -> capacity);
        
        istack_print_padding(deep);
        printf(" - stack initial capacity: ");
        
        if(!impl_pointer_valid || !first_stack) printf("unknown\n");
        else if(ambiguous_сapacity) printf("ambiguous (less or equal %zu)\n", stack_min_capacity);
        else printf("%zu\n", first_stack -> capacity);
        
        istack_print_padding(deep);
        printf(" - corrupted stacks:       %d\n", failed_stacks);
        
        istack_print_padding(deep);
        if(stack_length > ISTACK_MAX_DISPLAYABLE_STACK_ITEMS) {
            stack_length = ISTACK_MAX_DISPLAYABLE_STACK_ITEMS;
            printf(" - first %zu stack items:\n", ISTACK_MAX_DISPLAYABLE_STACK_ITEMS);
        } else {
            printf(" - stack items:\n");
        }
        
        for(int i = 0; i < stack_length; i++) {
            ISTACK_ELEM_T* element = NULL;
            
            bool ambiguous_element = false;
            
            for(int stack_index = 0; stack_index < ISTACK_IMPL_AMOUNT; stack_index++) {
                ISTACK_IMPL_TYPE** each_stack = &(thou -> impl_list[stack_index]);
                
                if(!istack_pointer_valid(each_stack, sizeof(ISTACK_IMPL_TYPE*))) {
                    continue;
                }
                if(!istack_pointer_valid(&((*each_stack) -> buffer[i]), sizeof(ISTACK_ELEM_T))) {
                    continue;
                }
                
                ISTACK_ELEM_T* buffer = &((*each_stack) -> buffer[i]);
                
                if(element != NULL) {
                    if(!ambiguous_element) {
                        for(int byte = 0; byte < sizeof(ISTACK_ELEM_T); byte++) {
                            if(((char*)buffer)[byte] != ((char*)element)[byte]) {
                                ambiguous_element = true;
                            }
                        }
                    }
                } else {
                    element = buffer;
                }
            }
            
            istack_print_padding(deep);
            printf("   [%d] = ", i);
            
            if(ambiguous_element) printf("ambiguous\n");
            else if(element == NULL) printf("unknown\n");
            else {
                printf("0x");
                for(int byte = sizeof(ISTACK_ELEM_T) - 1; byte >= 0; byte--) {
                    char value = ((char*)element)[byte];
                    
                    printf("%0*x", 2, (unsigned char)value);
                }
                printf("\n");
            }
        }
        
    }
    
    ISTACK_END_POINTER_CHECKS;
    
    if(deep == 0) printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
}

ISTACK_CONTAINER_TYPE* ISTACK_OVERLOAD(istack_container_new)(size_t capacity) {
    ISTACK_CONTAINER_TYPE* instance = calloc(1, sizeof(ISTACK_CONTAINER_TYPE));
    if(!instance) {
        return NULL;
    }
    
    istack_err_t error = ISTACK_OVERLOAD(istack_container_construct)(instance, capacity);
    
    if(error == ISTACK_ERR_OUT_OF_MEMORY) {
        free(instance);
        return NULL;
    }
    
    return instance;
}

istack_err_t ISTACK_OVERLOAD(istack_container_construct)(ISTACK_CONTAINER_TYPE* thou, size_t capacity) {
    
    thou -> impl_list = (ISTACK_IMPL_TYPE**)malloc(sizeof(ISTACK_IMPL_TYPE*) * ISTACK_IMPL_AMOUNT);
    
    if(!thou -> impl_list) {
        return ISTACK_ERR_OUT_OF_MEMORY;
    }
    
    for(int i = 0; i < ISTACK_IMPL_AMOUNT; i++) {
        ISTACK_IMPL_TYPE* impl = ISTACK_OVERLOAD(istack_impl_new)(capacity);
        
        if(!impl) {
            for(int j = 0; j < i; j++) {
                ISTACK_OVERLOAD(istack_impl_delete)(impl);
            }
            
            free(thou -> impl_list);
            
            return ISTACK_ERR_OUT_OF_MEMORY;
        }
        
        thou -> impl_list[i] = impl;
    }
    
    thou -> impl_pointer_hash = istack_pointer_hash(thou -> impl_list, ISTACK_IMPL_AMOUNT);
    
    return ISTACK_OK;
}

void ISTACK_OVERLOAD(istack_container_destruct)(ISTACK_CONTAINER_TYPE* thou) {
    for(int i = 0; i < ISTACK_IMPL_AMOUNT; i++) {
        ISTACK_OVERLOAD(istack_impl_delete)(thou -> impl_list[i]);
    }
    
    free(thou -> impl_list);
}

void ISTACK_OVERLOAD(istack_container_delete)(ISTACK_CONTAINER_TYPE* thou) {
    ISTACK_OVERLOAD(istack_container_destruct)(thou);
    free(thou);
}

istack_err_t ISTACK_OVERLOAD(istack_container_validate)(ISTACK_CONTAINER_TYPE* thou) {
    if(thou == NULL) {
        return ISTACK_ERR_NULL;
    }
    
    ISTACK_BEGIN_POINTER_CHECKS;
    
    ISTACK_TYPED_POINTER_CHECK(thou, istack_reference_list*);
    ISTACK_TYPED_POINTER_CHECK(&(thou -> impl_pointer_hash), uint64_t);
    
    for(int i = 0; i < ISTACK_IMPL_AMOUNT; i++) {
        if(!ISTACK_OVERLOAD(istack_impl_check_readability)(&(thou -> impl_list[i]))) {
            ISTACK_END_POINTER_CHECKS;
            return ISTACK_CORRUPT_INVALID_POINTER;
        }
    }
    
    ISTACK_IMPL_TYPE* first_stack = thou -> impl_list[0];
    size_t stack_capacity = first_stack -> capacity;
    size_t stack_size = first_stack -> size;
    size_t stack_min_capacity = first_stack -> min_capacity;
    uint64_t stack_hash = first_stack -> impl_hash;
    
    for(int i = 0; i < ISTACK_IMPL_AMOUNT; i++) {
        ISTACK_IMPL_TYPE* stack = thou -> impl_list[i];
        
        if(stack -> size != stack_size) return ISTACK_CORRUPT_CLONE_INCONSISTENCY;
        if(stack -> capacity != stack_capacity) return ISTACK_CORRUPT_CLONE_INCONSISTENCY;
        if(stack -> impl_hash != stack_hash) return ISTACK_CORRUPT_CLONE_INCONSISTENCY;
        if(stack -> min_capacity != stack_min_capacity) return ISTACK_CORRUPT_CLONE_INCONSISTENCY;
        
        for(int item_index = 0; item_index < stack_capacity; item_index++) {
            char* pointer_b = (char*)&stack -> buffer[item_index];
            
            if(!ISTACK_TYPED_POINTER_VALIDITY(pointer_b, ISTACK_ELEM_T*)) {
                return ISTACK_CORRUPT_INVALID_POINTER;
            }
            
            if(item_index < stack_size) {
                char* pointer_a = (char*)&first_stack -> buffer[item_index];
                
                for(int byte_index = 0; byte_index < sizeof(ISTACK_ELEM_T); byte_index++) {
                    if(pointer_a[byte_index] != pointer_b[byte_index]) {
                        return ISTACK_CORRUPT_CLONE_INCONSISTENCY;
                    }
                }
            }
        }
    }
    
    if(istack_pointer_hash(thou -> impl_list, ISTACK_IMPL_AMOUNT) != thou -> impl_pointer_hash) {
        return ISTACK_CORRUPT_HASH_CHECK_FAILED;
    }
    
    if(stack_hash != ISTACK_OVERLOAD(istack_impl_hash)(first_stack)) {
        return ISTACK_CORRUPT_HASH_CHECK_FAILED;
    }
    
    if(stack_size > stack_capacity) {
        return ISTACK_CORRUPT_SIZE_GREATER_THAN_CAPACITY;
    }
    
    ISTACK_END_POINTER_CHECKS;
    
    return ISTACK_OK;
}

istack_err_t ISTACK_OVERLOAD(istack_container_reallocate)(ISTACK_CONTAINER_TYPE* thou, size_t new_capacity) {
    
    
    for(int i = 0; i < ISTACK_IMPL_AMOUNT; i++) {
        ISTACK_IMPL_TYPE* impl = thou -> impl_list[i];
        size_t old_capacity = impl -> capacity;
        impl -> capacity = new_capacity;
        
        if(new_capacity == old_capacity) {
            break;
        }
        
        ISTACK_ELEM_T* reallocated = realloc(impl -> buffer, impl -> capacity * sizeof(ISTACK_ELEM_T));
        
        if(new_capacity > old_capacity && reallocated == NULL) {
            
            // realloc cannot return NULL when block size decreases,
            
            for(int j = 0; j < i; j++) {
                ISTACK_IMPL_TYPE* previous = thou -> impl_list[j];
                previous -> capacity = old_capacity;
                previous -> buffer = realloc(previous -> buffer, previous -> capacity * sizeof(ISTACK_ELEM_T));
            }
            
            return ISTACK_ERR_OUT_OF_MEMORY;
        }
        
        impl -> buffer = reallocated;
    }
    
    return ISTACK_OK;
}

istack_err_t ISTACK_OVERLOAD(istack_container_push)(ISTACK_CONTAINER_TYPE* thou, ISTACK_ELEM_T value) {
    
    istack_err_t error = ISTACK_OVERLOAD(istack_container_validate)(thou);
    
    if(error != ISTACK_OK) {
        return error;
    }
    
    ISTACK_IMPL_TYPE* first_stack = *thou -> impl_list;
    if(first_stack -> size == first_stack -> capacity) {
        error = ISTACK_OVERLOAD(istack_container_reallocate)(thou, first_stack -> capacity * 2);
        if(error != ISTACK_OK) {
            return error;
        }
    }
    
    for(int i = 0; i < ISTACK_IMPL_AMOUNT; i++) {
        ISTACK_IMPL_TYPE* impl = thou -> impl_list[i];
        
        impl -> buffer[impl -> size++] = value;
    }
    
    first_stack -> impl_hash = ISTACK_OVERLOAD(istack_impl_hash)(first_stack);
    
    for(int i = 1; i < ISTACK_IMPL_AMOUNT; i++) {
        thou -> impl_list[i] -> impl_hash = first_stack -> impl_hash;
    }
    
    return ISTACK_OK;
}

istack_err_t ISTACK_OVERLOAD(istack_container_pop)(ISTACK_CONTAINER_TYPE* thou, ISTACK_ELEM_T* value_target) {
    
    istack_err_t error = ISTACK_OVERLOAD(istack_container_validate)(thou);
    
    if(error != ISTACK_OK) {
        return error;
    }
    
    ISTACK_IMPL_TYPE* first_stack = *thou -> impl_list;
    
    if(first_stack -> size == 0) return ISTACK_ERR_STACK_UNDERFLOW;
    
    if(first_stack -> size < first_stack -> capacity / 2 && first_stack -> capacity > first_stack -> min_capacity) {
        ISTACK_OVERLOAD(istack_container_reallocate)(thou, first_stack -> capacity / 2);
    }
    
    *value_target = first_stack -> buffer[first_stack -> size - 1];
    
    for(int i = 0; i < ISTACK_IMPL_AMOUNT; i++) {
        thou -> impl_list[i] -> size--;
    }
    
    return ISTACK_OK;
}

istack_err_t ISTACK_OVERLOAD(istack_container_top)(ISTACK_CONTAINER_TYPE* thou, ISTACK_ELEM_T* value_target) {
    
    istack_err_t error = ISTACK_OVERLOAD(istack_container_validate)(thou);
    
    if(error != ISTACK_OK) {
        return error;
    }
    
    ISTACK_IMPL_TYPE* first_stack = *thou -> impl_list;
    *value_target = first_stack -> buffer[first_stack -> size];
    
    return ISTACK_OK;
}

#endif
