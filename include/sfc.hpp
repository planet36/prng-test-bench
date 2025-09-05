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
void sfc32::init()
{
    for (int i = 0; i < 6; ++i)
    {
        (void)next();
    }
}

sfc32::result_type sfc32::next()
{
    constexpr unsigned int S1 = 9;
    constexpr unsigned int S2 = 3;
    constexpr unsigned int R1 = 21;

    const result_type result = s[0] + s[1] + s[3]++;
    s[0] = s[1] ^ (s[1] >> S1);
    s[1] = s[2] + (s[2] << S2);
    s[2] = std::rotl(s[2], R1) + result;
    return result;
}

DEF_URBG_SUBCLASS(sfc64, SINGLE_ARG(std::array<uint64_t, 4>), uint64_t)

/// prepare the initial state
void sfc64::init()
{
    for (int i = 0; i < 9; ++i)
    {
        (void)next();
    }
}

sfc64::result_type sfc64::next()
{
    constexpr unsigned int S1 = 11;
    constexpr unsigned int S2 = 3;
    constexpr unsigned int R1 = 24;

    const result_type result = s[0] + s[1] + s[3]++;
    s[0] = s[1] ^ (s[1] >> S1);
    s[1] = s[2] + (s[2] << S2);
    s[2] = std::rotl(s[2], R1) + result;
    return result;
}
