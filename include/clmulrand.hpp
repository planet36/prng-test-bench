// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Carry-less Multiplication (CLMUL) PRNG
/**
* \file
* \author Steven Ward
* \sa https://en.wikipedia.org/wiki/CLMUL_instruction_set
* \sa https://www.felixcloutier.com/x86/pclmulqdq
* \sa https://www.intel.com/content/dam/develop/external/us/en/documents/clmul-wp-rev-2-02-2014-04-20.pdf
*/

#pragma once

#include "abstract_urbg_class.hpp"
#include "clmum.hpp"
#include "xxhprimes.hpp"

#include <cstdint>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
DEF_URBG_SUBCLASS(clmulrand, __m128i, uint64_t)
#pragma GCC diagnostic pop

/// prepare the initial state
void
clmulrand::init()
{}

clmulrand::result_type
clmulrand::next()
{
    // most significant elem first
    const __m128i inc = _mm_set_epi64x(XXH_PRIME64_2, XXH_PRIME64_1);
    s = _mm_add_epi64(s, inc);
    return clmums(s);
}
