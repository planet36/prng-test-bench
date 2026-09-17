// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// A PRNG that uses AES instructions
/**
* \file
* \author Steven Ward
*/

#pragma once

#if defined(__AES__)

#include "abstract_urbg_class.hpp"
#include "mm_cast.hpp"
#include "sha2_iv.h"
#include "simd_compress.hpp"
#include "simd_types.hpp"

#include <immintrin.h>

#if !defined(__SIZEOF_INT128__)
#error "__SIZEOF_INT128__ not defined"
#endif

// s[0] is the first state/counter
// s[1] is the second state/counter
DEF_URBG_SUBCLASS(aes_compress_ctr2_128, simd_arr_t<2>, __uint128_t)

/// prepare the initial state
void
aes_compress_ctr2_128::init()
{
}

/// advance the state of the PRNG, and generate a pseudo-random value
aes_compress_ctr2_128::result_type
aes_compress_ctr2_128::next()
{
    /*
    * The counter increment \c inc used below forms a Weyl sequence.
    * Criteria for its 64-bit lane values:
    *   1) Must be odd
    *   2) Must be unique across lanes
    *
    * \sa https://en.wikipedia.org/wiki/Weyl_sequence#In_computing
    */

    const simd_arr_t<2> inc{
        // most significant elem first
        _mm_set_epi64x(SHA_512_H0_1 | 1U, SHA_512_H0_0 | 1U), // NOLINT(cppcoreguidelines-narrowing-conversions)
        _mm_set_epi64x(SHA_512_H0_3 | 1U, SHA_512_H0_2 | 1U), // NOLINT(cppcoreguidelines-narrowing-conversions)
    };

    s[0] = _mm_add_epi64(s[0], inc[0]);
    s[1] = _mm_add_epi64(s[1], inc[1]);

    return uint128_from_m128i(simd_compress_aes_enc_r4(s[0], s[1]));
}

#else

#warning "__AES__ not defined"

#endif
