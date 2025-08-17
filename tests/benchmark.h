#pragma once

#include "grain.h"

#include <chrono>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <random>
#include <unordered_set>
#include <utility>

// Assume minimum alignment in objects
constexpr std::size_t min_alignment = 2;

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
    allocations.emplace(new Object<SIZE>, SIZE); \
    break;

void new_obj()
{
    const std::size_t size = (rand() % 16) + 1;
    switch (size)
    {
        CASE_NEW(1 * min_alignment)
        CASE_NEW(2 * min_alignment)
        CASE_NEW(3 * min_alignment)
        CASE_NEW(4 * min_alignment)
        CASE_NEW(5 * min_alignment)
        CASE_NEW(6 * min_alignment)
        CASE_NEW(7 * min_alignment)
        CASE_NEW(8 * min_alignment)
        CASE_NEW(9 * min_alignment)
        CASE_NEW(10 * min_alignment)
        CASE_NEW(11 * min_alignment)
        CASE_NEW(12 * min_alignment)
        CASE_NEW(13 * min_alignment)
        CASE_NEW(14 * min_alignment)
        CASE_NEW(15 * min_alignment)
        CASE_NEW(16 * min_alignment)
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
        CASE_DELETE(1 * min_alignment)
        CASE_DELETE(2 * min_alignment)
        CASE_DELETE(3 * min_alignment)
        CASE_DELETE(4 * min_alignment)
        CASE_DELETE(5 * min_alignment)
        CASE_DELETE(6 * min_alignment)
        CASE_DELETE(7 * min_alignment)
        CASE_DELETE(8 * min_alignment)
        CASE_DELETE(9 * min_alignment)
        CASE_DELETE(10 * min_alignment)
        CASE_DELETE(11 * min_alignment)
        CASE_DELETE(12 * min_alignment)
        CASE_DELETE(13 * min_alignment)
        CASE_DELETE(14 * min_alignment)
        CASE_DELETE(15 * min_alignment)
        CASE_DELETE(16 * min_alignment)
        default: return;
    }
}

void benchmark(const long long iterations, const bool sequential = false)
{
    for (long long i = 0; i < iterations; ++i)
    {
        const bool allocate = rand() % 2;
        if (sequential || allocations.empty() || allocate) new_obj();
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

    benchmark(iterations, seed == 0LL ? true : false);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "\ttook: " << elapsed.count() << " seconds" << std::endl;
}

int main()
{
    constexpr std::size_t seed_count = 4;
    long long seeds[seed_count] { 3127344003LL, 1878122349LL, 4119109763LL, 0LL };
    long long iterations[seed_count] { 100'000LL, 1'000'000LL, 2'000'000LL, 50'000LL };
    for (std::size_t i = 0; i < seed_count; ++i) run(seeds[i], iterations[i]);

    return 0;
}
