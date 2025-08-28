// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// MRG32k3a PRNG
/**
* \file
* \author Steven Ward
* \sa https://github.com/vigna/MRG32k3a/blob/master/MRG32k3a.c
*/

#pragma once

#include "def_urbg_class.hpp"
#include "xxhprimes.hpp"

#include <cstdint>

DEF_URBG_CLASS(MRG32k3a, uint64_t, uint64_t)
{
    static constexpr uint64_t inc = xxh_prime64[0]; // inc=1 yields failures
    static_assert(inc & 1, "must be odd");
    static constexpr uint64_t M1 = 0xbf58476d1ce4e5b9; // not prime (popcount = 36)
    static constexpr uint64_t M2 = 0x94d049bb133111eb; // not prime (popcount = 29)
    static_assert(M1 & 1, "must be odd");
    static_assert(M2 & 1, "must be odd");
    static constexpr unsigned int S1 = 30;
    static constexpr unsigned int S2 = 27;
    static constexpr unsigned int S3 = 32;

    auto x = s;
    s += inc; // (SDW)

    x = (x ^ (x >> S1)) * M1;
    x = (x ^ (x >> S2)) * M2;
    // This is necessary in Java to guarantee that we return a positive number
    //return (x >> 1) ^ (x >> 32);
    return x ^ (x >> S3); // (SDW)
}
