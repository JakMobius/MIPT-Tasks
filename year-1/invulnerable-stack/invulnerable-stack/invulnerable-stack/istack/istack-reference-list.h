
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef ISTACK_DECLARATIONS_ALLOWED
#error this file cannot be included directly, use #include "istack.h" instead.
#else

istack_err_t ISTACK_OVERLOAD(istack_reference_list_create)(istack_reference_list* result);
istack_err_t ISTACK_OVERLOAD(verify_istack_list)(istack_reference_list* thou);
istack_err_t ISTACK_OVERLOAD(istack_dereference)(istack_reference_list* thou, istack_t reference, ISTACK_OVERLOAD(istack_container_t)** target);
istack_err_t ISTACK_OVERLOAD(istack_register)(istack_reference_list* thou, istack_t* reference, ISTACK_OVERLOAD(istack_container_t)* target);
istack_err_t ISTACK_OVERLOAD(istack_unregister)(istack_reference_list* thou, istack_t reference, ISTACK_OVERLOAD(istack_container_t)** target);

void ISTACK_OVERLOAD(istack_list_dump)(istack_reference_list* list) {
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf(" istack_reference_list dump:\n");
    
    ISTACK_BEGIN_POINTER_CHECKS;
    
    bool pointer_valid = ISTACK_TYPED_POINTER_VALIDITY(list, istack_reference_list*);
    
    printf(" - expected value type:  " ISTACK_ELEM_T_STRING " (%lu-byte value)\n", sizeof(ISTACK_ELEM_T));
    printf(" - address:              %p (%s)\n", list, pointer_valid ? "valid" : "invalid");
    
    if(pointer_valid) {
        istack_err_t validator_result = ISTACK_OVERLOAD(verify_istack_list)(list);
        printf(" - validator verdict:    0x%x (%s)\n", validator_result, istack_err_get_description(validator_result));
        printf(" - capacity:             ");   ISTACK_PRINT_POINTER_VALUE(&(list -> list_capacity),      "%d", int);
        printf(" - stacks stored:        ");   ISTACK_PRINT_POINTER_VALUE(&(list -> list_length),        "%d", int);
        printf(" - entry hash:           ");   ISTACK_PRINT_POINTER_VALUE((void**)&(list -> entry_hash), "%p", uint64_t);
        printf(" - list hash:            ");   ISTACK_PRINT_POINTER_VALUE((void**)&(list -> list_hash),  "%p", uint64_t);
        printf(" - index buffer:         ");   ISTACK_PRINT_DOUBLE_POINTER(&list -> istack_indices);
        printf(" - stack buffer address: ");   ISTACK_PRINT_DOUBLE_POINTER(&list -> istack_list);
        
        if(validator_result == ISTACK_OK) {
            printf(" - stacks stored: \n");
            
            for(int i = 0; i < list -> list_length; i++) {
                printf("  [%lld] = \n", list -> istack_indices[i]);
                ISTACK_OVERLOAD(istack_container_dump)(list -> istack_list[i], 2);
            }
        }
    }
    
    ISTACK_END_POINTER_CHECKS;
    
    printf("=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
}

istack_err_t ISTACK_OVERLOAD(istack_reference_list_create)(istack_reference_list* result) {
    ISTACK_BEGIN_POINTER_CHECKS;
    
    ISTACK_TYPED_POINTER_CHECK(result, istack_reference_list*);
    ISTACK_TYPED_POINTER_CHECK(&(result -> istack_list), istack_reference_list*);
    ISTACK_TYPED_POINTER_CHECK(&(result -> entry_hash), uint64_t);
    
    ISTACK_END_POINTER_CHECKS;
    
    int capacity = 64;
    
    result -> istack_list = malloc(sizeof(ISTACK_OVERLOAD(istack_container_t)*) * capacity);
    result -> istack_indices = malloc(sizeof(int) * capacity);
    result -> list_length = 0;
    result -> list_capacity = capacity;
    result -> list_hash = istack_reference_list_hash(result);
    result -> entry_hash = istack_reference_entry_hash(result);
    
    return ISTACK_OK;
}

istack_err_t ISTACK_OVERLOAD(verify_istack_list)(istack_reference_list* thou) {
        
    if(thou == NULL) {
        return ISTACK_ERR_NULL;
    }
    
    ISTACK_BEGIN_POINTER_CHECKS;
    
    ISTACK_TYPED_POINTER_CHECK(thou, istack_reference_list*);
    ISTACK_TYPED_POINTER_CHECK(&(thou -> istack_list), istack_reference_list*);
    ISTACK_TYPED_POINTER_CHECK(&(thou -> entry_hash), uint64_t);
    ISTACK_TYPED_POINTER_CHECK(thou -> istack_list, void**);
    ISTACK_TYPED_POINTER_CHECK(thou -> istack_indices, istack_t*);
    
    ISTACK_END_POINTER_CHECKS;
    
    if(thou -> list_length > thou -> list_capacity) {
        return ISTACK_CORRUPT_SIZE_GREATER_THAN_CAPACITY;
    }
    
    uint64_t hash = istack_reference_list_hash(thou);
    
    if(thou -> list_hash != hash) {
        return ISTACK_CORRUPT_DESCRIPTOR_LIST_FAILURE;
    }
    
    uint64_t entry_hash = istack_reference_entry_hash(thou);
    
    if(thou -> entry_hash != entry_hash) {
        return ISTACK_CORRUPT_DESCRIPTOR_LIST_FAILURE;
    }
    
    return ISTACK_OK;
}

istack_err_t ISTACK_OVERLOAD(istack_dereference)(istack_reference_list* thou, istack_t reference, ISTACK_OVERLOAD(istack_container_t)** target) {
    istack_err_t istack_list_error = ISTACK_OVERLOAD(verify_istack_list)(thou);
    
    if(istack_list_error != ISTACK_OK) {
        return istack_list_error;
    }
    
    for(int i = 0; i < thou -> list_length; i++) {
        if(thou -> istack_indices[i] == reference) {
            *target = thou -> istack_list[i];
            return ISTACK_OK;
        }
    }
    
    return ISTACK_ERR_NOT_EXIST;
}

istack_err_t ISTACK_OVERLOAD(istack_register)(istack_reference_list* thou, istack_t* reference, ISTACK_OVERLOAD(istack_container_t)* target) {
    istack_err_t istack_list_error = ISTACK_OVERLOAD(verify_istack_list)(thou);
    
    if(istack_list_error != ISTACK_OK) {
        return istack_list_error;
    }
    
    if(thou -> list_capacity == thou -> list_length) {
        
        thou -> list_capacity *= 2;
        
        void** istack_list_new = (void**)realloc(thou -> istack_list, thou -> list_capacity * sizeof(void*));
        istack_t* istack_indices_new = (istack_t*)realloc(thou -> istack_indices, thou -> list_capacity * sizeof(istack_t));
        
        if(istack_list_new == NULL || istack_indices_new == NULL) {
            thou -> list_capacity /= 2;
            
            if(istack_list_new != NULL) {
                istack_list_new = (void**)realloc(istack_list_new, thou -> list_capacity * sizeof(void*));
                thou -> istack_list = istack_list_new;
            }
            
            if(istack_list_new != NULL) {
                istack_indices_new = (istack_t*)realloc(istack_indices_new, thou -> list_capacity * sizeof(istack_t));
                thou -> istack_indices = istack_indices_new;
            }
            
            return ISTACK_ERR_OUT_OF_MEMORY;
        }
        
        thou -> istack_list = istack_list_new;
        thou -> istack_indices = istack_indices_new;
    }
    
    srand((unsigned int)time(0));
    
    istack_t identifier = rand();
    
    bool match_found;
    
    do {
        match_found = false;
        for(int i = 0; i < thou -> list_length; i++) {
            if(target == thou -> istack_list[i]) {
                return ISTACK_ERR_DESCRIPTOR_LIST_DUPLICATE;
            }
            
            if(identifier == thou -> istack_indices[i]) {
                identifier++;
                match_found = true;
            }
        }
    } while(match_found);
    
    *reference = identifier;
    
    thou -> istack_indices[thou -> list_length] = identifier;
    thou -> istack_list[thou -> list_length] = target;
    thou -> list_length++;
    
    thou -> list_hash = istack_reference_list_hash(thou);
    thou -> entry_hash = istack_reference_entry_hash(thou);
    
    return ISTACK_OK;
}

istack_err_t ISTACK_OVERLOAD(istack_unregister)(istack_reference_list* thou, istack_t reference, ISTACK_OVERLOAD(istack_container_t)** target) {
    istack_err_t istack_list_error = ISTACK_OVERLOAD(verify_istack_list)(thou);
    
    if(istack_list_error != ISTACK_OK) {
        return istack_list_error;
    }
    
    if(thou -> list_length == 0) {
        return ISTACK_ERR_NOT_EXIST;
    }
    
    int real_index = -1;
    
    for(int i = 0; i < thou -> list_length; i++) {
        if(reference == thou -> istack_indices[i]) {
            real_index = i;
            if(target != NULL) {
                *target = thou -> istack_list[i];
            }
            break;
        }
    }
    
    if(real_index == -1) {
        return ISTACK_ERR_NOT_EXIST;
    }
    
    for(int i = real_index + 1; i < thou -> list_length; i++) {
        thou -> istack_indices[i - 1] = thou -> istack_indices[i];
        thou -> istack_list[i - 1] = thou -> istack_list[i];
    }
    
    thou -> list_length--;
    
    thou -> istack_indices[thou -> list_length] = 0;
    thou -> istack_list[thou -> list_length] = NULL;
    
    thou -> list_hash = istack_reference_list_hash(thou);
    thou -> entry_hash = istack_reference_entry_hash(thou);
    
    return ISTACK_OK;
}

#endif
