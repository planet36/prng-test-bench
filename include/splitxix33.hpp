// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// splitxix33 PRNG
/**
* \file
* \author Steven Ward
* \sa https://github.com/skeeto/scratch/tree/master/splitxix33
*/

#pragma once

#include "abstract_urbg_class.hpp"

#include <cstdint>

DEF_URBG_SUBCLASS(splitxix33, uint64_t, uint64_t)

/// prepare the initial state
void
splitxix33::init()
{}

// https://github.com/skeeto/scratch/blob/master/splitxix33/splitxix33.c
splitxix33::result_type
splitxix33::next()
{
    constexpr uint64_t inc = UINT64_C(1'111'111'111'111'111'111); // prime (popcount = 35)
    static_assert(inc & 1, "must be odd");

    constexpr uint64_t M = UINT64_C(1'111'111'111'111'111'111); // prime (popcount = 35)
    static_assert(M & 1, "must be odd");

    s += inc;
    auto x = s;

    x ^= x >> 33;
    x *= M;
    x ^= x >> 33;
    x *= M;
    x ^= x >> 33;
    return x;
}
