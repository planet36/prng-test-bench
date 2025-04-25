// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// shioi PRNG
/**
\file
\author Steven Ward
\sa https://github.com/andanteyk/prng-shioi/blob/master/shioi128.c
*/

#pragma once

#include "def_urbg_class.hpp"
#include "xxhprimes.hpp"

#include <array>
#include <bit>
#include <cstdint>

DEF_URBG_CLASS(shioi, SINGLE_ARG(std::array<uint64_t, 2>), uint64_t)
{
    static constexpr uint64_t M1 = 0xd2b74407b1ce6e93; // not prime (popcount = 33)
    static_assert(M1 & 1, "must be odd");
    static constexpr unsigned int R1 = 29;
    static constexpr unsigned int S1 = 19;

    s[0] += xxh_prime64[0]; // (SDW)
    s[1] += xxh_prime64[1]; // (SDW)
    const auto old_s = s;
    const result_type result = std::rotl(old_s[0] * M1, R1) + old_s[1];

    // Note: MUST use arithmetic right shift
    s[0] = old_s[1];
    s[1] = (old_s[0] << 2) ^ (static_cast<int64_t>(old_s[0]) >> S1) ^ old_s[1];

    return result;
}
