
#ifndef list_h
#define list_h

typedef struct mlist s_mlist;
typedef double mlist_element_type;
typedef unsigned int mlist_index;

#include "list-dumper.h"
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
    
    s_mlist_dumper* dumper;
};

s_mlist* mlist_new(mlist_index capacity);
e_mlist_error mlist_construct(s_mlist* thou, mlist_index capacity);
e_mlist_error mlist_insert(s_mlist* thou, mlist_index index, mlist_element_type element);
e_mlist_error mlist_insert_head(s_mlist* thou, mlist_element_type element);
e_mlist_error mlist_insert_tail(s_mlist* thou, mlist_element_type element);

e_mlist_error mlist_delete(s_mlist* thou, mlist_index index, mlist_element_type* target);
e_mlist_error mlist_delete_head(s_mlist* thou, mlist_element_type* target);
e_mlist_error mlist_delete_tail(s_mlist* thou, mlist_element_type* target);

e_mlist_error mlist_linear_address_search(s_mlist* thou, mlist_index index, mlist_index* address);

e_mlist_error mlist_engage_madness(s_mlist* thou);
e_mlist_error mlist_shrink_to_fit(s_mlist* thou);
e_mlist_error mlist_resize(s_mlist* thou, mlist_index size);

e_mlist_error mlist_validate(s_mlist* thou);

e_mlist_error mlist_set_dumper(s_mlist* thou, s_mlist_dumper* dumper);

void mlist_deconstruct(s_mlist* thou);
void mlist_release(s_mlist* thou);
void mlist_dump(s_mlist* thou, FILE* target);

void mlist_order_swap(s_mlist* thou, mlist_index a, mlist_index b);

#endif /* list_h */
