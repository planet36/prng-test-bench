// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Join smaller unsigned ints into larger unsigned ints
/**
* \file
* \author Steven Ward
*/

#pragma once

#include "int_bytes.hpp"
#include "int_parts_union.hpp"

#include <bit>
#include <concepts>
#include <limits>

template <std::unsigned_integral T>
constexpr auto
int_join(const T hi, const T lo)
{
    // https://en.cppreference.com/w/cpp/language/if#Consteval_if
    if consteval
    {
        using T2 = next_larger<T>;
        return (static_cast<T2>(hi) << std::numeric_limits<T>::digits) | lo;
    }
    else
    {
        if constexpr (std::endian::native == std::endian::little)
            return int_parts<T>{.parts{lo, hi}}.whole;
        else if constexpr (std::endian::native == std::endian::big)
            return int_parts<T>{.parts{hi, lo}}.whole;
        else // mixed endian
            __builtin_unreachable();
    }
}
