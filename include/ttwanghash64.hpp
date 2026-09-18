// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// ttwanghash64 PRNG
/**
* \file
* \author Steven Ward
* \sa https://web.archive.org/web/20121102023700/http://www.concentric.net/~ttwang/tech/inthash.htm
*/

#pragma once

#include "urbg_base_class.hpp"
#include "wyprimes.hpp"

#include <cstdint>

DEF_URBG_SUBCLASS(ttwanghash64, uint64_t, uint64_t)

/// Prepare the initial state
void
ttwanghash64::init()
{}

// adapted from hash64shift
ttwanghash64::result_type
ttwanghash64::next()
{
    constexpr uint64_t inc = wyprimes::_wyp[0]; // inc=1 yields failures
    static_assert(inc & 1, "must be odd");

    auto x = s;
    s += inc; // (SDW)

    x = (~x) + (x << 21); // x = (x << 21) - x - 1;
    x ^= (x >> 24);
    x += (x << 3) + (x << 8); // x * 265
    x ^= (x >> 14);
    x += (x << 2) + (x << 4); // x * 21
    x ^= (x >> 28);
    x += (x << 31);
    return x;
}
