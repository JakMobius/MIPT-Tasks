
#include "x86_register_extensions.hpp"

namespace bonk::x86_backend {

char register_extensions::get_byte() {
    char result = 0b01000000;
    result |= w << 3;
    result |= r << 2;
    result |= x << 1;
    result |= b << 0;
    return result;
}

bool register_extensions::exist() {
    return w || r || x || b;
}
}