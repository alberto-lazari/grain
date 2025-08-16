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
    Wheat* w = new Wheat {
        .product_id = 73248761,
        .origin = "IT",
        .weight = 1.5f,
        .kcal = 1200.0f,
    };
    assert(TheGranary::reach().owns(w));

    assert(std::string(w->origin) == "IT");
    assert(w->kcal == 1200.0f);

    delete w;
}

void test_array()
{
    Wheat* small = new Wheat {
        .product_id = 1,
        .origin = "FR",
        .weight = 0.25f,
        .kcal = 500.0f,
    };
    Wheat* large = new Wheat {
        .product_id = 2,
        .origin = "US",
        .weight = 1.0f,
        .kcal = 2500.0f,
    };
    assert(TheGranary::reach().owns(small));
    assert(TheGranary::reach().owns(large));

    Wheat* packs = new Wheat[2] { *small, *large };
    assert(!TheGranary::reach().owns(packs));
}

void test_multiple_allocations()
{
    Wheat* bag[10];
    for (unsigned i = 0; i < 10; ++i)
    {
        bag[i] = new Wheat {
            .product_id = i,
            .origin = "IT",
            .weight = 1.0f,
            .kcal = 100.0f,
        };
    }

    for (unsigned i = 0; i < 10; ++i) assert(bag[i]->product_id == i);
    for (unsigned i = 0; i < 10; ++i) delete bag[i];
}

void test_vector()
{
    std::vector<Wheat*> bag;
    for (unsigned i = 0; i < 5; ++i)
    {
        bag.push_back(new Wheat {
            .product_id = i * 10,
            .origin = "IT",
            .weight = 0.5f,
            .kcal = 50.0f,
        });
    }

    for (unsigned i = 0; i < 5; ++i) assert(bag[i]->product_id == i * 10);
    for (Wheat* w : bag) delete w;
}


struct MixedCereals : grain::Grain
{
    unsigned product_id;
    char origin[3][64];
    float weight;
    float kcal;
    char ingredients[4096];
};

void test_big_obj()
{
    MixedCereals* cereals = new MixedCereals {
        .product_id = 21387,
        .origin { "IT", "US", "ES" },
        .weight = 0.5f,
        .kcal = 2000.0f,
        .ingredients = "Rice, Wheat, Cereal, Chocolate, Sugar",
    };
    assert(!TheGranary::reach().owns(cereals));
    delete cereals;
}

int main()
{
    test_simple_alloc_dealloc();
    test_array();
    test_multiple_allocations();
    test_vector();
    test_big_obj();

    std::cout << "Test passed\n";

    return 0;
}
