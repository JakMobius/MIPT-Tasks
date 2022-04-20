

#include "string/string.hpp"
#include <iostream>

static void test1() {
    String a = "123";

    assert(a.capacity() == 15);
    a.resize(100);
    assert(a.capacity() == 100);

    assert(strcmp(a.data(), "123") == 0);
    a[3] = '4';
    assert(strcmp(a.data(), "1234") == 0);
}

void string_tests() {
    test1();

    std::cout << "All tests passed (ﾉ^_^)ﾉ\n";
}
