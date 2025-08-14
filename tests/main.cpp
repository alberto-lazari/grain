#include "grain.h"

void test_simple_alloc_dealloc()
{
    grain::Allocator<int> alloc;
    int* n = alloc.allocate(1);
    alloc.deallocate(n, 1);
}

int main()
{
    test_simple_alloc_dealloc();

    return 0;
}
