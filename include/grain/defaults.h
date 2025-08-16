#pragma once

#include <cstddef>

// Maximum size in bytes of objects that can fit in a Grain
constexpr std::size_t DEFAULT_MAX_GRAIN_SIZE = 64;

// Default number of bytes that get allocated for a handful.
// Big values of grain size may result in huge Handfuls, so this is an upper limit.
// However, small values of grain size may result in Handful::Count overflows,
// so this is not always enforced.
constexpr std::size_t DEFAULT_HANDFUL_SIZE = 4096;
