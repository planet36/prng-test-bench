// SPDX-FileCopyrightText: James W. Hanlon
// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: CC-BY-4.0

/// xoroshiro128aox PRNG
/**
* \file
* \author Steven Ward
* \sa https://www.jameswhanlon.com/the-hardware-pseudorandom-number-generator-of-the-graphcore-ipu.html
*/

#pragma once

#include "abstract_urbg_class.hpp"

#include <array>
#include <bit>
#include <cstdint>

DEF_URBG_SUBCLASS(xoroshiro128aox, SINGLE_ARG(std::array<uint64_t, 2>), uint64_t)

/// prepare the initial state
void
xoroshiro128aox::init()
{
}

xoroshiro128aox::result_type
xoroshiro128aox::next()
{
    const auto sx = s[0] ^ s[1];

    // Calculate the result, the 'AOX' step.
    const auto sa = s[0] & s[1];
    const auto result = sx ^ (std::rotl(sa, 1) | std::rotl(sa, 2));

    // xoroshiro128 state update
    s[0] = std::rotl(s[0], 55) ^ sx ^ (sx << 14);
    s[1] = std::rotl(sx, 36);

    return result;
}
