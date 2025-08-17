#include "grain/utils.h"

#include "grain/Granary.h"

namespace grain
{

using TheGranary = Granary<>;

void* Malloc(const std::size_t size) noexcept
{
    return TheGranary::reach().pick(size);
}

bool Free(void* const p, const std::size_t size) noexcept
{
    return TheGranary::reach().put_back(p, size);
}

} // namespace grain
