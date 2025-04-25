// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Carry-less Multiplication (CLMUL) PRNG
/**
\file
\author Steven Ward
\sa https://en.wikipedia.org/wiki/CLMUL_instruction_set
\sa https://www.felixcloutier.com/x86/pclmulqdq
*/

#pragma once

#include "byteprimes.hpp"
#include "clmum.hpp"
#include "def_urbg_class.hpp"

#include <cstdint>

DEF_URBG_CLASS(clmulrand, __m128i, uint64_t)
{
    const __m128i inc = _mm_set_epi64x(byteprimes[1], byteprimes[0]);
    s = _mm_add_epi64(s, inc);
    return clmums(s);
}
