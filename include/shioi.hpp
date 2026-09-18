// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// shioi PRNG
/**
* \file
* \author Steven Ward
* \sa https://github.com/andanteyk/prng-shioi/blob/master/shioi128.c
*/

#pragma once

#include "urbg_base_class.hpp"
#include "wyprimes.hpp"

#include <array>
#include <bit>
#include <cstdint>

DEF_URBG_SUBCLASS(shioi, SINGLE_ARG(std::array<uint64_t, 2>), uint64_t)

/// prepare the initial state
void
shioi::init()
{}

shioi::result_type
shioi::next()
{
    constexpr uint64_t M1 = 0xd2b74407b1ce6e93; // not prime (popcount = 33)
    static_assert(M1 & 1, "must be odd");

    s[0] += wyprimes::_wyp[0]; // (SDW)
    s[1] += wyprimes::_wyp[1]; // (SDW)
    const auto old_s = s;
    const result_type result = std::rotl(old_s[0] * M1, 29) + old_s[1];

    // Note: MUST use arithmetic right shift
    s[0] = old_s[1];
    s[1] = (old_s[0] << 2) ^ (static_cast<int64_t>(old_s[0]) >> 19) ^ old_s[1];

    return result;
}
