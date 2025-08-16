#pragma once

#include "grain/Granary.h"
#include "grain/defaults.h"

#include <cassert>
#include <cstddef>

namespace grain
{

/**
 * STL-compliant allocator using the global Granary.
 */
template <typename T,
          std::size_t max_grain_size = DEFAULT_MAX_GRAIN_SIZE,
          std::size_t handful_size = DEFAULT_HANDFUL_SIZE>
struct Allocator
{
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    template <typename U>
    struct rebind { using other = Allocator<U, max_grain_size, handful_size>; };

    /**
     * Non-standard noexcept allocation.
     */
    T* _allocate_nothrow(const std::size_t n) noexcept
    {
        void* const p = TheGranary::reach().pick(sizeof(T) * n);
        return static_cast<T*>(p);
    }

    T* allocate(const std::size_t n)
    {
        void* const p = TheGranary::reach().pick(sizeof(T) * n);
        if (!p) throw std::bad_alloc();
        return static_cast<T*>(p);
    }

    void deallocate(T* const p, const std::size_t n) noexcept
    {
        const bool all_well = TheGranary::reach().put_back(p, sizeof(T) * n);
        assert(all_well);
    }

    template <typename U>
    constexpr bool operator==(const Allocator<U>& other) const noexcept { return true; }

    template <typename U>
    constexpr bool operator!=(const Allocator<U>& other) const noexcept
    {
        return !(*this == other);
    }

private:
    using TheGranary = Granary<max_grain_size, handful_size>;
};

} // namespace grain
