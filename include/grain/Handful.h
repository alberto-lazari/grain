#pragma once

#include <cstdint>
#include <cstddef>
#include <limits>

namespace grain
{

class Handful
{
public:
    using Count = std::uint8_t;

private:
    std::byte* _grains {};
    Count _first_available_grain = 0;
    Count _available_grains = 0;

public:
    static constexpr std::size_t MAX_HAND_CAPACITY = std::numeric_limits<Count>::max();

    Handful() noexcept = default;

    Handful(Handful&& other) noexcept;
    Handful& operator=(Handful&& other) noexcept;

    // Prevent copies
    Handful(const Handful&) = delete;
    Handful& operator=(const Handful&) = delete;

    ~Handful();


    constexpr bool is_empty() const noexcept { return _available_grains <= 0; }
    constexpr bool is_full() const noexcept { return _available_grains >= MAX_HAND_CAPACITY; }

    /**
     * Grab a handful of grains from the sack, ready to be distributed.
     */
    bool grab(const std::size_t grain_size, const Count grains_count) noexcept;

    /**
     * Pick a grain from the handful (request allocated memory).
     */
    void* pick(const std::size_t grain_size) noexcept;

    /**
     * Put a grain back into the handful for reuse (release memory).
     */
    bool put_back(void* const grain, const std::size_t grain_size) noexcept;
};

} // namespace grain
