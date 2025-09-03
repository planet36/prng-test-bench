// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Carry-less Multiplication (CLMUL) PRNG
/**
* \file
* \author Steven Ward
* \sa https://en.wikipedia.org/wiki/CLMUL_instruction_set
* \sa https://www.felixcloutier.com/x86/pclmulqdq
*/

#pragma once

#include "abstract_urbg_class.hpp"
#include "byteprimes.hpp"
#include "clmum.hpp"

#include <cstdint>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
DEF_URBG_SUBCLASS(clmulrand, __m128i, uint64_t)
#pragma GCC diagnostic pop

/// prepare the initial state
void clmulrand::init()
{
}

clmulrand::result_type clmulrand::next()
{
    const __m128i inc = _mm_set_epi64x(byteprimes[1], byteprimes[0]);
    s = _mm_add_epi64(s, inc);
    return clmums(s);
}
