// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// mx3 PRNG
/**
* \file
* \author Steven Ward
* \sa https://github.com/jonmaiga/mx3/blob/master/mx3.h
*/

#pragma once

#include "abstract_urbg_class.hpp"

#include <cstdint>

DEF_URBG_SUBCLASS(mx3, uint64_t, uint64_t)

/// prepare the initial state
void
mx3::init()
{}

mx3::result_type
mx3::next()
{
    constexpr uint64_t M1 = 0xbea225f9eb34556d; // not prime (popcount = 36)
    static_assert(M1 & 1, "must be odd");

    // NOTE: Their algorithm adds M1 to the seed.
    // NOTE: Their state is named "_counter".
    auto x = s++;

    x ^= x >> 32;
    x *= M1;
    x ^= x >> 29;
    x *= M1;
    x ^= x >> 32;
    x *= M1;
    x ^= x >> 29;
    return x;
}
