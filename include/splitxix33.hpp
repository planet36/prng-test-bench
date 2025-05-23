// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// splitxix33 PRNG
/**
\file
\author Steven Ward
\sa https://github.com/skeeto/scratch/tree/master/splitxix33
*/

#pragma once

#include "def_urbg_class.hpp"

#include <cstdint>

DEF_URBG_CLASS(splitxix33, uint64_t, uint64_t)
{
    static constexpr uint64_t inc = 1'111'111'111'111'111'111U; // prime (popcount = 35)
    static_assert(inc & 1, "must be odd");

    static constexpr uint64_t M = 1'111'111'111'111'111'111U; // prime (popcount = 35)
    static_assert(M & 1, "must be odd");
    static constexpr unsigned int S = 33;

    s += inc;
    auto x = s;

    x ^= x >> S; x *= M;
    x ^= x >> S; x *= M;
    x ^= x >> S;
    return x;
}
