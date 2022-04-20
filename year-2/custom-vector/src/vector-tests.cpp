
#include <iostream>
#include <cassert>
#include <algorithm>
#include "vector/vector.hpp"
#include "vector/storages/dynamic-storage.hpp"
#include "vector/storages/chunked/chunked-storage.hpp"
#include "vector/storages/static-storage.hpp"

// TODO: adopt tests for new usings

int allocations = 0;
class ComplexInt
{
public:
    ComplexInt() : ComplexInt(0) {}
    ComplexInt(int value) : value_(value), dummy_(malloc(1024)), throw_(false) { allocations++; }

    ComplexInt(const ComplexInt &other) : ComplexInt(other.value_)
    {
        if (other.throw_)
            throw std::runtime_error("lol");

        // std::cout << "copy" << std::endl;
    }

    // ComplexInt(ComplexInt &&other) : value_(other.value_), dummy_(malloc(1024))
    // {
    //     other.value_ = 0;
    //     std::cout << "move" << std::endl;
    // }

    operator int() const
    {
        return value_;
    }

    ComplexInt &operator=(int value)
    {
        value_ = value;
        return *this;
    }

    ComplexInt &operator=(const ComplexInt &other)
    {
        value_ = other.value_;
        if (other.throw_)
            throw std::runtime_error("lol");

        // std::cout << "copy assignment" << std::endl;

        return *this;
    }

    // ComplexInt &operator=(ComplexInt &&other)
    // {
    //     dummy_ = malloc(1024);

    //     value_ = other.value_;
    //     other.value_ = 0;

    //     if (other.throw_)
    //         throw std::runtime_error("lol");

    //     std::cout << "move assignment" << std::endl;

    //     return *this;
    // }

    ~ComplexInt()
    {
        if(dummy_) allocations--;
        free(dummy_);
    }

public:
    bool throw_;

private:
    int value_;
    void *dummy_;
};

#define TestStorage DynamicStorage<>::type
//#define TestStorage ChunkedStorage<>::type

using TestType = ComplexInt;


static void test1()
{
    Vector<Vector<TestType, StaticStorage<10>::type>, TestStorage> arr;

    for (int i = 0; i < 100; i++)
    {
        assert(arr.size() == i);
        arr.emplace_back();
        assert(arr[i].size() == 10);
        for (int j = 0; j < 10; j++)
            arr.back()[j] = i + j;
    }

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < arr[i].size(); j++)
            assert(arr[i][j] == i + j);
    }

    //--------------------------------------------------------

    const Vector<Vector<TestType, StaticStorage<10>::type>, TestStorage> copy = arr;
    assert(copy.size() == 100);

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < copy[i].size(); j++)
            assert(copy[i][j] == i + j);
    }

    //--------------------------------------------------------

    const Vector<Vector<TestType, StaticStorage<10>::type>, TestStorage> move = std::move(arr);
    assert(move.size() == 100);
    assert(arr.size() == 0);

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < move[i].size(); j++)
            assert(move[i][j] == i + j);
    }

    Vector<Vector<TestType, StaticStorage<10>::type>, TestStorage> copy2 = move;
    Vector<Vector<TestType, StaticStorage<10>::type>, TestStorage> copy3 = move;
    assert(copy2.size() == 100);
    assert(copy3.size() == 100);

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < copy[i].size(); j++)
        {
            assert(copy2[i][j] == i + j);
            assert(copy3[i][j] == i + j);
        }
    }

    copy2.resize(50);
    assert(copy2.size() == 50);
    copy2 = copy3;
    assert(copy2.size() == 100);
    assert(copy3.size() == 100);
    copy2 = std::move(copy3);
    assert(copy2.size() == 100);
    assert(copy3.size() == 0);

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < copy[i].size(); j++)
        {
            assert(copy2[i][j] == i + j);
        }
    }

    //--------------------------------------------------------

    copy2.clear();
    assert(copy2.size() == 0);
}

static void test2()
{
    Vector<Vector<TestType, TestStorage>, TestStorage> arr(100);
    assert(arr.size() == 100);

    for (int i = 0; i < 100; i++)
    {
        arr[i].resize(100);
        assert(arr[i].size() == 100);
        for (int j = 0; j < 100; j++)
            arr[i][j] = i + j;
    }

    for (int i = 0; i < arr.size(); i++)
    {
        for (int j = 0; j < arr[i].size(); j++)
            assert(arr[i][j] == i + j);
    }

    arr[37].clear();
    assert(arr[37].size() == 0);
    arr.clear();
    assert(arr.size() == 0);
}

static void test3()
{
    Vector<Vector<TestType, StaticStorage<10>::type>, TestStorage> arr;

    for (int i = 0; i < 100; i++)
    {
        arr.emplace_back();
        for (int j = 0; j < 10; j++)
            arr.back()[j] = i + j;
    }

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < arr[i].size(); j++)
            assert(arr[i][j] == i + j);
    }
}

static void test4()
{
    Vector<TestType, TestStorage> arr(4096);

    for (int j = 0; j < 128; j++)
    {
        for (int i = 0; i < arr.size(); i++)
            arr[i] = i;

        while (!arr.empty())
        {
            for (int i = 0; i < 128; i++)
                arr.pop_back();

            arr.shrink();
        }

        arr.resize(4096);
    }
}

static void test5()
{
    Vector<bool, TestStorage> arr(100);
    assert(arr.size() == 100);

    for (int i = 0; i < 100; i++)
        arr[i] = i % 2;

    for (int i = 100; i < 137; i++)
    {
        assert(arr.size() == i);
        arr.push_back(i % 2);
    }

    for (int i = 0; i < 137; i++)
        assert(arr[i] == i % 2);

    arr.clear();
    assert(arr.size() == 0);
}

static void test6()
{
    const Vector<TestType, TestStorage> arr = {1, 2, 3, 4, 5};
    assert(arr.size() == 5);

    for (int i = 0; i < 5; i++)
        assert(arr[i] == i + 1);

    Vector<TestType, TestStorage> arr2 = arr;
    assert(arr2.size() == 5);

    for (int i = 0; i < 5; i++)
        assert(arr2[i] == i + 1);

    arr2 = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    assert(arr2.size() == 10);
    for (int i = 0; i < 10; i++)
        assert(arr2[i] == 11 + i);

    arr2.clear();
    assert(arr2.size() == 0);
}

static void test7()
{
    const Vector<bool, TestStorage> arr = {0, 1, 0, 1, 0};
    assert(arr.size() == 5);

    for (int i = 0; i < 5; i++)
        assert(arr[i] == i % 2);

    Vector<bool, TestStorage> arr2 = arr;
    assert(arr2.size() == 5);

    for (int i = 0; i < 5; i++)
        assert(arr2[i] ==  i % 2);

    arr2 = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
    assert(arr2.size() == 10);
    for (int i = 0; i < 10; i++)
        assert(arr2[i] == (i + 1) % 2);

    arr2.clear();
    assert(arr2.size() == 0);
}

// static void test8()
// {
//     Array<Array<ComplexInt, StaticStorage, 1>, StaticStorage, 10> arr;
//     for (int i = 0; i < 10; i++)
//     {
//         arr[i][0] = i % 2;
//     }

//     Array<Array<ComplexInt, StaticStorage, 1>, StaticStorage, 10> move = std::move(arr);
// }

static void test9()
{
    Vector<int, TestStorage> arr = {5, 4, 3, 2, 1, 6, 7, 8, 9, 10};
    Vector<int, TestStorage> arr_copy(8);

    std::sort(arr.begin(), arr.end());
    std::copy(arr.begin(), arr.begin() + 8, arr_copy.begin());

    int i = 0;
    for (auto &elem : arr_copy)
    {
        assert(elem == i + 1);
        i++;
    }
}

static void test10()
{
    const Vector<int, TestStorage> arr = {1, 2, 3, 4, 5, 6, 7, 8, 10};
    Vector<int, TestStorage> arr_copy(8);

    std::copy(arr.begin(), arr.begin() + 8, arr_copy.begin());

    int i = 0;
    for (auto &elem : arr_copy)
    {
        assert(elem == i + 1);
        i++;
    }
}

static void test11()
{
    const Vector<bool, TestStorage> arr = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
    Vector<bool, TestStorage> arr_copy(8);

    std::copy(arr.begin(), arr.begin() + 8, arr_copy.begin());

    int i = 1;
    for (auto elem : arr_copy)
    {
        assert(elem == i % 2);
        i++;
    }
}

void vector_tests()
{
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
    test7();
    //test8();
    test9();
    test10();
    test11();
    assert(!allocations);

    std::cout << "All tests passed (ﾉ^_^)ﾉ\n";
}