// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// lcg32, lcg64 PRNGs
/**
* \file
* \author Steven Ward
* \sa https://www.pcg-random.org/posts/does-it-beat-the-minimal-standard.html
*/

#pragma once

#include "abstract_urbg_class.hpp"
#include "int_join.hpp"

#include <cstdint>

#if !defined(__SIZEOF_INT128__)
#error "__SIZEOF_INT128__ not defined"
#endif

DEF_URBG_SUBCLASS(lcg32, __uint128_t, uint32_t)

/// prepare the initial state
void lcg32::init()
{
}

lcg32::result_type lcg32::next()
{
    constexpr __uint128_t M = int_join(UINT64_C(0xc580cadd), UINT64_C(0x754f7336d2eaa27d)); // prime (popcount = 52)
    static_assert(M & 1, "must be odd");

    s *= M;
    s += M;
    return s >> 64;
}

DEF_URBG_SUBCLASS(lcg64, __uint128_t, uint64_t)

/// prepare the initial state
void lcg64::init()
{
}

lcg64::result_type lcg64::next()
{
    constexpr __uint128_t M = int_join(UINT64_C(0x2d99787926d46932), UINT64_C(0xa4c1f32680f70c55)); // not prime (popcount = 60)
    static_assert(M & 1, "must be odd");

    s *= M;
    s += M;
    return s >> 64;
}
