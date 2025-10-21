// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// squares32, squares64 PRNGs
/**
* \file
* \author Steven Ward
* \sa https://arxiv.org/pdf/2004.06278.pdf
* \sa https://www.arxiv-vanity.com/papers/2004.06278/
*/

#pragma once

#include "abstract_urbg_class.hpp"
#include "xxhprimes.hpp"

#include <bit>
#include <cstdint>

DEF_URBG_SUBCLASS(squares32, uint64_t, uint32_t)

/// prepare the initial state
void
squares32::init()
{}

squares32::result_type
squares32::next()
{
    constexpr uint64_t key = XXH_PRIME64_1;
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
    constexpr uint64_t key = XXH_PRIME64_1;
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
