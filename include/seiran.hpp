// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// seiran PRNG
/**
\file
\author Steven Ward
\sa https://github.com/andanteyk/prng-seiran/blob/master/seiran128.c
*/

#pragma once

#include "def_urbg_class.hpp"
#include "xxhprimes.hpp"

#include <array>
#include <bit>
#include <cstdint>

DEF_URBG_CLASS(seiran, SINGLE_ARG(std::array<uint64_t, 2>), uint64_t)
{
    static constexpr unsigned int M1 = 9;
    static_assert(M1 & 1, "must be odd");
    static constexpr unsigned int R1 = 29;
    static constexpr unsigned int S1 = 9;

    s[0] += xxh_prime64[0]; // (SDW)
    s[1] += xxh_prime64[1]; // (SDW)

    const auto old_s = s;
    const result_type result = std::rotl((old_s[0] + old_s[1]) * M1, R1) + old_s[0];

    s[0] = old_s[0] ^ std::rotl(old_s[1], R1);
    s[1] = old_s[0] ^ (old_s[1] << S1);

    return result;
}
