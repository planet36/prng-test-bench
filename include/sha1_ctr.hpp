// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// A PRNG that uses SHA-1 instructions
/**
* \file
* \author Steven Ward
*/

#pragma once

#if defined(__SHA__)

#include "abstract_urbg_class.hpp"
#include "mm_cast.hpp"
#include "xxhprimes.hpp"

#include <immintrin.h>

/// 4*4 rounds of SHA-1
/**
* \sa https://www.felixcloutier.com/x86/sha1rnds4
* \sa https://github.com/noloader/SHA-Intrinsics/blob/master/sha1-x86.c
*/
static __m128i
sha1_rnds4x4(__m128i a, __m128i b)
{
    a = _mm_sha1rnds4_epu32(a, b, 0);
    b = _mm_sha1rnds4_epu32(b, a, 1);
    a = _mm_sha1rnds4_epu32(a, b, 2);
    b = _mm_sha1rnds4_epu32(b, a, 3);
    return b;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
DEF_URBG_SUBCLASS(sha1_ctr_128, __m128i, __uint128_t)
#pragma GCC diagnostic pop

/// prepare the initial state
void
sha1_ctr_128::init()
{
}

sha1_ctr_128::result_type
sha1_ctr_128::next()
{
    // most significant elem first
    const __m128i inc = _mm_set_epi64x(XXH_PRIME64_2, XXH_PRIME64_1);

    __m128i dst = s;
    s = _mm_add_epi64(s, inc);
    dst = sha1_rnds4x4(dst, s);

    return uint128_from_m128i(dst);
}

#else

#warning "__SHA__ not defined"

#endif
