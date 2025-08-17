#include "grain.h"

#include <cassert>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

void test_simple_alloc_dealloc()
{
    int* p = grain::Allocator<int>::allocate();
    grain::Allocator<int>::deallocate(p);
}

template <typename A, typename B>
void test_stl_compliance()
{
    const A a;
    constexpr typename A::size_type n = 5;

    const typename A::size_type max_size = a.max_size();
    assert(sizeof(typename A::value_type) * n <= max_size);

    typename A::pointer p = a.allocate(n);
    typename A::const_pointer cp = p;
    typename A::value_type& r = *p;
    const typename A::value_type& cr = *cp;

    typename A::void_pointer vp = p;
    typename A::const_void_pointer cvp = cp;
    assert(static_cast<A::pointer>(vp) == p);
    assert(static_cast<A::const_pointer>(cvp) == cp);

    a.deallocate(p, n);

    A a1(a);
    assert(a1 == a);

    B b;
    A a2(b);
    assert(B(a) == b && A(b) == a);

    A a3(std::move(a));
    assert(a3 == a);

    A a4(std::move(b));
}

template <typename IntAllocator, typename CharAllocator, typename PairAllocator>
void test_stl_containers()
{
    // Vector
    std::vector<int, IntAllocator> vec;
    for (int i = 0; i < 100; ++i) vec.push_back(i);
    assert(vec.size() == 100);
    assert(vec[42] == 42);

    // String
    std::basic_string<char, std::char_traits<char>, CharAllocator> str = "hello";
    str += " world";
    assert(str == "hello world");

    // Map
    std::map<std::string, int, std::less<std::string>, PairAllocator> m;

    m["one"] = 1;
    m["two"] = 2;
    m["three"] = 3;

    assert(m.size() == 3);
    assert(m.at("two") == 2);

    // Set
    std::set<int, std::less<int>, IntAllocator> s;

    s.insert(10);
    s.insert(20);
    s.insert(30);

    assert(s.find(20) != s.end());
    assert(s.count(10) == 1);
}

#define TEST_STL(Allocator) \
    test_stl_compliance<Allocator<int>, Allocator<char>>(); \
    test_stl_containers<Allocator<int>, Allocator<char>, \
                        Allocator<std::pair<const std::string, int>>>()

int main()
{
    test_simple_alloc_dealloc();

    TEST_STL(grain::Allocator);
    TEST_STL(grain::SystemAllocator);

    std::cout << "Test passed\n";

    return 0;
}
