// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// gjrand PRNG
/**
* \file
* \author Steven Ward
*/

#pragma once

#include "def_urbg_class.hpp"

#include <array>
#include <bit>
#include <cstdint>

/*
https://gist.github.com/imneme/7a783e20f71259cc13e219829bcea4ac
https://sourceforge.net/projects/gjrand/files/
*/
DEF_URBG_CLASS(gjrand, SINGLE_ARG(std::array<uint64_t, 4>), uint64_t)
{
    static constexpr uint64_t A1 = 0x55aa96a5; // not prime (popcount = 16)
    static_assert(A1 & 1, "must be odd");
    static constexpr unsigned int R1 = 32;
    static constexpr unsigned int R2 = 23;
    static constexpr unsigned int R3 = 19;

    s[1] += s[2];
    s[0] = std::rotl(s[0], R1);
    s[2] ^= s[1];
    s[3] += A1;
    s[0] += s[1];
    s[2] = std::rotl(s[2], R2);
    s[1] ^= s[0];
    s[0] += s[2];
    s[1] = std::rotl(s[1], R3);
    s[2] += s[0];
    s[1] += s[3];
    return s[0];
}
