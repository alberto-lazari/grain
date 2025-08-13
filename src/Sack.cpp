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

    if (!pick_hand->has_grains())
    {
        pick_hand = find_available_handful();
        if (!pick_hand) return nullptr;
    }

    return pick_hand->pick(grain_size);
}

void Sack::put_back(void* const grain) noexcept
{
    // TODO
}

Handful* Sack::grab_new_handful() noexcept
{
    Handful new_handful;
    if (!new_handful.grab(grain_size, handful_size)) return nullptr;

    load.push_back(std::move(new_handful));

    // Return the vector's internal handful, since the new one was a local variable
    return &load.back();
}

Handful* Sack::find_available_handful() noexcept
{
    // Doesn't make sense to search an empty loot
    if (load.empty()) return grab_new_handful();

    // Start from the current handful and cycle forward
    std::size_t hand_index = pick_hand ? pick_hand - load.data() : 0;
    for (std::size_t i = hand_index + 1; i != hand_index; i = (i + 1) % load.size())
    {
        Handful& handful = load[i];
        if (handful.has_grains()) return &handful;
    }

    // Grab new if couldn't find a suitable one
    return grab_new_handful();
}

} // namespace grain
