#pragma once

#include "Sack.h"
#include "SystemAllocator.h"
#include "defaults.h"

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <set>

namespace grain
{

template <std::size_t max_grain_size = DEFAULT_MAX_GRAIN_SIZE,
          std::size_t handful_size = DEFAULT_HANDFUL_SIZE>
class Granary
{
private:
    using Self = Granary<max_grain_size, handful_size>;

    std::set<Sack, std::less<>, SystemAllocator<Sack>> _stock;
    Sack* _pick_sack {};
    Sack* _put_back_sack {};

public:
    Granary() noexcept = default;

    static Self& reach() noexcept
    {
        static Self granary;
        return granary;
    };

    /**
     * @return true iff the grain was picked from this granary.
     */
    bool owns(void* const grain, const std::size_t size) const noexcept
    {
        if (size == 0) return false;

        const std::set<Sack>::const_iterator it = _stock.find(Sack(handful_size / size, size));
        return it != _stock.end() && it->owns(grain);
    }

    template <typename T>
    bool owns(T* const grain) const noexcept { return owns(grain, sizeof(T)); }

    void* pick(const std::size_t grain_size) noexcept
    {
        if (grain_size == 0) return nullptr;

        // Fall back to system allocator for big objects
        if (grain_size > max_grain_size) return std::malloc(grain_size);

        if (!_pick_sack || _pick_sack->grain_size() != grain_size)
        {
            _pick_sack = find_right_sack(grain_size);
            if (!_pick_sack) _pick_sack = fill_new_sack(grain_size);
            assert(_pick_sack != nullptr);
        }

        return _pick_sack->pick();
    }

    bool put_back(void* const grain, const std::size_t size) noexcept
    {
        if (!grain) return true;

        // Fall back to system allocator for big objects
        if (size > max_grain_size)
        {
            std::free(grain);
            return true;
        }

        if (!_put_back_sack || _put_back_sack->grain_size() != size)
        {
            _put_back_sack = find_right_sack(size);
            if (!_put_back_sack) return false;
        }

        return _put_back_sack->put_back(grain);
    }

    // Disable copy and move
    Granary(const Self&) = delete;
    Granary(Self&&) = delete;
    Self& operator=(const Self&) = delete;
    Self& operator=(Self&&) = delete;

private:
    /**
     * Get a mutable pointer from a set iterator.
     *
     * I know, I know, this is ugly and I should not do it,
     * however, std::set iterators only const references,
     * because they have to guarantee that keys will not break the order and bla bla bla...
     * Now, let me tell you that the key (Sack) is ordered by comparing `grain_size`,
     * which is declared const and will remain const happily ever after!!
     * (that is, unless I sneaked a const_cast somewhere else...)
     *
     * I HEREBY SWEAR UPON THE HIGHEST PRINCIPLES OF CORRECTNESS THAT THIS IS A SAFE CONST CAST.
     * YOU CAN COUNT ON MY WORD, AND MAY GOD HAVE MERCY ON ME IF THIS IS NOT TRUE!
     *
     * (btw, I know I could store a unique_ptr and use a custom comparator,
     *  but I just don't feel like rewriting this mess right now)
     */
    static Sack* open(std::set<Sack>::iterator it) noexcept { return const_cast<Sack*>(&*it); }

    Sack* fill_new_sack(const std::size_t grain_size) noexcept
    {
        auto [it, _] = _stock.emplace(handful_size / grain_size, grain_size);
        return open(it);
    }

    Sack* find_right_sack(const std::size_t grain_size) noexcept
    {
        std::set<Sack>::iterator it = _stock.find(Sack(handful_size / grain_size, grain_size));
        return it != _stock.end() ? open(it) : nullptr;
    }
};

} // namespace grain
