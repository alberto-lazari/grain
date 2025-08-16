#pragma once

#include <cstddef>
#include <limits>
#include <new>
#include <utility>

namespace grain
{

void* Malloc(const std::size_t size) noexcept;
bool Free(void* const p) noexcept;
bool Free(void* const p, const std::size_t size) noexcept;

template <typename T, typename... Args>
T* New_Nothrow(Args&&... args) noexcept
{
    void* const vp = Malloc(sizeof(T));
    if (!vp) return nullptr;

    T* const p = reinterpret_cast<T*>(vp);
    return new (p) T(std::forward<Args>(args)...);
}

template <typename T, typename... Args>
T* New(Args&&... args)
{
    T* const p = New_Nothrow<T>(std::forward<Args>(args)...);
    if (!p) throw std::bad_alloc();
    return p;
}

template <typename T>
bool Delete(T* const p) noexcept
{
    if (!p) return false;

    p->~T();
    return Free(p, sizeof(T));
}


template <typename T, typename... Args>
T* New_Array_Nothrow(const std::size_t count, Args&&... args) noexcept
{
    if (count == 0) return nullptr;

    const std::size_t max_count =
        (std::numeric_limits<std::size_t>::max() - sizeof(count)) / sizeof(T);
    if (count > max_count) return nullptr;

    const std::size_t size = sizeof(count) + sizeof(T) * count;

    // Store elements count before the actual data
    void* const p = Malloc(size);
    std::size_t* const count_ptr = reinterpret_cast<std::size_t*>(p);
    *count_ptr = count;

    // Initialize values
    T* const values = reinterpret_cast<T*>(count_ptr + 1);
    for (std::size_t i = 0; i < count; ++i)
    {
        try { new (&values[i]) T(std::forward<Args>(args)...); }
        catch (...)
        {
            // Revert initialization
            for (std::size_t j = 0; j < i; ++j) values[j].~T();
            Free(count_ptr, size);
            return nullptr;
        }
    }

    return values;
}

template <typename T, typename... Args>
T* New_Array(const std::size_t count, Args&&... args)
{
    if (count == 0) return nullptr;

    const std::size_t max_count =
        (std::numeric_limits<std::size_t>::max() - sizeof(count)) / sizeof(T);
    if (count > max_count) throw std::bad_array_new_length();

    const std::size_t size = sizeof(count) + sizeof(T) * count;

    // Store elements count before the actual data
    void* const p = Malloc(size);
    std::size_t* const count_ptr = reinterpret_cast<std::size_t*>(p);
    *count_ptr = count;

    // Initialize values
    T* const values = reinterpret_cast<T*>(count_ptr + 1);
    for (std::size_t i = 0; i < count; ++i)
    {
        try { new (&values[i]) T(std::forward<Args>(args)...); }
        catch (...)
        {
            // Revert initialization
            for (std::size_t j = 0; j < i; ++j) values[j].~T();
            Free(count_ptr, size);
            throw;
        }
    }

    return values;
}

template <typename T>
bool Delete_Array(T* const p) noexcept
{
    if (!p) return false;

    // Recover count stored before the data
    std::size_t* const count_ptr = reinterpret_cast<std::size_t*>(p) - 1;
    const std::size_t count = *count_ptr;

    // Destroy objects
    for (std::size_t i = 0; i < count; ++i) p[i].~T();

    // Deallocate memory
    return Free(count_ptr, sizeof(count) + sizeof(T) * count);
}

} // namespace grain
