// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// staffordMix13 PRNG
/**
* \file
* \author Steven Ward
*
* The mixing function comes from the code that seeds MRG32k3a.
*
* \sa https://github.com/vigna/MRG32k3a/blob/master/MRG32k3a.c
* \sa http://zimbry.blogspot.com/2011/09/better-bit-mixing-improving-on.html
*/

#pragma once

#include "abstract_urbg_class.hpp"
#include "xxhprimes.hpp"

#include <cstdint>

DEF_URBG_SUBCLASS(staffordMix13, uint64_t, uint64_t)

/// prepare the initial state
void
staffordMix13::init()
{}

staffordMix13::result_type
staffordMix13::next()
{
    constexpr uint64_t inc = XXH_PRIME64_1; // inc=1 yields failures
    static_assert(inc & 1, "must be odd");
    constexpr uint64_t M1 = 0xbf58476d1ce4e5b9; // not prime (popcount = 36)
    constexpr uint64_t M2 = 0x94d049bb133111eb; // not prime (popcount = 29)
    static_assert(M1 & 1, "must be odd");
    static_assert(M2 & 1, "must be odd");

    auto x = s;
    s += inc; // (SDW)

    x = (x ^ (x >> 30)) * M1;
    x = (x ^ (x >> 27)) * M2;
    // This is necessary in Java to guarantee that we return a positive number
    //return (x >> 1) ^ (x >> 32);
    return x ^ (x >> 32); // (SDW)
}
