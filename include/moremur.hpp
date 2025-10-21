// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// moremur PRNG
/**
* \file
* \author Steven Ward
* \sa https://mostlymangling.blogspot.com/2019/12/stronger-better-morer-moremur-better.html
*/

#pragma once

#include "abstract_urbg_class.hpp"
#include "golden_ratio.h"

#include <cstdint>

DEF_URBG_SUBCLASS(moremur, uint64_t, uint64_t)

/// prepare the initial state
void
moremur::init()
{}

moremur::result_type
moremur::next()
{
    constexpr uint64_t inc = GOLDEN_RATIO_64;
    static_assert((inc & 1) != 0, "must be odd");

    constexpr uint64_t M1 = 0x3c79ac492ba7b653; // not prime (popcount = 34)
    constexpr uint64_t M2 = 0x1c69b3f74ac4ae35; // not prime (popcount = 34)
    static_assert((M1 & 1) != 0, "must be odd");
    static_assert((M2 & 1) != 0, "must be odd");

    auto x = s;
    s += inc; // (SDW)

    x ^= x >> 27;
    x *= M1;
    x ^= x >> 33;
    x *= M2;
    x ^= x >> 27;
    return x;
}
