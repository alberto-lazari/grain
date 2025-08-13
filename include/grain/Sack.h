#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>

namespace grain
{

class Handful;

class Sack
{
private:
    std::vector<Handful> load;
    std::size_t grain_size;
    uint8_t handful_size;
    Handful* pick_hand {};
    Handful* put_back_hand {};

public:
    void* pick() noexcept;
    void put_back(void* const grain) noexcept;

private:
    Handful* grab_new_handful() noexcept;
    Handful* find_available_handful() noexcept;
};

} // namespace grain
