#include "grain/Sack.h"

namespace grain
{

Sack::Sack(const std::size_t handful_capacity, const std::size_t grain_size) noexcept
    : _grain_size(grain_size)
    , _handful_capacity(handful_capacity < Handful::MAX_HAND_CAPACITY
            ? handful_capacity : Handful::MAX_HAND_CAPACITY)
{
}

void* Sack::pick() noexcept
{
    if (!_pick_hand)
    {
        _pick_hand = _load.empty()
            ? grab_new_handful()
            : _load.data();
        if (!_pick_hand) return nullptr;
    }

    if (_pick_hand->is_empty())
    {
        _pick_hand = find_with_grains();
        if (!_pick_hand) return nullptr;
    }

    return _pick_hand->pick(_grain_size);
}

bool Sack::put_back(void* const grain) noexcept
{
    if (!_put_back_hand)
    {
        if (_pick_hand) _put_back_hand = _pick_hand;
        else return false;
    }

    if (_put_back_hand->is_full())
    {
        Handful* last = &_load.back();

        // Remove a handful when two are full
        if (last != _put_back_hand && last->is_full())
        {
            _load.pop_back();

            // Update invalidated pointers
            if (last == _pick_hand) _pick_hand = _put_back_hand;
            last = &_load.back();
        }

        // Keep the full hand as last
        if (last != _put_back_hand) std::swap(*_put_back_hand, *last);
        // Search only if the full hand was already last
        else if (!( _put_back_hand = find_with_room() )) return false;
    }

    return _put_back_hand->put_back(grain, _grain_size);
}

Handful* Sack::grab_new_handful() noexcept
{
    Handful new_handful;
    if (!new_handful.grab(_grain_size, _handful_capacity)) return nullptr;

    _load.push_back(std::move(new_handful));

    // Update both pointers, since `push_back` may invalidate them
    return _pick_hand = _put_back_hand = &_load.back();
}

Handful* Sack::find_with_grains() noexcept
{
    // Doesn't make sense to search an empty load
    if (_load.empty()) return grab_new_handful();

    // Start from the current handful
    std::size_t hand_index = _pick_hand ? _pick_hand - _load.data() : 0;

    // Cycle forwards
    for (
        std::size_t i = (hand_index + 1) % _load.size();
        i != hand_index;
        i = (i + 1) % _load.size())
    {
        Handful& handful = _load[i];
        if (!handful.is_empty()) return &handful;
    }

    // Grab new if couldn't find a suitable one
    return grab_new_handful();
}

Handful* Sack::find_with_room() noexcept
{
    // Doesn't make sense to search an empty load
    if (_load.empty()) return nullptr;

    // Start from the current handful
    std::size_t hand_index = _pick_hand ? _pick_hand - _load.data() : 0;
    std::size_t load_size = _load.size();

    // Cycle both forwards and backwards
    for (
        std::size_t f = (hand_index + 1) % load_size,
                    b = (hand_index + load_size - 1) % load_size;

        f != b;

        f = (f + 1) % load_size,
        b = (b + load_size - 1) % load_size)
    {
        Handful* handful = nullptr;
        if (!( handful = &_load[f] )->is_full()) return handful;
        if (!( handful = &_load[b] )->is_full()) return handful;
    }

    // Grab new if couldn't find a suitable one
    return nullptr;
}

} // namespace grain
