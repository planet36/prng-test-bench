// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// stc64 PRNG
/**
* \file
* \author Steven Ward
*/

#pragma once

#include "abstract_urbg_class.hpp"
#include "golden_ratio.h"

#include <array>
#include <bit>
#include <cstdint>

/*
* Based on SFC
* https://github.com/stclib/STC/blob/main/include/stc/random.h
* https://github.com/stclib/STC/blob/main/docs/random_api.md
*/
DEF_URBG_SUBCLASS(stc64, SINGLE_ARG(std::array<uint64_t, 4>), uint64_t)

/// prepare the initial state
void stc64::init()
{
}

stc64::result_type stc64::next()
{
    constexpr uint64_t inc = GOLDEN_RATIO_64;
    static_assert((inc & 1) != 0, "must be odd");

    const result_type result = (s[0] ^ (s[3] += inc)) + s[1];
    s[0] = s[1] ^ (s[1] >> 11);
    s[1] = s[2] + (s[2] << 3);
    s[2] = std::rotl(s[2], 24) + result;
    return result;
}
