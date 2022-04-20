#pragma once

struct NewCharAllocator {
    static void* allocate(int size) {
        char* ptr = new char[size];
        for(int i = 0; i < size; i++) ptr[i] = 0;
        return ptr;
    }

    static void free(void* ptr) {
        delete[] (char*)ptr;
    }
};