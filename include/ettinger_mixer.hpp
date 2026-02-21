// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// ettinger_mixer PRNG
/**
* \file
* \author Steven Ward
*/

#pragma once

#include "abstract_urbg_class.hpp"
#include "golden_ratio.h"

#include <bit>
#include <cstdint>

/*
adapted from:
https://mostlymangling.blogspot.com/2019/01/better-stronger-mixer-and-test-procedure.html?showComment=1548399531434#c7135800794322093693
*/
DEF_URBG_SUBCLASS(ettinger_mixer, uint64_t, uint64_t)

/// prepare the initial state
void
ettinger_mixer::init()
{}

ettinger_mixer::result_type
ettinger_mixer::next()
{
    constexpr uint64_t X1 = 0xdb4f0b9175ae2165; // not prime (popcount = 33)
    static_assert(X1 & 1, "must be odd");
    constexpr uint64_t M1 = 0x4823a80b2006e21b; // prime (popcount = 22)
    constexpr uint32_t M2 = 0x81383173;         // prime (popcount = 13)
    static_assert(M1 & 1, "must be odd");
    static_assert(M2 & 1, "must be odd");

    auto x = s++; // (SDW)

    x ^= X1;
    x *= M1;
    x ^= std::rotr(x, 12) ^ std::rotl(x, 21) ^ GOLDEN_RATIO_64;
    x *= M2;
    x ^= x >> 28;
    return x;
}
