// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// MRG32k3a PRNG
/**
* \file
* \author Steven Ward
* \sa https://github.com/vigna/MRG32k3a/blob/master/MRG32k3a.c
*/

#pragma once

#include "abstract_urbg_class.hpp"
#include "xxhprimes.hpp"

#include <cstdint>

DEF_URBG_SUBCLASS(MRG32k3a, uint64_t, uint64_t)

/// prepare the initial state
void MRG32k3a::init()
{
}

MRG32k3a::result_type MRG32k3a::next()
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
