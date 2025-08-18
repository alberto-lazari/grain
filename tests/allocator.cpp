#include "test.h"

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
    TEST_ASSERT(sizeof(typename A::value_type) * n <= max_size);

    typename A::pointer p = a.allocate(n);
    typename A::const_pointer cp = p;
    typename A::value_type& r = *p;
    (void)r;
    const typename A::value_type& cr = *cp;
    (void)cr;

    typename A::void_pointer vp = p;
    typename A::const_void_pointer cvp = cp;
    TEST_ASSERT(reinterpret_cast<A::pointer>(vp) == p);
    TEST_ASSERT(reinterpret_cast<A::const_pointer>(cvp) == cp);

    a.deallocate(p, n);

    A a1(a);
    TEST_ASSERT(a1 == a);

    B b;
    A a2(b);
    TEST_ASSERT(a2 == a && B(a) == b);

    A a3(std::move(a));
    TEST_ASSERT(a3 == a);

    A a4(std::move(b));
    TEST_ASSERT(a4 == a);

    // This should be fine and raise no exception or cause runtime errors
    a.deallocate(nullptr);
}

template <typename IntAllocator, typename CharAllocator, typename PairAllocator>
void test_stl_containers()
{
    // Vector
    std::vector<int, IntAllocator> vec;
    for (int i = 0; i < 100; ++i) vec.push_back(i);
    TEST_ASSERT(vec.size() == 100);
    TEST_ASSERT(vec[42] == 42);

    // String
    std::basic_string<char, std::char_traits<char>, CharAllocator> str = "hello";
    str += " world";
    TEST_ASSERT(str == "hello world");

    // Map
    std::map<std::string, int, std::less<std::string>, PairAllocator> m;

    m["one"] = 1;
    m["two"] = 2;
    m["three"] = 3;

    TEST_ASSERT(m.size() == 3);
    TEST_ASSERT(m.at("two") == 2);

    // Set
    std::set<int, std::less<int>, IntAllocator> s;

    s.insert(10);
    s.insert(20);
    s.insert(30);

    TEST_ASSERT(s.find(20) != s.end());
    TEST_ASSERT(s.count(10) == 1);
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
