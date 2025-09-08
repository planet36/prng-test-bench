// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// rrmxmx PRNG
/**
* \file
* \author Steven Ward
* \sa https://mostlymangling.blogspot.com/2019/01/better-stronger-mixer-and-test-procedure.html
*/

#pragma once

#include "abstract_urbg_class.hpp"

#include <bit>
#include <cstdint>

// Old mixer, my rrmxmx
DEF_URBG_SUBCLASS(rrmxmx, uint64_t, uint64_t)

/// prepare the initial state
void rrmxmx::init()
{
}

rrmxmx::result_type rrmxmx::next()
{
    constexpr uint64_t M1 = 0x9fb21c651e98df25; // prime (popcount = 34)
    constexpr uint64_t M2 = 0x9fb21c651e98df25; // prime (popcount = 34)
    static_assert(M1 & 1, "must be odd");
    static_assert(M2 & 1, "must be odd");

    auto x = s++; // (SDW)

    x ^= std::rotr(x, 49) ^ std::rotr(x, 24);
    x *= M1;
    x ^= x >> 28;
    x *= M2;
    x ^= x >> 28;
    return x;
}
