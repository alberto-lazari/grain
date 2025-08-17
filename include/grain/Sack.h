#pragma once

#include "Handful.h"
#include "SystemAllocator.h"

#include <cstddef>
#include <vector>

namespace grain
{

class Sack
{
private:
    const std::size_t _grain_size;
    const Handful::Count _handful_capacity;

    std::vector<Handful, SystemAllocator<Handful>> _load;
    Handful* _pick_hand {};
    Handful* _put_back_hand {};

public:
    Sack(const std::size_t handful_capacity, const std::size_t grain_size) noexcept;

    constexpr std::size_t grain_size() const noexcept { return _grain_size; }
    bool owns(void* const grain) const noexcept;

    void* pick() noexcept;
    bool put_back(void* const grain) noexcept;

    constexpr bool operator<(const Sack& other) const noexcept
    {
        return _grain_size < other._grain_size;
    }

private:
    Handful* grab_new_handful() noexcept;
    Handful* find_with_grains() noexcept;
    Handful* find_with_room() noexcept;
};

} // namespace grain
