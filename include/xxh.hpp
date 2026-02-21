// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// PRNGs inspired by XXH avalange functions
/**
* \file
* \author Steven Ward
* \sa https://www.romu-random.org/code.c
*/

#pragma once

#include "abstract_urbg_class.hpp"
#include "xxhprimes.hpp"

#include <bit>
#include <cstdint>

DEF_URBG_SUBCLASS(xxh32_avalanche, uint32_t, uint32_t)

/// prepare the initial state
void
xxh32_avalanche::init()
{}

// https://github.com/Cyan4973/xxHash/blob/release/xxhash.h#L2985
xxh32_avalanche::result_type
xxh32_avalanche::next()
{
    auto x = s;
    s += XXH_PRIME32_1; // (SDW)

    x ^= x >> 15;
    x *= XXH_PRIME32_2;
    x ^= x >> 13;
    x *= XXH_PRIME32_3;
    x ^= x >> 16;
    return x;
}

DEF_URBG_SUBCLASS(xxh64_avalanche, uint64_t, uint64_t)

/// prepare the initial state
void
xxh64_avalanche::init()
{}

// https://github.com/Cyan4973/xxHash/blob/release/xxhash.h#L3503
xxh64_avalanche::result_type
xxh64_avalanche::next()
{
    auto x = s;
    s += XXH_PRIME64_1; // (SDW)

    x ^= x >> 33;
    x *= XXH_PRIME64_2;
    x ^= x >> 29;
    x *= XXH_PRIME64_3;
    x ^= x >> 32;
    return x;
}

DEF_URBG_SUBCLASS(xxh3_avalanche, uint64_t, uint64_t)

/// prepare the initial state
void
xxh3_avalanche::init()
{}

// https://github.com/Cyan4973/xxHash/blob/release/xxhash.h#L4583
xxh3_avalanche::result_type
xxh3_avalanche::next()
{
    auto x = s;
    s += XXH_PRIME64_1; // (SDW)

    x ^= x >> 37;
    x *= PRIME_MX1;
    x ^= x >> 32;
    return x;
}

DEF_URBG_SUBCLASS(xxh3_rrmxmx, uint64_t, uint64_t)

/// prepare the initial state
void
xxh3_rrmxmx::init()
{}

// https://github.com/Cyan4973/xxHash/blob/release/xxhash.h#L4596
xxh3_rrmxmx::result_type
xxh3_rrmxmx::next()
{
    auto x = s;
    s += XXH_PRIME64_1; // (SDW)

    /* this mix is inspired by Pelle Evensen's rrmxmx */
    x ^= std::rotl(x, 49) ^ std::rotl(x, 24);
    x *= PRIME_MX2;
    x ^= (x >> 35);
    x *= PRIME_MX2;
    x ^= x >> 28;
    return x;
}
