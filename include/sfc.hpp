// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// SFC (Small Fast Chaotic) PRNG by Chris Doty-Humphrey
/**
* \file
* \author Steven Ward
* \sa https://github.com/numpy/numpy/blob/main/numpy/random/src/sfc64/sfc64.h
* \sa https://sourceforge.net/projects/pracrand/files/
*/

#pragma once

#include "abstract_urbg_class.hpp"

#include <array>
#include <bit>
#include <cstdint>

// Adapted from:
// PractRand-pre0.95/src/RNGs/sfc.cpp
DEF_URBG_SUBCLASS(sfc32, SINGLE_ARG(std::array<uint32_t, 4>), uint32_t)

/// prepare the initial state
void
sfc32::init()
{
    for (int i = 0; i < 6; ++i)
    {
        (void)next();
    }
}

sfc32::result_type
sfc32::next()
{
    const result_type result = s[0] + s[1] + s[3]++;
    s[0] = s[1] ^ (s[1] >> 9);
    s[1] = s[2] + (s[2] << 3);
    s[2] = std::rotl(s[2], 21) + result;
    return result;
}

DEF_URBG_SUBCLASS(sfc64, SINGLE_ARG(std::array<uint64_t, 4>), uint64_t)

/// prepare the initial state
void
sfc64::init()
{
    for (int i = 0; i < 9; ++i)
    {
        (void)next();
    }
}

sfc64::result_type
sfc64::next()
{
    const result_type result = s[0] + s[1] + s[3]++;
    s[0] = s[1] ^ (s[1] >> 11);
    s[1] = s[2] + (s[2] << 3);
    s[2] = std::rotl(s[2], 24) + result;
    return result;
}
