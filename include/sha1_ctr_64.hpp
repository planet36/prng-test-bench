// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// A PRNG that uses SHA-1 instructions
/**
* \file
* \author Steven Ward
*/

#pragma once

#if defined(__SHA__)

#include "abstract_urbg_class.hpp"
#include "hxor.h"
#include "make_odd.h"
#include "mm_equal.h"
#include "scaled-const.h"
#include "simd-array.hpp"

#include <cstdint>
#include <immintrin.h>
#include <random>

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

DEF_URBG_SUBCLASS(sha1_ctr_64, arr_m128i<2>, uint64_t)

// s[0] is the state/counter
// s[1] is the increment (must be odd)

/// prepare the initial state
/**
* Every odd integer is coprime with every power of 2.
* Therefore, \c inc shall be made odd.
*/
void sha1_ctr_64::init()
{
    s[1] = mm_make_odd_epu64(s[1]);

    // both increment values must be unique (and odd)
    if (mm_all_equal_epi64(s[1]))
    {
        // most significant elem first
        const auto mask_inc = _mm_set_epi64x(FLOOR_SCALED_FRAC_SQRT_3,
                                             FLOOR_SCALED_FRAC_SQRT_2);

        // make unique
        s[1] = _mm_xor_si128(s[1], mask_inc);

        s[1] = mm_make_odd_epu64(s[1]);
    }
}

sha1_ctr_64::result_type sha1_ctr_64::next()
{
    __m128i dst = s[0];
    s[0] = _mm_add_epi64(s[0], s[1]);
    dst = sha1_rnds4x4(dst, s[0]);
    return mm_hxor_epu64(dst);
}

#else

#warning "__SHA__ not defined"

#endif
