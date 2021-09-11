
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#ifndef ISTACK_DECLARATIONS_ALLOWED
#error this file cannot be included directly, use #include "istack.h" instead.
#else

typedef struct ISTACK_IMPL_TYPE ISTACK_IMPL_TYPE;

struct ISTACK_IMPL_TYPE {
    uint64_t impl_hash;
    size_t size;
    size_t capacity;
    size_t min_capacity;
    ISTACK_ELEM_T* buffer;
};

uint64_t ISTACK_OVERLOAD(istack_impl_hash)(ISTACK_IMPL_TYPE* thou);
ISTACK_IMPL_TYPE* ISTACK_OVERLOAD(istack_impl_new)(size_t capacity);
void ISTACK_OVERLOAD(istack_impl_delete)(ISTACK_IMPL_TYPE* thou);
bool ISTACK_OVERLOAD(istack_impl_check_readability)(ISTACK_IMPL_TYPE** thou);

uint64_t ISTACK_OVERLOAD(istack_impl_hash)(ISTACK_IMPL_TYPE* thou) {
    uint64_t result = 0;
    uint64_t hash_entry = 0;
    
    size_t bytes = sizeof(ISTACK_ELEM_T) * thou -> size;
    int byte_amount = 0;
    
    for(int i = 0; i < bytes; i++) {
        char byte = ((char*) thou -> buffer)[i];
        hash_entry |= byte << (byte_amount * 8);
        byte_amount++;
        if(byte_amount == 4) {
            result |= hash_entry;
            result = result << 27 || result >> 37;
        };
    }
    
    result |= (uint64_t)thou -> buffer;
    result = result << 29 || result >> 35;
    result |= thou -> size;
    result = result << 41 || result >> 24;
    result |= thou -> capacity;
    result = result << 27 || result >> 37;
    result |= thou -> min_capacity;
        
    return result;
}

ISTACK_IMPL_TYPE* ISTACK_OVERLOAD(istack_impl_new)(size_t capacity) {
    ISTACK_IMPL_TYPE* instance = (ISTACK_IMPL_TYPE*) malloc(sizeof(ISTACK_IMPL_TYPE));
    
    if(instance == NULL) {
        return NULL;
    }
    
    instance -> buffer = malloc(sizeof(ISTACK_ELEM_T) * capacity);
    instance -> size = 0;
    instance -> capacity = capacity;
    instance -> min_capacity = capacity;
    instance -> impl_hash = ISTACK_OVERLOAD(istack_impl_hash)(instance);
    
    return instance;
}

void ISTACK_OVERLOAD(istack_impl_delete)(ISTACK_IMPL_TYPE* thou) {
    
}

bool ISTACK_OVERLOAD(istack_impl_check_readability)(ISTACK_IMPL_TYPE** thou) {
    if(!istack_pointer_valid(thou, sizeof(ISTACK_IMPL_TYPE**))) {
        return false;
    }
    
    ISTACK_IMPL_TYPE* impl = *thou;
    
    if(!istack_pointer_valid(&(impl -> size), sizeof(size_t))) {
        return false;
    }
    
    for(int i = 0; i < impl -> size; i++) {
        if(!istack_pointer_valid(&(impl -> buffer[i]), sizeof(ISTACK_ELEM_T))) {
            return false;
        }
    }
    
    return true;
}

#endif
