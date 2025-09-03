// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// splitmix64 PRNG
/**
* \file
* \author Steven Ward
* \sa https://hg.openjdk.java.net/jdk8/jdk8/jdk/file/tip/src/share/classes/java/util/SplittableRandom.java
* \sa https://gee.cs.oswego.edu/dl/papers/oopsla14.pdf
*/

#pragma once

#include "abstract_urbg_class.hpp"

#include <cstdint>

/// GoldenRatio scaled to uint64_t
/**
* https://www.wolframalpha.com/input?i=IntegerString%5BFloor%5B%282**64%29*Frac%5BGoldenRatio%5D%5D%2C16%5D
*/
#define GOLDEN_RATIO_64 UINT64_C(0x9e3779b97f4a7c15) // not prime (popcount = 38)

DEF_URBG_SUBCLASS(splitmix32, uint64_t, uint32_t)

/// prepare the initial state
void splitmix32::init()
{
}

splitmix32::result_type splitmix32::next()
{
    static constexpr uint64_t inc = GOLDEN_RATIO_64;
    static_assert((inc & 1) != 0, "must be odd");

    static constexpr uint64_t M1 = 0x62a9d9ed799705f5; // not prime (popcount = 36)
    static constexpr uint64_t M2 = 0xcb24d0a5c88c35b3; // not prime (popcount = 29)
    static constexpr unsigned int S1 = 33;
    static constexpr unsigned int S2 = 28;
    static constexpr unsigned int S3 = 32;
    static_assert((M1 & 1) != 0, "must be odd");
    static_assert((M2 & 1) != 0, "must be odd");

    // Not result_type
    auto x = s;
    s += inc; // (SDW)

    x ^= x >> S1;
    x *= M1;
    x ^= x >> S2;
    x *= M2;
#if 0
    x ^= x >> S3;
#else
    // "Returns the 32 high bits of Stafford variant 4 mix64 function as int."
    x >>= S3;
#endif
    return x;
}

DEF_URBG_SUBCLASS(splitmix64, uint64_t, uint64_t)

/// prepare the initial state
void splitmix64::init()
{
}

splitmix64::result_type splitmix64::next()
{
    static constexpr uint64_t inc = GOLDEN_RATIO_64;
    static_assert((inc & 1) != 0, "must be odd");

    static constexpr uint64_t M1 = 0xbf58476d1ce4e5b9; // not prime (popcount = 36)
    static constexpr uint64_t M2 = 0x94d049bb133111eb; // not prime (popcount = 29)
    static constexpr unsigned int S1 = 30;
    static constexpr unsigned int S2 = 27;
    static constexpr unsigned int S3 = 31;
    static_assert((M1 & 1) != 0, "must be odd");
    static_assert((M2 & 1) != 0, "must be odd");

    auto x = s;
    s += inc; // (SDW)

    x ^= x >> S1;
    x *= M1;
    x ^= x >> S2;
    x *= M2;
    x ^= x >> S3;
    return x;
}
