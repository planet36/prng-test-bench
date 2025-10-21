// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// STC crand PRNG
/**
* \file
* \author Steven Ward
* Based on SFC
* https://github.com/stclib/STC/blob/main/include/stc/random.h
* https://github.com/stclib/STC/blob/main/docs/random_api.md
*/

#pragma once

#include "abstract_urbg_class.hpp"
#include "golden_ratio.h"

#include <array>
#include <bit>
#include <cstdint>

DEF_URBG_SUBCLASS(stc_crand32, SINGLE_ARG(std::array<uint32_t, 4>), uint32_t)

/// prepare the initial state
void
stc_crand32::init()
{}

stc_crand32::result_type
stc_crand32::next()
{
    constexpr uint32_t inc = GOLDEN_RATIO_32;
    static_assert((inc & 1) != 0, "must be odd");

    const result_type result = (s[0] ^ (s[3] += inc)) + s[1];
    s[0] = s[1] ^ (s[1] >> 9);
    s[1] = s[2] + (s[2] << 3);
    s[2] = std::rotr(s[2], 11) + result;
    return result;
}

DEF_URBG_SUBCLASS(stc_crand64, SINGLE_ARG(std::array<uint64_t, 4>), uint64_t)

/// prepare the initial state
void
stc_crand64::init()
{}

stc_crand64::result_type
stc_crand64::next()
{
    constexpr uint64_t inc = GOLDEN_RATIO_64;
    static_assert((inc & 1) != 0, "must be odd");

    const result_type result = (s[0] ^ (s[3] += inc)) + s[1];
    s[0] = s[1] ^ (s[1] >> 11);
    s[1] = s[2] + (s[2] << 3);
    s[2] = std::rotl(s[2], 24) + result;
    return result;
}
