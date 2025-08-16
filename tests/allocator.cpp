#include "grain.h"

#include <cassert>
#include <map>
#include <set>
#include <string>
#include <vector>

void test_simple_alloc_dealloc()
{
    int* p = grain::Allocator<int>::allocate();
    grain::Allocator<int>::deallocate(p);
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

void test_stl_containers()
{
    // Vector
    std::vector<int, grain::Allocator<int>> vec;
    for (int i = 0; i < 100; ++i) vec.push_back(i);
    assert(vec.size() == 100);
    assert(vec[42] == 42);

    // String
    std::basic_string<char, std::char_traits<char>, grain::Allocator<char>> str = "hello";
    str += " world";
    assert(str == "hello world");

    // Map
    std::map<std::string, int,
             std::less<std::string>,
             grain::Allocator<std::pair<const std::string, int>>> m;

    m["one"] = 1;
    m["two"] = 2;
    m["three"] = 3;

    assert(m.size() == 3);
    assert(m.at("two") == 2);

    // Set
    std::set<int,
             std::less<int>,
             grain::Allocator<int>> s;

    s.insert(10);
    s.insert(20);
    s.insert(30);

    assert(s.find(20) != s.end());
    assert(s.count(10) == 1);
}

int main()
{
    test_simple_alloc_dealloc();
    test_stl_compliance();
    test_stl_containers();
}
