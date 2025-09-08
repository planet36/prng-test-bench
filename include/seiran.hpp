// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// seiran PRNG
/**
* \file
* \author Steven Ward
* \sa https://github.com/andanteyk/prng-seiran/blob/master/seiran128.c
*/

#pragma once

#include "abstract_urbg_class.hpp"
#include "xxhprimes.hpp"

#include <array>
#include <bit>
#include <cstdint>

DEF_URBG_SUBCLASS(seiran, SINGLE_ARG(std::array<uint64_t, 2>), uint64_t)

/// prepare the initial state
void seiran::init()
{
}

seiran::result_type seiran::next()
{
    constexpr unsigned int M1 = 9;
    static_assert(M1 & 1, "must be odd");

    s[0] += XXH_PRIME64_1; // (SDW)
    s[1] += XXH_PRIME64_2; // (SDW)

    const auto old_s = s;
    const result_type result = std::rotl((old_s[0] + old_s[1]) * M1, 29) + old_s[0];

    s[0] = old_s[0] ^ std::rotl(old_s[1], 29);
    s[1] = old_s[0] ^ (old_s[1] << 9);

    return result;
}
