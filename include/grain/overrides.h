#pragma once

#include "utils.h"

#include <cassert>
#include <iostream>
#include <new>

#ifdef GRAIN_HARVEST_MODE

void* operator new(const std::size_t size, const std::nothrow_t&) noexcept
{
    return grain::Malloc(size);
}

void* operator new(const std::size_t size)
{
    if (void* const p = operator new(size, std::nothrow)) return p;
    throw std::bad_alloc();
}

void operator delete(void* const p) noexcept
{
    // TODO: force the compiler to call the sized version
    std::cerr << "Warning: custom '::operator delete(void*)' does nothing" << std::endl;
}

void operator delete(void* const p, const std::size_t size) noexcept { grain::Free(p, size); }


void* operator new[](const std::size_t size, const std::nothrow_t&) noexcept
{
    // Allocate extra space to store the size
    void* p = grain::Malloc(sizeof(std::size_t) + size);
    if (!p) return nullptr;

    // Store size and return pointer past it
    std::size_t* const size_ptr = reinterpret_cast<std::size_t*>(p);
    *size_ptr = size;

    return size_ptr + 1;
}

void* operator new[](const std::size_t size)
{
    if (void* const p = operator new[](size, std::nothrow)) return p;
    throw std::bad_alloc();
}

void operator delete[](void* const p) noexcept
{
    if (!p) return;

    // Recover original allocation size
    size_t* const size_ptr = reinterpret_cast<std::size_t*>(p) - 1;
    const std::size_t size = *size_ptr;

    grain::Free(size_ptr, sizeof(std::size_t) + size);
}

void operator delete[](void* const p, const std::size_t size) noexcept
{
    if (!p) return;

    std::size_t* const size_ptr = reinterpret_cast<std::size_t*>(p) - 1;
    assert(size == *size_ptr);

    grain::Free(size_ptr, sizeof(std::size_t) + size);
}

#endif // GRAIN_HARVEST_MODE
