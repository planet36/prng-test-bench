// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// klimov_shamir_32 PRNG
/**
* \file
* \author Steven Ward
*/

#pragma once

#include "urbg_base_class.hpp"

#include <cstdint>

DEF_URBG_SUBCLASS(klimov_shamir_32, uint64_t, uint32_t)

/// Prepare the initial state
void
klimov_shamir_32::init()
{
    constexpr int num_warmup_discards = 5;
    for (int i = 0; i < num_warmup_discards; ++i)
    {
        (void)next(); // Assumes this function advances the state
    }
}

/**
* \sa https://link.springer.com/content/pdf/10.1007/3-540-36400-5_34.pdf
* \sa https://old.reddit.com/r/cpp/comments/8vhrzh/better_c_pseudo_random_number_generator/e1nlcv9/
*/
klimov_shamir_32::result_type
klimov_shamir_32::next()
{
    constexpr unsigned int C = 5;
    static_assert(C & 0b001, "least significant bit must be 1");
    static_assert(C & 0b100, "third least significant bit must be 1");

    s += (s * s) | C;
    result_type result = s >> 32;

    return result;
}
