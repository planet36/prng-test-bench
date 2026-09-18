// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// seiran PRNG
/**
* \file
* \author Steven Ward
* \sa https://github.com/andanteyk/prng-seiran/blob/master/seiran128.c
*/

#pragma once

#include "urbg_base_class.hpp"
#include "wyprimes.hpp"

#include <array>
#include <bit>
#include <cstdint>

DEF_URBG_SUBCLASS(seiran, SINGLE_ARG(std::array<uint64_t, 2>), uint64_t)

/// Prepare the initial state
void
seiran::init()
{}

seiran::result_type
seiran::next()
{
    constexpr int M1 = 9;
    static_assert(M1 & 1, "must be odd");

    s[0] += wyprimes::_wyp[0]; // (SDW)
    s[1] += wyprimes::_wyp[1]; // (SDW)

    const auto old_s = s;
    const result_type result = std::rotl((old_s[0] + old_s[1]) * M1, 29) + old_s[0];

    s[0] = old_s[0] ^ std::rotl(old_s[1], 29);
    s[1] = old_s[0] ^ (old_s[1] << 9);

    return result;
}
