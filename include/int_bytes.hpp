// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// compile-time typing of integers based on a given byte size or bit size
/**
* \file
* \author Steven Ward
* \sa https://peter.bloomfield.online/using-cpp-templates-for-size-based-type-selection/
*/

#pragma once

#include <concepts>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <utility>

template <unsigned int N>
using int_bytes = std::conditional_t<N <= sizeof(std::int8_t), std::int8_t,
                  std::conditional_t<N <= sizeof(std::int16_t), std::int16_t,
                  std::conditional_t<N <= sizeof(std::int32_t), std::int32_t,
                  std::conditional_t<N <= sizeof(std::int64_t), std::int64_t,
#if defined(__SIZEOF_INT128__)
                  std::conditional_t<N <= sizeof(__int128_t), __int128_t, void>
#else
                  void
#endif
>>>>;

template <unsigned int N>
using uint_bytes = std::conditional_t<N <= sizeof(std::uint8_t), std::uint8_t,
                   std::conditional_t<N <= sizeof(std::uint16_t), std::uint16_t,
                   std::conditional_t<N <= sizeof(std::uint32_t), std::uint32_t,
                   std::conditional_t<N <= sizeof(std::uint64_t), std::uint64_t,
#if defined(__SIZEOF_INT128__)
                   std::conditional_t<N <= sizeof(__uint128_t), __uint128_t, void>
#else
                   void
#endif
>>>>;

/**
* \sa https://en.cppreference.com/w/cpp/types/numeric_limits/digits
*/
template <unsigned int N>
using int_bits = std::conditional_t<N <= std::numeric_limits<std::int8_t>::digits, std::int8_t,
                 std::conditional_t<N <= std::numeric_limits<std::int16_t>::digits, std::int16_t,
                 std::conditional_t<N <= std::numeric_limits<std::int32_t>::digits, std::int32_t,
                 std::conditional_t<N <= std::numeric_limits<std::int64_t>::digits, std::int64_t,
#if defined(__SIZEOF_INT128__)
                 std::conditional_t<N <= std::numeric_limits<__int128_t>::digits, __int128_t, void>
#else
                 void
#endif
>>>>;

/**
* \sa https://en.cppreference.com/w/cpp/types/numeric_limits/digits
*/
template <unsigned int N>
using uint_bits = std::conditional_t<N <= std::numeric_limits<std::uint8_t>::digits, std::uint8_t,
                  std::conditional_t<N <= std::numeric_limits<std::uint16_t>::digits, std::uint16_t,
                  std::conditional_t<N <= std::numeric_limits<std::uint32_t>::digits, std::uint32_t,
                  std::conditional_t<N <= std::numeric_limits<std::uint64_t>::digits, std::uint64_t,
#if defined(__SIZEOF_INT128__)
                  std::conditional_t<N <= std::numeric_limits<__uint128_t>::digits, __uint128_t, void>
#else
                  void
#endif
>>>>;

// For libstdc++, you must compile in gnu++XX mode to get the correct 128-bit
// integer limits/traits.
// https://quuxplusone.github.io/blog/2019/02/28/is-int128-integral/

template <std::integral T>
using next_smaller = std::conditional_t<std::is_signed_v<T>,
                                        int_bytes<sizeof(T) / 2>,
                                        uint_bytes<sizeof(T) / 2>>;

template <std::integral T>
using next_larger = std::conditional_t<std::is_signed_v<T>,
                                       int_bytes<sizeof(T) * 2>,
                                       uint_bytes<sizeof(T) * 2>>;

template <typename T, typename T2>
using smaller_of = std::conditional_t<sizeof(T) <= sizeof(T2), T, T2>;

template <typename T, typename T2>
using larger_of = std::conditional_t<sizeof(T) >= sizeof(T2), T, T2>;

/**
* \return \a n cast to the next larger integer type
*/
template <std::integral T>
constexpr auto
widen(const T n)
{
    using T2 = next_larger<T>;
    return static_cast<T2>(n);
}

/**
* \return \a n cast to the next smaller integer type
* Values outside the bounds of the return type are clamped.
* \sa https://en.wikipedia.org/wiki/Saturation_arithmetic
*/
template <std::integral T>
constexpr auto
narrow(const T n)
{
    using T2 = next_smaller<T>;

    if constexpr (std::is_signed_v<T>)
    {
        if (std::cmp_less(n, std::numeric_limits<T2>::min()))
            return std::numeric_limits<T2>::min();
    }

    if (std::cmp_greater(n, std::numeric_limits<T2>::max()))
        return std::numeric_limits<T2>::max();

    return static_cast<T2>(n);
}
