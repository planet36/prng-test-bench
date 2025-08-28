// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// mcg128 PRNG
/**
* \file
* \author Steven Ward
* \sa https://www.pcg-random.org/posts/128-bit-mcg-passes-practrand.html
*/

#pragma once

#include "def_urbg_class.hpp"
#include "int_join.hpp"

#include <cstdint>

DEF_URBG_CLASS(mcg128, __uint128_t, uint64_t)
{
    static constexpr __uint128_t M = int_join(0x45a31efc5a35d971UL, 0x261fd0407a968addUL); // not prime (popcount = 64)
    static_assert(M & 1, "must be odd");

    s *= M;
    s += M;
    return s >> 64;
}
