#include "grain.h"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

struct Wheat : grain::Grain
{
    unsigned product_id;
    char origin[3];
    float weight;
    float kcal;
};

using TheGranary = grain::Granary<>;

void test_simple_alloc_dealloc()
{
    // Standard new does not support designated init lists (GNU extension), so pass a tmp object
    Wheat* w = new Wheat({
        .product_id = 73248761,
        .origin = "IT",
        .weight = 1.5f,
        .kcal = 1200.0f,
    });
    assert(TheGranary::reach().owns(w));

    assert(std::string(w->origin) == "IT");
    assert(w->kcal == 1200.0f);

    delete w;
}

void test_multiple_allocations()
{
    Wheat* bag[10];
    for (unsigned i = 0; i < 10; ++i)
    {
        bag[i] = new Wheat({
            .product_id = i,
            .origin = "IT",
            .weight = 1.0f,
            .kcal = 100.0f,
        });
    }

    for (unsigned i = 0; i < 10; ++i) assert(bag[i]->product_id == i);
    for (unsigned i = 0; i < 10; ++i) delete bag[i];
}

void test_vector()
{
    std::vector<Wheat*> bag;
    for (unsigned i = 0; i < 5; ++i)
    {
        bag.push_back(new Wheat({
            .product_id = i * 10,
            .origin = "IT",
            .weight = 0.5f,
            .kcal = 50.0f
        }));
    }

    for (unsigned i = 0; i < 5; ++i) assert(bag[i]->product_id == i * 10);
    for (Wheat* w : bag) delete w;
}

int main()
{
    test_simple_alloc_dealloc();
    test_multiple_allocations();
    test_vector();
}
