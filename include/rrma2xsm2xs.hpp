// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// rrma2xsm2xs PRNG
/**
* \file
* \author Steven Ward
* \sa http://mostlymangling.blogspot.com/2020/01/nasam-not-another-strange-acronym-mixer.html
*/

#pragma once

#include "abstract_urbg_class.hpp"

#include <bit>
#include <cstdint>

/// GoldenRatio scaled to uint64_t
/**
* https://www.wolframalpha.com/input?i=IntegerString%5BFloor%5B%282**64%29*Frac%5BGoldenRatio%5D%5D%2C16%5D
*/
#define GOLDEN_RATIO_64 UINT64_C(0x9e3779b97f4a7c15) // not prime (popcount = 38)

DEF_URBG_SUBCLASS(rrma2xsm2xs, uint64_t, uint64_t)

/// prepare the initial state
void rrma2xsm2xs::init()
{
}

rrma2xsm2xs::result_type rrma2xsm2xs::next()
{
    constexpr uint64_t M1 = 0x9e6c63d0676a9a99; // not prime (popcount = 33)
    constexpr uint64_t M2 = 0x9e6d62d06f6a9a9b; // not prime (popcount = 35)
    static_assert(M1 & 1, "must be odd");
    static_assert(M2 & 1, "must be odd");

    constexpr auto C = GOLDEN_RATIO_64; // (SDW)

    constexpr unsigned int R1 = 25;
    constexpr unsigned int R2 = 47;
    constexpr unsigned int S1 = 23;
    constexpr unsigned int S2 = 51;

    s ^= std::rotr(s, R1) ^ std::rotr(s, R2);
    s = s * M1 + C; // Avoids trivial fixpoint at 0.
    s ^= (s >> S1) ^ (s >> S2);
    s *= M2;
    s ^= (s >> S1) ^ (s >> S2);

    return s;
}
