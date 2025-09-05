// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Lehmer PRNG
/**
* \file
* \author Steven Ward
* \sa https://lemire.me/blog/2019/03/19/the-fastest-conventional-random-number-generator-that-can-pass-big-crush/
* \sa https://www.pcg-random.org/posts/128-bit-mcg-passes-practrand.html
* \sa https://github.com/lemire/testingRNG/blob/master/source/lehmer64.h
*/

#pragma once

#if !defined(__SIZEOF_INT128__)
#error "__SIZEOF_INT128__ not defined"
#endif

#include "abstract_urbg_class.hpp"

#include <cstdint>

DEF_URBG_SUBCLASS(lehmer64, __uint128_t, uint64_t)

/// prepare the initial state
void lehmer64::init()
{
}

lehmer64::result_type lehmer64::next()
{
    constexpr uint64_t M = 0xda942042e4dd58b5; // not prime (popcount = 29)
    static_assert((M & 1) != 0, "must be odd");

    ++s; // (SDW)
    s *= M;
    return s >> 64;
}
