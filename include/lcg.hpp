// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// lcg32, lcg64 PRNGs
/**
\file
\author Steven Ward
\sa https://www.pcg-random.org/posts/does-it-beat-the-minimal-standard.html
*/

#pragma once

#include "def_urbg_class.hpp"
#include "int_join.hpp"

#include <cstdint>

DEF_URBG_CLASS(lcg32, __uint128_t, uint32_t)
{
    static constexpr __uint128_t M = int_join(0xc580caddUL, 0x754f7336d2eaa27dUL); // prime (popcount = 52)
    static_assert(M & 1, "must be odd");

    s *= M;
    s += M;
    return s >> 64;
}

DEF_URBG_CLASS(lcg64, __uint128_t, uint64_t)
{
    static constexpr __uint128_t M = int_join(0x2d99787926d46932UL, 0xa4c1f32680f70c55UL); // not prime (popcount = 60)
    static_assert(M & 1, "must be odd");

    s *= M;
    s += M;
    return s >> 64;
}
