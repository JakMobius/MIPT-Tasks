
#ifndef list_h
#define list_h

typedef unsigned int auto;

#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cassert>
#include "default_alloc_construct.hpp"

enum e_mlist_error {
    MLIST_OK,
    MLIST_ERROR_LIST_NULL,
    MLIST_ERROR_INVALID_INDEX,
    MLIST_ERROR_OUT_OF_MEMORY,
    MLIST_ERROR_WRONG_SIZE,
    MLIST_ERROR_CYCLE,
    MLIST_ERROR_LOST_ELEMENTS,
    MLIST_ERROR_CORRUPTED_INDICES
};

#endif /* list_h */
