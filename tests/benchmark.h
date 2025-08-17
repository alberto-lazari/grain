#pragma once

#include "grain.h"

#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <random>
#include <unordered_set>
#include <utility>

template <std::size_t size>
struct Object { std::byte data[size]; };

using Allocation = std::pair<void*, std::size_t>;

struct Hash
{
    std::size_t operator()(const Allocation& a) const noexcept
    {
        return std::hash<void*>()(a.first);
    }
};

struct Eq
{
    bool operator()(const Allocation& a, const Allocation& b) const noexcept
    {
        return a.first == b.first;
    }
};

static std::unordered_set<Allocation, Hash, Eq, grain::SystemAllocator<Allocation>> allocations;

#define CASE_NEW(SIZE) case SIZE: \
    allocations.emplace(new Object<SIZE>(), SIZE); \
    break;

void new_obj()
{
    const std::size_t size = (rand() % 16) + 1;
    switch (size)
    {
        CASE_NEW(1)
        CASE_NEW(2)
        CASE_NEW(4)
        CASE_NEW(6)
        CASE_NEW(8)
        CASE_NEW(10)
        CASE_NEW(12)
        CASE_NEW(14)
        CASE_NEW(16)
        default: return;
    }
}

#define CASE_DELETE(SIZE) case SIZE: \
    delete reinterpret_cast<Object<SIZE>*>(p); \
    break;

void delete_obj()
{
    if (allocations.empty()) return;

    // Pick random allocation
    auto it = allocations.begin();
    std::advance(it, rand() % allocations.size());
    auto [p, size] = *it;
    allocations.erase(it);

    switch (size)
    {
        CASE_DELETE(1)
        CASE_DELETE(2)
        CASE_DELETE(4)
        CASE_DELETE(6)
        CASE_DELETE(8)
        CASE_DELETE(10)
        CASE_DELETE(12)
        CASE_DELETE(14)
        CASE_DELETE(16)
        default: return;
    }
}

void benchmark(const long long iterations)
{
    for (long long i = 0; i < iterations; ++i)
    {
        const bool allocate = rand() % 2;
        if (allocations.empty() || allocate) new_obj();
        else delete_obj();
    }

    // Clean all objects left
    while (!allocations.empty()) delete_obj();
}

void run(const long long seed, const long long iterations)
{
    std::srand(seed);
    std::cout << "Seed: " << seed << ", iterations: " << iterations;

    auto start = std::chrono::high_resolution_clock::now();

    benchmark(iterations);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "\ttook: " << elapsed.count() << " seconds" << std::endl;
}

int main()
{
    constexpr std::size_t seed_count = 3;
    long long seeds[seed_count] { 3127344003LL, 1878122349LL, 4119109763LL };
    long long iterations[seed_count] { 100'000LL, 1'000'000LL, 10'000'000LL };
    for (std::size_t i = 0; i < seed_count; ++i) run(seeds[i], iterations[i]);

    return 0;
}
