#include "grain/Grain.h"

#include "grain/Granary.h"

namespace grain
{

using TheGranary = Granary<>;

void* Grain::operator new(const std::size_t grain_size)
{
    return TheGranary::reach().pick(grain_size);
}

void Grain::operator delete(void* const grain, const std::size_t size) noexcept
{
    TheGranary::reach().put_back(grain, size);
}

} // namespace grain
