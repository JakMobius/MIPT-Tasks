
#ifndef BONK_COMPILER_REINVENTED_INHERIT_CPP
#define BONK_COMPILER_REINVENTED_INHERIT_CPP

template<typename A, typename B>
B inherit(const A superclass_methods, const B subclass_methods) noexcept {
    void* result_array[sizeof(B) / 8];
    void** superclass_array = (void**)(&superclass_methods);
    void** subclass_array = (void**) &subclass_methods;

    for(int i = 0; i < sizeof(A) / sizeof(void*); i++) {
        if(subclass_array[i] == nullptr) {
            result_array[i] = superclass_array[i];
        } else {
            result_array[i] = subclass_array[i];
        }
    }

    return *(B*)result_array;
}

#endif