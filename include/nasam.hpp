// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// nasam PRNG
/**
* \file
* \author Steven Ward
* \sa http://mostlymangling.blogspot.com/2020/01/nasam-not-another-strange-acronym-mixer.html
* \sa https://github.com/martinus/better-faster-stronger-mixer/blob/master/include/mixer/nasam.h
*/

#pragma once

#include "def_urbg_class.hpp"

#include <bit>
#include <cstdint>

DEF_URBG_CLASS(nasam, uint64_t, uint64_t)
{
    static constexpr uint64_t M1 = 0x9e6c63d0676a9a99; // not prime (popcount = 33)
    static constexpr uint64_t M2 = 0x9e6d62d06f6a9a9b; // not prime (popcount = 35)
    static_assert(M1 & 1, "must be odd");
    static_assert(M2 & 1, "must be odd");

    static constexpr unsigned int R1 = 25;
    static constexpr unsigned int R2 = 47;
    static constexpr unsigned int S1 = 23;
    static constexpr unsigned int S2 = 51;

    auto x = s++; // (SDW)

    x ^= std::rotr(x, R1) ^ std::rotr(x, R2);
    x *= M1;
    x ^= (x >> S1) ^ (x >> S2);
    x *= M2;
    x ^= (x >> S1) ^ (x >> S2);

    return x;
}
