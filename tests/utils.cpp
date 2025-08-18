#include "grain/utils.h"
#include "test_assert.h"

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
    TEST_ASSERT(p != nullptr);
    TEST_ASSERT(p->x == 42);
    TEST_ASSERT(grain::Delete(p));
}

void test_single_object_nothrow()
{
    S* const p = grain::New<S>(std::nothrow, 123);
    TEST_ASSERT(p != nullptr);
    TEST_ASSERT(p->x == 123);
    TEST_ASSERT(grain::Delete(p));
}

void test_s_array()
{
    const std::size_t count = 5;
    auto* const arr = grain::New_Array<S>(count, 99);
    TEST_ASSERT(arr != nullptr);
    for (std::size_t i = 0; i < count; ++i)
        TEST_ASSERT(arr[i].x == 99);
    TEST_ASSERT(grain::Delete_Array(arr));
}

void test_c_array()
{
    // Reset counters from potential previous tests
    C::constructed = 0;
    C::destroyed = 0;

    const std::size_t count = 3;
    C* const arr = grain::New_Array<C>(count, 7);

    TEST_ASSERT(arr != nullptr);
    TEST_ASSERT(C::constructed == 3);

    TEST_ASSERT(grain::Delete_Array(arr));
    TEST_ASSERT(C::destroyed == 3);
}

void test_zero_length_array()
{
    int* const arr = grain::New_Array<int>(0);
    TEST_ASSERT(arr == nullptr);
    TEST_ASSERT(grain::Delete_Array(arr));
}

void test_bad_alloc()
{
    try
    {
        grain::New_Array<int>(-1);
        TEST_ASSERT(false && "Expected bad_array_new_length");
    }
    catch (const std::bad_array_new_length&)
    {
        // Expected
    }

    TEST_ASSERT(!grain::New_Array<int>(std::nothrow, -1));
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
