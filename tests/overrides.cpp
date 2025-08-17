#define GRAIN_HARVEST_MODE

#include "grain/overrides.h"

#include <cassert>
#include <iostream>

struct S
{
    int x;

    S(int x_ = -1) : x(x_)
    {
    }
};

void test_new()
{
    S* const s = new S(42);
    assert(s->x == 42);
    delete s;
}

void test_new_default()
{
    S* const s = new S();
    assert(s->x == -1);
    delete s;
}

void test_new_nothrow()
{
    S* const s = new (std::nothrow) S(99);
    assert(s && s->x == 99);
    delete s;
}

void test_new_array()
{
    constexpr std::size_t n = 5;
    S* const arr = new S[n] { 1, 2, 3, 4, 5 };
    for (std::size_t i = 0; i < n; ++i)
        assert(arr[i].x == static_cast<int>(i + 1));

    delete[] arr;
}

void test_new_array_nothrow()
{
    constexpr std::size_t n = 5;
    S* const arr = new (std::nothrow) S[n] { 10, 20, 30, 40, 50 };
    assert(arr);

    for (std::size_t i = 0; i < n; ++i)
        assert(arr[i].x == static_cast<int>((i + 1) * 10));

    delete[] arr;
}

void test_delete_nullptr()
{
    int* const p {};
    assert(p == nullptr);
    delete p;
}

int main()
{
    test_new();
    test_new_default();
    test_new_nothrow();
    test_new_array();
    test_new_array_nothrow();
    test_delete_nullptr();

    std::cout << "Test passed\n";

    return 0;
}
