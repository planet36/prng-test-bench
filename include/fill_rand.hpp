// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// Fill a buffer with random bytes
/**
* \file
* \author Steven Ward
*/

#pragma once

#include <ranges>
#include <span>
#include <type_traits>

#if defined(_GLIBCXX_HAVE_ARC4RANDOM)

#include <cstdlib>

// arc4random_buf
// https://www.gnu.org/software/libc/manual/html_node/High-Quality-Random.html

template <typename T>
requires (!std::ranges::contiguous_range<T> && std::is_trivially_copyable_v<T>)
void
fill_rand(T& x) noexcept
{
    arc4random_buf(&x, sizeof(T));
}

template <std::ranges::contiguous_range T>
requires std::is_trivially_copyable_v<std::ranges::range_value_t<T>>
void
fill_rand(T& container) noexcept
{
    auto sp = std::span{container};
    arc4random_buf(std::data(sp), sp.size_bytes());
}

#elif defined(_GLIBCXX_HAVE_GETENTROPY)

#include <algorithm>
#include <cerrno>
#include <cstddef>
#include <system_error>
#include <unistd.h>

// getentropy
// https://www.gnu.org/software/libc/manual/html_node/Unpredictable-Bytes.html
// Max num bytes allowed is 256
inline constexpr size_t getentropy_max_len = 256;

template <typename T>
requires (!std::ranges::contiguous_range<T> && std::is_trivially_copyable_v<T> &&
          (sizeof(T) <= getentropy_max_len))
void
fill_rand(T& x)
{
    if (getentropy(&x, sizeof(T)) < 0)
    {
        throw std::system_error(std::make_error_code(std::errc{errno}), "getentropy");
    }
}

template <std::ranges::contiguous_range T>
requires std::is_trivially_copyable_v<std::ranges::range_value_t<T>>
void
fill_rand(T& container)
{
    std::span<std::byte> sp = std::as_writable_bytes(std::span{container});

    while (!sp.empty())
    {
        const size_t len = std::min(sp.size(), getentropy_max_len);

        if (getentropy(sp.data(), len) < 0)
        {
            throw std::system_error(std::make_error_code(std::errc{errno}), "getentropy");
        }

        sp = sp.subspan(len);
    }
}

#else

#include "int_join.hpp"

#include <array>
#include <concepts>
#include <random>
#include <utility>
#include <vector>

template <std::unsigned_integral T>
void
fill_rand(T& x)
{
    static std::random_device rd;

    // std::random_device::result_type is unsigned int
    // https://en.cppreference.com/w/cpp/numeric/random/random_device
    if constexpr (sizeof(T) <= sizeof(typename std::random_device::result_type))
        x = rd();
    else if constexpr (sizeof(T) <= 2 * sizeof(typename std::random_device::result_type))
        x = int_join(rd(), rd());
    else
        std::unreachable();
}

template <std::unsigned_integral T, size_t N>
void
fill_rand(std::array<T, N>& container)
{
    static std::random_device rd;

    for (T& x : container)
    {
        // std::random_device::result_type is unsigned int
        // https://en.cppreference.com/w/cpp/numeric/random/random_device
        if constexpr (sizeof(T) <= sizeof(typename std::random_device::result_type))
            x = rd();
        else if constexpr (sizeof(T) <= 2 * sizeof(typename std::random_device::result_type))
            x = int_join(rd(), rd());
        else
            std::unreachable();
    }
}

template <std::unsigned_integral T>
void
fill_rand(std::vector<T>& container)
{
    static std::random_device rd;

    for (T& x : container)
    {
        // std::random_device::result_type is unsigned int
        // https://en.cppreference.com/w/cpp/numeric/random/random_device
        if constexpr (sizeof(T) <= sizeof(typename std::random_device::result_type))
            x = rd();
        else if constexpr (sizeof(T) <= 2 * sizeof(typename std::random_device::result_type))
            x = int_join(rd(), rd());
        else
            std::unreachable();
    }
}

#endif
