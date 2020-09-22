
#include "bubblesort.h"

static void swapchars(char* p1, char* p2, size_t size) {
    for(size_t p = 0; p < size; p++) {
        char buffer = *(p1 + p);
        *(p1 + p) = *(p2 + p);
        *(p2 + p) = buffer;
    }
}

void bubble_sort(void* _Nonnull data, size_t n, size_t size, int (* _Nonnull comparator)(const void* _Nonnull a, const void* _Nonnull b)) {
    
    char* bytes = (char*) data;
    
    for (size_t i = 0; i < n-1; i++) {
        for (size_t j = 0; j < n - i - 1; j++) {
            char* p1 = bytes + size * j;
            char* p2 = p1 + size;
            if(comparator(p1, p2) > 0) {
                swapchars(p1, p2, size);
            }
        }
    }
}
