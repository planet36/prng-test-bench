// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// squares32, squares64 PRNGs
/**
* \file
* \author Steven Ward
* \sa https://arxiv.org/pdf/2004.06278.pdf
* \sa https://www.arxiv-vanity.com/papers/2004.06278/
*/

#pragma once

#include "abstract_urbg_class.hpp"

#include <bit>
#include <cstdint>

/// Key for the squares32 and squares64 PRNGs
/**
* This is the first key in keys.h from the squares RNG software download (squaresrngv8), where
* keys.c generated it.  Its upper 8 hex digits are all different, its lower 8 hex digits are all
* different, its ninth digit differs from its eighth, and none of its digits is 0.
*
* The key must be odd, so that multiplying it by the counter reaches all 2^64 values.
*
* \sa http://squaresrng.wixsite.com/rand
* \sa https://arxiv.org/abs/2004.06278
*/
inline constexpr uint64_t squares_key = 0xc8e4fd154ce32f6d; // not prime (popcount = 35)

DEF_URBG_SUBCLASS(squares32, uint64_t, uint32_t)

/// prepare the initial state
void
squares32::init()
{}

squares32::result_type
squares32::next()
{
    constexpr uint64_t key = squares_key;
    static_assert(key & 1, "must be odd");

    ++s; // (SDW)

    auto x = s * key;
    const auto y = x;
    const auto z = y + key;

    // round 1
    x = x * x + y;
    x = std::rotl(x, 32);

    // round 2
    x = x * x + z;
    x = std::rotl(x, 32);

    // round 3
    x = x * x + y;
    x = std::rotl(x, 32);

    // round 4
    return (x * x + z) >> 32;
}

DEF_URBG_SUBCLASS(squares64, uint64_t, uint64_t)

/// prepare the initial state
void
squares64::init()
{}

squares64::result_type
squares64::next()
{
    constexpr uint64_t key = squares_key;
    static_assert(key & 1, "must be odd");

    ++s; // (SDW)

    auto x = s * key;
    const auto y = x;
    const auto z = y + key;

    // round 1
    x = x * x + y;
    x = std::rotl(x, 32);

    // round 2
    x = x * x + z;
    x = std::rotl(x, 32);

    // round 3
    x = x * x + y;
    x = std::rotl(x, 32);

    // round 4
    x = x * x + z;
    const auto t = x;
    x = std::rotl(x, 32);

    // round 5
    return t ^ ((x * x + y) >> 32);
}
