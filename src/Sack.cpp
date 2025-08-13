#include "grain/Sack.h"

#include "grain/Handful.h"

namespace grain
{

void* Sack::pick() noexcept
{
    if (!pick_hand)
    {
        pick_hand = load.empty()
            ? grab_new_handful()
            : load.data();
        if (!pick_hand) return nullptr;
    }

    if (pick_hand->is_empty())
    {
        pick_hand = find_with_grains();
        if (!pick_hand) return nullptr;
    }

    return pick_hand->pick(grain_size);
}

bool Sack::put_back(void* const grain) noexcept
{
    if (!put_back_hand)
    {
        if (pick_hand) put_back_hand = pick_hand;
        else return false;
    }

    if (put_back_hand->is_full())
    {
        Handful* last = &load.back();

        // Remove a handful when two are full
        if (last != put_back_hand && last->is_full())
        {
            load.pop_back();

            // Update invalidated pointers
            if (last == pick_hand) pick_hand = put_back_hand;
            last = &load.back();
        }

        // Keep the full hand as last
        if (last != put_back_hand) std::swap(*put_back_hand, *last);
        // Search only if the full hand was already last
        else if (!(put_back_hand = find_with_room())) return false;
    }

    return put_back_hand->put_back(grain, grain_size);
}

Handful* Sack::grab_new_handful() noexcept
{
    Handful new_handful;
    if (!new_handful.grab(grain_size, handful_size)) return nullptr;

    load.push_back(std::move(new_handful));

    // Update both pointers, since `push_back` may invalidate them
    return pick_hand = put_back_hand = &load.back();
}

Handful* Sack::find_with_grains() noexcept
{
    // Doesn't make sense to search an empty loot
    if (load.empty()) return grab_new_handful();

    // Start from the current handful
    std::size_t hand_index = pick_hand ? pick_hand - load.data() : 0;

    // Cycle forwards
    for (
        std::size_t i = (hand_index + 1) % load.size();
        i != hand_index;
        i = (i + 1) % load.size())
    {
        Handful& handful = load[i];
        if (!handful.is_empty()) return &handful;
    }

    // Grab new if couldn't find a suitable one
    return grab_new_handful();
}

Handful* Sack::find_with_room() noexcept
{
    // Doesn't make sense to search an empty loot
    if (load.empty()) return nullptr;

    // Start from the current handful
    std::size_t hand_index = pick_hand ? pick_hand - load.data() : 0;
    std::size_t load_size = load.size();

    // Cycle both forwards and backwards
    for (
        std::size_t f = (hand_index + 1) % load_size,
                    b = (hand_index + load_size - 1) % load_size;

        f != b;

        f = (f + 1) % load_size,
        b = (b + load_size - 1) % load_size)
    {
        Handful* handful = nullptr;
        if (!(handful = &load[f])->is_full()) return handful;
        if (!(handful = &load[b])->is_full()) return handful;
    }

    // Grab new if couldn't find a suitable one
    return nullptr;
}

} // namespace grain
