#pragma once

#include "grain/Granary.h"
#include "grain/defaults.h"

#include <cassert>
#include <cstddef>
#include <limits>

namespace grain
{

/**
 * STL-compliant allocator using the global Granary.
 * Satisfies requirements listed here:
 * https://en.cppreference.com/w/cpp/named_req/Allocator
 */
template <typename T,
          std::size_t max_grain_size = DEFAULT_MAX_GRAIN_SIZE,
          std::size_t handful_size = DEFAULT_HANDFUL_SIZE>
struct Allocator
{
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using void_pointer = void*;
    using const_void_pointer = const void*;

    Allocator() noexcept = default;

    template <typename U>
    constexpr Allocator(const Allocator<U, max_grain_size, handful_size>&) noexcept
    {
    }

    template <typename U>
    struct rebind { using other = Allocator<U, max_grain_size, handful_size>; };

    static constexpr size_type max_size() noexcept
    {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    /**
     * Non-standard noexcept allocation.
     */
    static pointer _allocate_nothrow(const size_type n) noexcept
    {
        void* const p = TheGranary::reach().pick(sizeof(value_type) * n);
        return static_cast<pointer>(p);
    }

    static pointer allocate(const size_type n)
    {
        if (n > max_size()) throw std::bad_array_new_length();

        void* const p = TheGranary::reach().pick(sizeof(value_type) * n);
        if (!p) throw std::bad_alloc();
        return static_cast<pointer>(p);
    }

    static void deallocate(const pointer p, const size_type n) noexcept
    {
        const bool all_well = TheGranary::reach().put_back(p, sizeof(value_type) * n);
        assert(all_well);
    }

    template <typename U>
    constexpr bool operator==(const Allocator<U>& other) const noexcept { return true; }

    template <typename U>
    constexpr bool operator!=(const Allocator<U>& other) const noexcept
    {
        return !operator==(other);
    }

private:
    using TheGranary = Granary<max_grain_size, handful_size>;
};

} // namespace grain
