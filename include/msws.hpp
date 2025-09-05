// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Middle-Square Weyl Sequence PRNG
/**
* \file
* \author Steven Ward
* \sa https://arxiv.org/pdf/1704.00358
*/

#pragma once

#include "abstract_urbg_class.hpp"

#include <array>
#include <bit>
#include <cstdint>

DEF_URBG_SUBCLASS(msws32, SINGLE_ARG(std::array<uint64_t, 2>), uint32_t)

/// prepare the initial state
void msws32::init()
{
}

msws32::result_type msws32::next()
{
    constexpr uint64_t inc = 0xb5ad4eceda1ce2a9;
    static_assert(inc & 1, "must be odd");

    s[0] *= s[0];
    s[0] += (s[1] += inc);
    s[0] = std::rotr(s[0], 32);

    return static_cast<uint32_t>(s[0]);
}

DEF_URBG_SUBCLASS(msws64, SINGLE_ARG(std::array<uint64_t, 4>), uint64_t)

/// prepare the initial state
void msws64::init()
{
}

msws64::result_type msws64::next()
{
    constexpr uint64_t inc = 0xb5ad4eceda1ce2a9;
    constexpr uint64_t inc2 = 0x278c5a4d8419fe6b;

    static_assert(inc & 1, "must be odd");
    static_assert(inc2 & 1, "must be odd");

    uint64_t xx;

    s[0] *= s[0];
    s[0] += (s[1] += inc);
    xx = s[0];
    s[0] = std::rotr(s[0], 32);

    s[2] *= s[2];
    s[2] += (s[3] += inc2);
    s[2] = std::rotr(s[2], 32);

    return xx ^ s[2];
}
