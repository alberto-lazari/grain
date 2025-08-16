#include "grain/Grain.h"
#include "grain/Granary.h"

#include <cassert>

namespace grain
{

using TheGranary = Granary<>;

void* Grain::operator new(const std::size_t grain_size)
{
    void* const p = TheGranary::reach().pick(grain_size);
    if (!p) throw std::bad_alloc();
    return p;
}

void Grain::operator delete(void* const grain, const std::size_t size) noexcept
{
    bool all_well = TheGranary::reach().put_back(grain, size);
    assert(all_well);
}

} // namespace grain
