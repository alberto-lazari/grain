#pragma once

#include <cstdint>
#include <cstddef>
#include <limits>

namespace grain
{

class Handful
{
public:
    static constexpr std::size_t HAND_CAPACITY = std::numeric_limits<uint8_t>::max();

    /**
     * Grab a handful of grains from the pool, ready to be distributed.
     */
    void grab(const std::size_t grain_size, const uint8_t grains_count);

    /**
     * Pick a grain from the handful (request allocated memory).
     */
    void* pick(const std::size_t grain_size) noexcept;

    /**
     * Put a grain back into the handful for reuse (release memory).
     */
    void put_back(void* grain, const std::size_t grain_size) noexcept;

private:
    std::byte* grains {};
    uint8_t first_available_grain = 0;
    uint8_t available_grains = 0;

};

} // namespace grain
