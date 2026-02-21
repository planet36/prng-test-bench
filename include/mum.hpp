// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// MUltiply and Mix ("MUM") functions
/**
* \file
* \author Steven Ward
* \sa https://github.com/vnmakarov/mum-hash
* \sa https://github.com/vnmakarov/mum-hash/blob/master/mum.h#L97
* \sa https://github.com/vnmakarov/mum-hash/blob/master/mum-prng.h#L74
* \sa https://github.com/gcc-mirror/gcc/blob/master/gcc/config/i386/bmi2intrin.h#L83
*/

#pragma once

#include "int_bytes.hpp"

#include <concepts>
#include <limits>

/// Multiply \a hi and \a lo and return the high and low parts of the product
template <std::unsigned_integral T>
constexpr void
mul(T& hi, T& lo)
{
    // When T is uint64_t, this is optimized the same as _mulx_u64.
    using T2 = next_larger<T>;
    const T2 r = static_cast<T2>(hi) * static_cast<T2>(lo);
    hi = static_cast<T>(r >> std::numeric_limits<T>::digits);
    lo = static_cast<T>(r);
}

/// Multiply \a a and \a b and return the XOR of the high and low parts of the product
template <std::unsigned_integral T>
constexpr T
mumx(T a, T b)
{
    mul(a, b);
    return a ^ b;
}

/// Multiply \a a and \a b and return the sum of the high and low parts of the product
template <std::unsigned_integral T>
constexpr T
muma(T a, T b)
{
    mul(a, b);
    return a + b;
}

/// Multiply \a a and \a b and return the difference of the high and low parts of the product
template <std::unsigned_integral T>
constexpr T
mums(T a, T b)
{
    mul(a, b);
    return a - b;
}
