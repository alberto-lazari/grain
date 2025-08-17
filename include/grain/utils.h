#pragma once

#include <cstddef>
#include <limits>
#include <new>
#include <utility>

namespace grain
{

void* Malloc(const std::size_t size) noexcept;
bool Free(void* const p, const std::size_t size) noexcept;

template <typename T, typename... Args>
T* New(const std::nothrow_t&, Args&&... args) noexcept
{
    void* const vp = Malloc(sizeof(T));
    if (!vp) return nullptr;

    T* const p = reinterpret_cast<T*>(vp);
    return new (p) T(std::forward<Args>(args)...);
}

template <typename T, typename... Args>
T* New(Args&&... args)
{
    if (T* const p = New<T>(std::nothrow, std::forward<Args>(args)...)) return p;
    throw std::bad_alloc();
}

template <typename T>
bool Delete(T* const p) noexcept
{
    if (!p) return false;

    p->~T();
    return Free(p, sizeof(T));
}


template <typename T, typename... Args>
T* New_Array(const std::size_t count, Args&&... args)
{
    if (count == 0) return nullptr;

    const std::size_t max_count =
        (std::numeric_limits<std::size_t>::max() - sizeof(count)) / sizeof(T);
    if (count > max_count) throw std::bad_array_new_length();

    const std::size_t size = sizeof(T) * count;

    // Store array size before the actual data
    void* const p = Malloc(sizeof(count) + size);
    std::size_t* const size_ptr = reinterpret_cast<std::size_t*>(p);
    *size_ptr = size;

    // Initialize values
    T* const values = reinterpret_cast<T*>(size_ptr + 1);
    for (std::size_t i = 0; i < count; ++i)
    {
        try { new (&values[i]) T(std::forward<Args>(args)...); }
        catch (...)
        {
            // Revert initialization
            for (std::size_t j = 0; j < i; ++j) values[j].~T();
            Free(size_ptr, size);
            throw;
        }
    }

    return values;
}

template <typename T, typename... Args>
T* New_Array(const std::nothrow_t&, const std::size_t count, Args&&... args) noexcept
{
    if (count == 0) return nullptr;

    const std::size_t max_count =
        (std::numeric_limits<std::size_t>::max() - sizeof(count)) / sizeof(T);
    if (count > max_count) return nullptr;

    const std::size_t size = sizeof(T) * count;

    // Store array size before the actual data
    void* const p = Malloc(sizeof(count) + size);
    std::size_t* const size_ptr = reinterpret_cast<std::size_t*>(p);
    *size_ptr = size;

    // Initialize values
    T* const values = reinterpret_cast<T*>(size_ptr + 1);
    for (std::size_t i = 0; i < count; ++i)
    {
        try { new (&values[i]) T(std::forward<Args>(args)...); }
        catch (...)
        {
            // Revert initialization
            for (std::size_t j = 0; j < i; ++j) values[j].~T();
            Free(size_ptr, size);
            return nullptr;
        }
    }

    return values;
}

template <typename T>
bool Delete_Array(T* const p) noexcept
{
    if (!p) return false;

    // Recover count from size, stored before the data
    std::size_t* const size_ptr = reinterpret_cast<std::size_t*>(p) - 1;
    const std::size_t size = *size_ptr;
    const std::size_t count = size / sizeof(T);

    // Destroy objects
    for (std::size_t i = 0; i < count; ++i) p[i].~T();

    // Deallocate memory
    return Free(size_ptr, sizeof(size) + size);
}

} // namespace grain
