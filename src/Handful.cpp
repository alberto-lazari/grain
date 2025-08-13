#include "grain/Handful.h"

#include <new>

namespace grain
{

void Handful::grab(const std::size_t grain_size, const uint8_t grains_count) noexcept
{
    // Cannot hold more than HAND_CAPACITY grains, fail silently
    if (grains_count > HAND_CAPACITY) return;

    grains = new (std::nothrow) std::byte[grain_size * grains_count];

    // Fail silently on allocation failure
    if (grains == nullptr) return;

    first_available_grain = 0;
    available_grains = grains_count;

    // Initialize blocks to point the next available one
    uint8_t i = 0;
    for (std::byte* grain = grains; i < grains_count; grain += grain_size)
    {
        *reinterpret_cast<uint8_t*>(grain) = ++i;
    }
}

void* Handful::pick(const std::size_t grain_size) noexcept
{
    if (!available_grains) return nullptr;

    std::byte* grain = grains + (first_available_grain * grain_size);

    // Next available grain is the one pointed by the first one
    first_available_grain = *reinterpret_cast<uint8_t*>(grain);
    --available_grains;

    return grain;
}

void Handful::put_back(void* const grain, const std::size_t grain_size) noexcept
{
    const std::size_t grain_index = (static_cast<std::byte*>(grain) - grains) / grain_size;

    if (available_grains >= HAND_CAPACITY || grain < grains || grain_index > HAND_CAPACITY)
    {
        // Something went wrong, a grain has been put back in the wrong handful.
        // Mark it with special value as a fall-back.
        // 0 is special because internal pointers to other grains start from 1,
        // so it can be treated as a "failed to put back" error code.
        *reinterpret_cast<uint8_t*>(grain) = 0;
        return;
    }

    // Add grain to the head of the queue
    *reinterpret_cast<uint8_t*>(grain) = first_available_grain;
    first_available_grain = grain_index;
    ++available_grains;
}

Handful::Handful(Handful&& other) noexcept
    : grains(other.grains)
    , first_available_grain(other.first_available_grain)
    , available_grains(other.available_grains)
{
    other.grains = nullptr;
    other.first_available_grain = 0;
    other.available_grains = 0;
}

Handful& Handful::operator=(Handful&& other) noexcept
{
    if (this != &other)
    {
        grains = other.grains;
        first_available_grain = other.first_available_grain;
        available_grains = other.available_grains;

        other.grains = nullptr;
        other.first_available_grain = 0;
        other.available_grains = 0;
    }
    return *this;
}

Handful::~Handful()
{
    delete[] grains;
}

} // namespace grain
