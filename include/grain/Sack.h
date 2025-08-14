#pragma once

#include "grain/Handful.h"

#include <cstdint>
#include <cstddef>
#include <vector>

namespace grain
{

class Sack
{
private:
    std::vector<Handful> load;
    std::size_t grain_size;
    Handful::Count_t handful_size;
    Handful* pick_hand {};
    Handful* put_back_hand {};

public:
    void* pick() noexcept;
    bool put_back(void* const grain) noexcept;

private:
    Handful* grab_new_handful() noexcept;
    Handful* find_with_grains() noexcept;
    Handful* find_with_room() noexcept;
};

} // namespace grain
