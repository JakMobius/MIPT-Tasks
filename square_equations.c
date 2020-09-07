#include <stdio.h>

typedef char byte;

void swapbytes(byte* p1, byte* p2, size_t size) {
    for(size_t p = 0; p < size; p++) {
        byte buffer = *(p1 + p);
        *(p1 + p) = *(p2 + p);
        *(p2 + p) = buffer;
    }
}

void sort(void* data, size_t n, size_t size, int (*comparator)(void* a, void* b)) {
    
    byte* bytes = (byte*) data;
    byte* p1;
    
    for(size_t i = 0; i < n; i++) {
        p1 = bytes + size * i;
        for(size_t j = 0; j < n; j++) {
            byte* p2 = bytes + size * j;
            if(comparator(p1, p2) > 0) {
                swapbytes(p1, p2, size);
            }
        }
    }
}

int comparator(void* a, void* b) {
    return *(int*)b - *(int*)a;
}

int main(int argc, const char * argv[]) {
    
    int intarray[] = {1, 4, 3, 7, 5, 4, 3};
    
    size_t length = sizeof(intarray) / sizeof(intarray[0]);
    
    sort(intarray, length, sizeof(intarray[0]), &comparator);
    
    for(int i = 0; i < length; i++) {
        printf("%d ", intarray[i]);
    }
    
    printf("\n");
    
    return 0;
}

