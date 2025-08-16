#include "grain/Handful.h"

#include <new>

namespace grain
{

Handful::Handful(Handful&& other) noexcept
    : _grains(other._grains)
    , _first_available_grain(other._first_available_grain)
    , _available_grains(other._available_grains)
{
    other._grains = nullptr;
    other._first_available_grain = 0;
    other._available_grains = 0;
}

Handful& Handful::operator=(Handful&& other) noexcept
{
    if (this != &other)
    {
        _grains = other._grains;
        _first_available_grain = other._first_available_grain;
        _available_grains = other._available_grains;

        other._grains = nullptr;
        other._first_available_grain = 0;
        other._available_grains = 0;
    }
    return *this;
}

Handful::~Handful()
{
    delete[] _grains;
}


bool Handful::grab(const std::size_t grain_size, const Count grains_count) noexcept
{
    if (grains_count > MAX_HAND_CAPACITY) return false;

    _grains = new (std::nothrow) std::byte[grain_size * grains_count];

    if (_grains == nullptr) return false;

    _first_available_grain = 0;
    _available_grains = grains_count;

    // Initialize blocks to point the next available one
    std::size_t i = 0;
    for (std::byte* grain = _grains; i < grains_count; grain += grain_size)
        *reinterpret_cast<Count*>(grain) = ++i;

    return true;
}

void* Handful::pick(const std::size_t grain_size) noexcept
{
    if (!_available_grains || grain_size == 0) return nullptr;

    std::byte* grain = _grains + (_first_available_grain * grain_size);

    // Next available grain is the one pointed by the first one
    _first_available_grain = *reinterpret_cast<Count*>(grain);
    --_available_grains;

    return grain;
}

bool Handful::owns(void* const grain, const std::size_t grain_size) const noexcept
{
    if (grain_size == 0) return false;

    const std::ptrdiff_t grain_offset = (static_cast<std::byte*>(grain) - _grains) / grain_size;
    return is_in_hand(grain_offset);
}

bool Handful::put_back(void* const grain, const std::size_t grain_size) noexcept
{
    if (grain_size == 0) return false;

    const std::ptrdiff_t grain_offset = (static_cast<std::byte*>(grain) - _grains) / grain_size;
    if (is_full() || !is_in_hand(grain_offset)) return false;

    const std::size_t grain_index = grain_offset;

    // Add grain to the head of the queue
    *reinterpret_cast<Count*>(grain) = _first_available_grain;
    _first_available_grain = grain_index;
    ++_available_grains;

    return true;
}

} // namespace grain
