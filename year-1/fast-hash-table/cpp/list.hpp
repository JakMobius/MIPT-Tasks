
#ifndef list_h
#define list_h

typedef struct mlist s_mlist;
typedef double mlist_element_type;
typedef unsigned int mlist_index;

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

typedef enum mlist_error {
    MLIST_OK,
    MLIST_ERROR_LIST_NULL,
    MLIST_ERROR_INVALID_INDEX,
    MLIST_ERROR_OUT_OF_MEMORY,
    MLIST_ERROR_WRONG_SIZE,
    MLIST_ERROR_CYCLE,
    MLIST_ERROR_LOST_ELEMENTS,
    MLIST_ERROR_CORRUPTED_INDICES
} e_mlist_error;

extern const char* MLIST_ERROR_DESCRIPTIONS[];

struct mlist {
    mlist_element_type* element_array;
    mlist_index* prev_indices;
    mlist_index* next_indices;
    mlist_index head;
    mlist_index tail;
    mlist_index capacity;
    mlist_index size;
    mlist_index min_size;
    
    bool is_mad;
    
    e_mlist_error construct(mlist_index capacity);
    e_mlist_error insert(mlist_index index, mlist_element_type element);
    e_mlist_error insert_head(mlist_element_type element);
    e_mlist_error insert_tail(mlist_element_type element);
    e_mlist_error delete_index(mlist_index index, mlist_element_type* target);
    e_mlist_error delete_head(mlist_element_type* target);
    e_mlist_error delete_tail(mlist_element_type* target);
    e_mlist_error linear_address_search(mlist_index index, mlist_index* address);
    e_mlist_error engage_madness();
    e_mlist_error shrink_to_fit();
    e_mlist_error resize(mlist_index size);
    e_mlist_error validate();
    
    void order_swap(mlist_index a, mlist_index b);
    void deconstruct();
    void release();
};

s_mlist* mlist_new(mlist_index capacity);

void mlist_dump(FILE* target);

#endif /* list_h */
