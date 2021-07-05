//
// Created by Темыч on 20.04.2021.
//

#ifndef BONK_COMPILER_REINVENTED_DEFAULT_ALLOC_CONSTRUCT_HPP
#define BONK_COMPILER_REINVENTED_DEFAULT_ALLOC_CONSTRUCT_HPP

#define DEFAULT_ALLOC_CONSTRUCT(type, ...){   \
    auto* node = (type*)calloc(1, sizeof(type)); \
    if(!node) return nullptr;           \
    node->construct(__VA_ARGS__);     \
    return node;                        \
}

#define CHECK_ALLOC_CONSTRUCT(type, ...){   \
    auto* node = (type*)calloc(1, sizeof(type)); \
    if(!node) return nullptr;           \
    if(!node->construct(__VA_ARGS__)) { \
        node->destruct();               \
        return nullptr;                 \
    }                                   \
    return node;                        \
}

#endif //BONK_COMPILER_REINVENTED_DEFAULT_ALLOC_CONSTRUCT_HPP
