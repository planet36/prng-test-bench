// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// murmurhash3_32, murmurhash3 PRNGs
/**
* \file
* \author Steven Ward
* \sa https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp
*/

#pragma once

#include "abstract_urbg_class.hpp"
#include "xxhprimes.hpp"

#include <cstdint>

// https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp#L68
DEF_URBG_SUBCLASS(murmurhash3_32, uint32_t, uint32_t)

/// prepare the initial state
void murmurhash3_32::init()
{
}

murmurhash3_32::result_type murmurhash3_32::next()
{
    constexpr uint32_t inc = xxh_prime32[0]; // inc=1 yields failures
    static_assert(inc & 1, "must be odd");

    constexpr uint32_t M1 = 0x85ebca6b; // not prime (popcount = 18)
    constexpr uint32_t M2 = 0xc2b2ae35; // not prime (popcount = 16)
    static_assert(M1 & 1, "must be odd");
    static_assert(M2 & 1, "must be odd");
    constexpr unsigned int S1 = 16;
    constexpr unsigned int S2 = 13;
    constexpr unsigned int S3 = 16;

    auto x = s;
    s += inc; // (SDW)

    x ^= x >> S1; x *= M1;
    x ^= x >> S2; x *= M2;
    x ^= x >> S3;
    return x;
}

// https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp#L81
DEF_URBG_SUBCLASS(murmurhash3, uint64_t, uint64_t)

/// prepare the initial state
void murmurhash3::init()
{
}

murmurhash3::result_type murmurhash3::next()
{
    constexpr uint64_t inc = xxh_prime64[0]; // inc=1 yields failures
    static_assert(inc & 1, "must be odd");

    constexpr uint64_t M1 = 0xff51afd7ed558ccd; // prime (popcount = 41)
    constexpr uint64_t M2 = 0xc4ceb9fe1a85ec53; // not prime (popcount = 35)
    static_assert(M1 & 1, "must be odd");
    static_assert(M2 & 1, "must be odd");
    constexpr unsigned int S1 = 33;
    constexpr unsigned int S2 = 33;
    constexpr unsigned int S3 = 33;

    auto x = s;
    s += inc; // (SDW)

    x ^= x >> S1; x *= M1;
    x ^= x >> S2; x *= M2;
    x ^= x >> S3;
    return x;
}
