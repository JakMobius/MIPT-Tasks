#include <iostream>

#include "vector.hpp"
#include "chunked/chunked-storage.hpp"
#include "static-storage.hpp"

struct Vec2 {
    float x, y;
};

int main() {
//    Vector<Vec2, DynamicStorage> vec(10, {1, 1});
    Vector<Vec2, ChunkedStorage<3>::type> vec(10, {1, 1});

    int n = 0;

    for(auto& i : vec) {
        i.x = (float)n;
        n++;
    }

    vec.push_back({3, 3});
    vec.emplace_back(4.f, 4.f);

    for(auto& i : vec) {
        std::cout << i.x << "," << i.y << "\n";
    }

    Vector<bool, ChunkedStorage<16>::type> bitset(20, true);

    for(int i = 0; i < 20; i++) {
        bitset[i] = i % 2 == 0;
    }

    for(auto i : bitset) {
        std::cout << i << "\n";
    }

    Vector<Vec2, StaticStorage<10>::type> static_vector {};

    static_vector[0] = {1, 1};

    for(auto& i : static_vector) {
        std::cout << i.x << "," << i.y << "\n";
    }

    return 0;
}
