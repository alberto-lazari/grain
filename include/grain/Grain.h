#pragma once

#include <cstddef>

namespace grain
{

class Grain
{
public:
    static void* operator new(const std::size_t grain_size);
    static void operator delete(void* const grain, const std::size_t size) noexcept;

    // Wouldn't make a difference. The custom delete would still leak part of the object,
    // since it relies on an explicit size. There's no way to get the actual runtime size.
    // A solution would be to require every SubGrain to override a virtual get_size(),
    // but it's tedious and error prone.
    // virtual ~Grain() = default;
};

} // namespace grain
