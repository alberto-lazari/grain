#include "grain/Sack.h"

#include "grain/Handful.h"

namespace grain
{

void* Sack::pick() noexcept
{
    if (!pick_hand || !pick_hand->has_grains())
    {
        for (Handful& handful : load)
        {
            // Find a handful with some grains in it
            if (handful.has_grains())
            {
                pick_hand = &handful;
                break;
            }
        }

        // If the hand is still empty it's time to grab another handful
        if (!pick_hand || !pick_hand->has_grains())
        {
            Handful new_handful;
            new_handful.grab(grain_size, handful_size);
            pick_hand = &new_handful;
            put_back_hand = &new_handful;
            load.push_back(std::move(new_handful));
        }
    }

    return pick_hand->pick(grain_size);
}

void Sack::put_back(void* const grain) noexcept
{
    // TODO
}

} // namespace grain
