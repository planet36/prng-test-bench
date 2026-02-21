// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// degski32, degski64 PRNGs
/**
* \file
* \author Steven Ward
*/

#pragma once

#include "abstract_urbg_class.hpp"
#include "xxhprimes.hpp"

#include <cstdint>

// https://gist.github.com/degski/6e2069d6035ae04d5d6f64981c995ec2#file-invertible_hash_functions-hpp-L29
DEF_URBG_SUBCLASS(degski32, uint32_t, uint32_t)

/// prepare the initial state
void
degski32::init()
{}

degski32::result_type
degski32::next()
{
    constexpr uint32_t inc = XXH_PRIME32_1; // inc=1 yields failures
    static_assert(inc & 1, "must be odd");

    constexpr uint32_t M1 = 0x45d9f3b; // not prime (popcount = 17)
    constexpr uint32_t M2 = 0x45d9f3b; // not prime (popcount = 17)
    static_assert(M1 & 1, "must be odd");
    static_assert(M2 & 1, "must be odd");

    auto x = s;
    s += inc; // (SDW)

    x ^= x >> 16;
    x *= M1;
    x ^= x >> 16;
    x *= M2;
    x ^= x >> 16;
    return x;
}

// https://gist.github.com/degski/6e2069d6035ae04d5d6f64981c995ec2#file-invertible_hash_functions-hpp-L43
DEF_URBG_SUBCLASS(degski64, uint64_t, uint64_t)

/// prepare the initial state
void
degski64::init()
{}

degski64::result_type
degski64::next()
{
    constexpr uint64_t inc = XXH_PRIME64_1; // inc=1 yields failures
    static_assert(inc & 1, "must be odd");

    constexpr uint64_t M1 = 0xd6e8feb86659fd93; // not prime (popcount = 39)
    constexpr uint64_t M2 = 0xd6e8feb86659fd93; // not prime (popcount = 39)
    static_assert(M1 & 1, "must be odd");
    static_assert(M2 & 1, "must be odd");

    auto x = s;
    s += inc; // (SDW)

    x ^= x >> 32;
    x *= M1;
    x ^= x >> 32;
    x *= M2;
    x ^= x >> 32;
    return x;
}
