// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// biski64 PRNG
/**
* \file
* \author Steven Ward
* \sa https://github.com/danielcota/biski64/blob/main/c/biski64.c
*/

#pragma once

#include "abstract_urbg_class.hpp"

#include <array>
#include <bit>
#include <cstdint>

DEF_URBG_SUBCLASS(biski64, SINGLE_ARG(std::array<uint64_t, 3>), uint64_t)
// s[0] is the fast_loop
// s[1] is the mix
// s[2] is the loop_mix

/// prepare the initial state
/**
* Adapted from biski64_warmup
* \sa https://github.com/danielcota/biski64/blob/main/c/biski64.c#L49
*/
void biski64::init()
{
    for (int i = 0; i < 16; ++i)
    {
        next(); // Assumes this function advances the state
    }
}

biski64::result_type biski64::next()
{
    constexpr unsigned int R1 = 16;
    constexpr unsigned int R2 = 40;

    const result_type result = s[1] + s[2];
    const auto old_loop_mix = s[2];

    s[2] = s[0] ^ s[1];
    s[1] = std::rotl(s[1], R1) + std::rotl(old_loop_mix, R2);
    s[0] += 0x9999999999999999ULL; // Additive constant for the Weyl sequence.

    return result;
}
