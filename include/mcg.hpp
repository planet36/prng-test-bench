// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// mcg128 PRNG
/**
* \file
* \author Steven Ward
* \sa https://www.pcg-random.org/posts/128-bit-mcg-passes-practrand.html
*/

#pragma once

#include "abstract_urbg_class.hpp"
#include "int_join.hpp"

#include <cstdint>

#if !defined(__SIZEOF_INT128__)
#error "__SIZEOF_INT128__ not defined"
#endif

DEF_URBG_SUBCLASS(mcg128, __uint128_t, uint64_t)

/// prepare the initial state
void
mcg128::init()
{}

mcg128::result_type
mcg128::next()
{
    constexpr __uint128_t M =
        int_join(UINT64_C(0x45a31efc5a35d971),
                 UINT64_C(0x261fd0407a968add)); // not prime (popcount = 64)
    static_assert(M & 1, "must be odd");

    s *= M;
    s += M;
    return s >> 64;
}
