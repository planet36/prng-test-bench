// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// A PRNG that uses AES instructions
/**
* \file
* \author Steven Ward
*/

#pragma once

#if defined(__AES__)

#include "mm_cast.hpp"
#include "sha2_iv.h"
#include "simd_types.hpp"
#include "urbg_base_class.hpp"

#include <immintrin.h>

#if !defined(__SIZEOF_INT128__)
#error "__SIZEOF_INT128__ not defined"
#endif

// s[0] is the state/counter
// s[1] is the key
DEF_URBG_SUBCLASS(aes_ctr_128, simd_arr_t<2>, __uint128_t)

/// prepare the initial state
void
aes_ctr_128::init()
{
}

/// advance the state of the PRNG, and generate a pseudo-random value
aes_ctr_128::result_type
aes_ctr_128::next()
{
    // must do at least 3 rounds of AES
    constexpr int Nr = 3;
    static_assert(Nr >= 3);

    /*
    * The counter increment \c inc used below forms a Weyl sequence.
    * Criteria for its 64-bit lane values:
    *   1) Must be odd
    *   2) Must be unique across lanes
    *
    * \sa https://en.wikipedia.org/wiki/Weyl_sequence#In_computing
    */

    // most significant elem first
    const __m128i inc = _mm_set_epi64x(SHA_512_H0_1 | 1U, SHA_512_H0_0 | 1U); // NOLINT(cppcoreguidelines-narrowing-conversions)

    __m128i dst = s[0];
    s[0] = _mm_add_epi64(s[0], inc);

    for (int r = 0; r < Nr; ++r)
    {
        dst = _mm_aesenc_si128(dst, s[1]);
    }

    return uint128_from_m128i(dst);
}

#else

#warning "__AES__ not defined"

#endif
