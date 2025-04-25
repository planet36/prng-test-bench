// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// ettinger_mixer PRNG
/**
\file
\author Steven Ward
*/

#pragma once

#include "def_urbg_class.hpp"
#include "scaled-const.hpp"

#include <bit>
#include <cstdint>

/*
adapted from:
https://mostlymangling.blogspot.com/2019/01/better-stronger-mixer-and-test-procedure.html?showComment=1548399531434#c7135800794322093693
*/
DEF_URBG_CLASS(ettinger_mixer, uint64_t, uint64_t)
{
    static constexpr uint64_t X1 = 0xdb4f0b9175ae2165; // not prime (popcount = 33)
    static_assert(X1 & 1, "must be odd");
    static constexpr uint64_t M1 = 0x4823a80b2006e21b; // prime (popcount = 22)
    static constexpr uint32_t M2 = 0x81383173; // prime (popcount = 13)
    static_assert(M1 & 1, "must be odd");
    static_assert(M2 & 1, "must be odd");
    static constexpr unsigned int R1 = 12;
    static constexpr unsigned int R2 = 21;
    static constexpr unsigned int S1 = 28;

    auto x = s++; // (SDW)

    x ^= X1;
    x *= M1;
    x ^= std::rotr(x, R1) ^ std::rotl(x, R2) ^ GoldenRatio_64;
    x *= M2;
    x ^= x >> S1;
    return x;
}
