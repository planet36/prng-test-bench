// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// klimov_shamir_32 PRNG
/**
* \file
* \author Steven Ward
*/

#pragma once

#include "abstract_urbg_class.hpp"
#include "xxhprimes.hpp"

#include <cstdint>

// https://link.springer.com/content/pdf/10.1007/3-540-36400-5_34.pdf
// https://old.reddit.com/r/cpp/comments/8vhrzh/better_c_pseudo_random_number_generator/e1nlcv9/

DEF_URBG_SUBCLASS(klimov_shamir_32, uint64_t, uint32_t)

/// prepare the initial state
void
klimov_shamir_32::init()
{}

klimov_shamir_32::result_type
klimov_shamir_32::next()
{
    constexpr uint64_t inc = XXH_PRIME64_1;
    static_assert(inc & 1, "must be odd");

    constexpr unsigned int C = 5;
    static_assert(C & 0b001, "least significant bit must be 1");
    static_assert(C & 0b100, "third least significant bit must be 1");

#if 0
    // XXX: a zero seed needs 5 iterations to avalanche the bits
    s += (s * s) | C;
    result_type result = s >> 32;
#else
    s += inc; // (SDW)
    result_type result = (s + ((s * s) | C)) >> 32; // (SDW)
#endif

    return result;
}
