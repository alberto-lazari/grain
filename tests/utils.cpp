#include "grain/utils.h"

#include <cassert>
#include <iostream>
#include <new>
#include <stdexcept>

struct S
{
    int x;

    S(int x_) : x(x_)
    {
    }
};

struct C
{
    static inline int constructed = 0;
    static inline int destroyed = 0;
    double d;

    C(double d_) : d(d_)
    {
        ++constructed;
    }

    ~C()
    {
        ++destroyed;
    }
};

void test_single_object()
{
    S* const p = grain::New<S>(42);
    assert(p != nullptr);
    assert(p->x == 42);
    assert(grain::Delete(p));
}

void test_single_object_nothrow()
{
    S* const p = grain::New<S>(std::nothrow, 123);
    assert(p != nullptr);
    assert(p->x == 123);
    assert(grain::Delete(p));
}

void test_s_array()
{
    const std::size_t count = 5;
    auto* const arr = grain::New_Array<S>(count, 99);
    assert(arr != nullptr);
    for (std::size_t i = 0; i < count; ++i)
        assert(arr[i].x == 99);
    assert(grain::Delete_Array(arr));
}

void test_c_array()
{
    // Reset counters from potential previous tests
    C::constructed = 0;
    C::destroyed = 0;

    const std::size_t count = 3;
    C* const arr = grain::New_Array<C>(count, 7);

    assert(arr != nullptr);
    assert(C::constructed == 3);

    assert(grain::Delete_Array(arr));
    assert(C::destroyed == 3);
}

void test_zero_length_array()
{
    int* const arr = grain::New_Array<int>(0);
    assert(arr == nullptr);
    assert(grain::Delete_Array(arr));
}

void test_bad_alloc()
{
    try
    {
        grain::New_Array<int>(-1);
        assert(false && "Expected bad_array_new_length");
    }
    catch (const std::bad_array_new_length&)
    {
        // Expected
    }

    assert(!grain::New_Array<int>(std::nothrow, -1));
}

int main() {
    test_single_object();
    test_single_object_nothrow();
    test_s_array();
    test_c_array();
    test_zero_length_array();
    test_bad_alloc();

    std::cout << "Test passed\n";

    return 0;
}
