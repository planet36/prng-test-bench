// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// nasam PRNG
/**
* \file
* \author Steven Ward
* \sa http://mostlymangling.blogspot.com/2020/01/nasam-not-another-strange-acronym-mixer.html
* \sa https://github.com/martinus/better-faster-stronger-mixer/blob/master/include/mixer/nasam.h
*/

#pragma once

#include "abstract_urbg_class.hpp"

#include <bit>
#include <cstdint>

DEF_URBG_SUBCLASS(nasam, uint64_t, uint64_t)

/// prepare the initial state
void
nasam::init()
{}

nasam::result_type
nasam::next()
{
    constexpr uint64_t M1 = 0x9e6c63d0676a9a99; // not prime (popcount = 33)
    constexpr uint64_t M2 = 0x9e6d62d06f6a9a9b; // not prime (popcount = 35)
    static_assert(M1 & 1, "must be odd");
    static_assert(M2 & 1, "must be odd");

    auto x = s++; // (SDW)

    x ^= std::rotr(x, 25) ^ std::rotr(x, 47);
    x *= M1;
    x ^= (x >> 23) ^ (x >> 51);
    x *= M2;
    x ^= (x >> 23) ^ (x >> 51);

    return x;
}
