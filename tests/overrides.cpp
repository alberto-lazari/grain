#define GRAIN_HARVEST_MODE

#include "grain/overrides.h"
#include "test_assert.h"

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
    TEST_ASSERT(s->x == 42);
    delete s;
}

void test_new_default()
{
    S* const s = new S();
    TEST_ASSERT(s->x == -1);
    delete s;
}

void test_new_nothrow()
{
    S* const s = new (std::nothrow) S(99);
    TEST_ASSERT(s && s->x == 99);
    delete s;
}

void test_new_array()
{
    constexpr std::size_t n = 5;
    S* const arr = new S[n] { 1, 2, 3, 4, 5 };
    for (std::size_t i = 0; i < n; ++i)
        TEST_ASSERT(arr[i].x == static_cast<int>(i + 1));

    delete[] arr;
}

void test_new_array_nothrow()
{
    constexpr std::size_t n = 5;
    S* const arr = new (std::nothrow) S[n] { 10, 20, 30, 40, 50 };
    TEST_ASSERT(arr);

    for (std::size_t i = 0; i < n; ++i)
        TEST_ASSERT(arr[i].x == static_cast<int>((i + 1) * 10));

    delete[] arr;
}

void test_delete_nullptr()
{
    int* const p {};
    TEST_ASSERT(p == nullptr);
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
