#include "grain.h"

// Allocator
void test_simple_alloc_dealloc()
{
    int* p = grain::Allocator<int>::allocate(1);
    grain::Allocator<int>::deallocate(p, 1);
}

void test_stl_compliance()
{
    using T = int;
    using A = grain::Allocator<T>;
    const A a;
    constexpr A::size_type n = 5;

    const A::size_type max_size = a.max_size();
    assert(sizeof(A::value_type) * n <= max_size);

    A::pointer p = a.allocate(n);
    A::const_pointer cp = p;
    A::value_type& r = *p;
    const A::value_type& cr = *cp;

    A::void_pointer vp = p;
    A::const_void_pointer cvp = cp;
    assert(static_cast<A::pointer>(vp) == p);
    assert(static_cast<A::const_pointer>(cvp) == cp);

    a.deallocate(p, n);

    A a1(a);
    assert(a1 == a);

    using U = char;
    using B = grain::Allocator<U>;
    B b;
    A a2(b);
    assert(B(a) == b && A(b) == a);

    A a3(std::move(a));
    assert(a3 == a);

    A a4(std::move(b));
}

int main()
{
    test_simple_alloc_dealloc();
    test_stl_compliance();

    return 0;
}
