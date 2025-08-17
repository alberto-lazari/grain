#pragma once

#include <cstdlib>
#include <limits>
#include <new>

namespace grain
{

/**
 * STL-compliant allocator using the malloc/free for the underlying management.
 */
template <typename T>
struct SystemAllocator
{
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using void_pointer = void*;
    using const_void_pointer = const void*;

    SystemAllocator() noexcept = default;

    template <typename U>
    constexpr SystemAllocator(const SystemAllocator<U>&) noexcept
    {
    }

    template <typename U>
    struct rebind { using other = SystemAllocator<U>; };

    static constexpr size_type max_size() noexcept
    {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    static pointer allocate(const size_type n = 1)
    {
        if (n == 0) return nullptr;
        if (n > max_size()) throw std::bad_array_new_length();

        void* const p = std::malloc(sizeof(value_type) * n);
        if (!p) throw std::bad_alloc();
        return reinterpret_cast<pointer>(p);
    }

    static void deallocate(const pointer p, const size_type = 1) noexcept { std::free(p); }

    template <typename U>
    constexpr bool operator==(const SystemAllocator<U>&) const noexcept { return true; }

    template <typename U>
    constexpr bool operator!=(const SystemAllocator<U>& other) const noexcept
    {
        return !operator==(other);
    }
};

} // namespace grain
