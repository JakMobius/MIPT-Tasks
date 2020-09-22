
#ifndef bubblesort_h
#define bubblesort_h

#include <stdlib.h>

void bubble_sort(void* _Nonnull data, size_t n, size_t size, int (* _Nonnull comparator)(const void* _Nonnull a, const void* _Nonnull b));

#endif /* bubblesort_h */
