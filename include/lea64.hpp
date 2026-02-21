// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// lea64 PRNG
/**
* \file
* \author Steven Ward
* \sa https://mostlymangling.blogspot.com/2021/11/applying-rrc-64-test-to-degski64-and.html
*/

#pragma once

#include "abstract_urbg_class.hpp"
#include "golden_ratio.h"

#include <cstdint>

DEF_URBG_SUBCLASS(lea64, uint64_t, uint64_t)

/// prepare the initial state
void
lea64::init()
{}

lea64::result_type
lea64::next()
{
    constexpr uint64_t inc = GOLDEN_RATIO_64;
    static_assert((inc & 1) != 0, "must be odd");

    constexpr uint64_t M1 = 0xdaba0b6eb09322e3; // not prime (popcount = 32)
    constexpr uint64_t M2 = 0xdaba0b6eb09322e3; // not prime (popcount = 32)
    static_assert((M1 & 1) != 0, "must be odd");
    static_assert((M2 & 1) != 0, "must be odd");

    auto x = s;
    s += inc; // (SDW)

    x ^= x >> 32;
    x *= M1;
    x ^= x >> 32;
    x *= M2;
    x ^= x >> 32;
    return x;
}
