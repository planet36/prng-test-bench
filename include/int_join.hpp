// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// Join smaller unsigned ints into larger unsigned ints
/**
* \file
* \author Steven Ward
*/

#pragma once

#include "int_bytes.hpp"

#include <array>
#include <bit>
#include <concepts>
#include <utility>

template <std::unsigned_integral T>
[[nodiscard]] constexpr auto
int_join(const T hi, const T lo)
{
    using T2 = next_larger<T>;

    if constexpr (std::endian::native == std::endian::little)
        return std::bit_cast<T2>(std::array<T, 2>{lo, hi});
    else if constexpr (std::endian::native == std::endian::big)
        return std::bit_cast<T2>(std::array<T, 2>{hi, lo});
    else // mixed endian
        std::unreachable();
}
