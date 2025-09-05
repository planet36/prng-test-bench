// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// stc64 PRNG
/**
* \file
* \author Steven Ward
*/

#pragma once

#include "abstract_urbg_class.hpp"

#include <array>
#include <bit>
#include <cstdint>

/*
very similar to SFC
https://github.com/stclib/STC/blob/master/include/stc/crand.h#L79
https://github.com/tkaitchuck/Mwc256XXA64/discussions/3#discussioncomment-2326885
https://github.com/stclib/STC/blob/master/docs/crandom_api.md
*/
DEF_URBG_SUBCLASS(stc64, SINGLE_ARG(std::array<uint64_t, 4>), uint64_t)

/// prepare the initial state
void stc64::init()
{
}

stc64::result_type stc64::next()
{
    constexpr unsigned int S1 = 11;
    constexpr unsigned int S2 = 3;
    constexpr unsigned int R1 = 24;

    // NOTE: Their algorithm adds a constant to the seed.

    //const result_type result = (s[0] ^ (s[3] += s[4])) + s[1];
    //const result_type result = (s[0] ^ (s[3] += inc)) + s[1];
    const result_type result = (s[0] ^ s[3]++) + s[1]; // (SDW)
    s[0] = s[1] ^ (s[1] >> S1);
    s[1] = s[2] + (s[2] << S2);
    s[2] = std::rotl(s[2], R1) + result;
    return result;
}
