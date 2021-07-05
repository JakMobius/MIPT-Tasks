
#ifndef BONK_COMPILER_REINVENTED_X86_REGISTER_EXTENSIONS_HPP
#define BONK_COMPILER_REINVENTED_X86_REGISTER_EXTENSIONS_HPP

namespace bonk::x86_backend {

struct register_extensions;

}

#include <cstdlib>

namespace bonk::x86_backend {

struct register_extensions {
    bool w;
    bool r;
    bool x;
    bool b;

    char get_byte();

    bool exist();
};

}

#endif //BONK_COMPILER_REINVENTED_X86_REGISTER_EXTENSIONS_HPP
