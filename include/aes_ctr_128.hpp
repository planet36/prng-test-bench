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
#include "wyprimes.hpp"

#include <immintrin.h>

#if !defined(__SIZEOF_INT128__)
#error "__SIZEOF_INT128__ not defined"
#endif

#if !defined(__SSE4_1__)
#error "Architecture not supported"
#endif

// s[0] is the state/counter
// s[1] is the key
DEF_URBG_SUBCLASS(aes_ctr_128, simd_arr_t<2>, __uint128_t)

/// Prepare the initial state
/**
* The key is adjusted, if necessary, so that its 64-bit lanes differ.
*/
void
aes_ctr_128::init()
{
    // The 64-bit lanes of the key must differ.
    // next() uses the same key in every round.  With a key of (K, K), if the counter
    // (A, B) gives the output (X, Y), then the counter (B, A) gives the output (Y, X).

    // most significant elem first
    const auto key_mask = _mm_set_epi64x(SHA_512_H0_1, SHA_512_H0_0); // NOLINT(cppcoreguidelines-narrowing-conversions)

    const auto swapped = _mm_shuffle_epi32(s[1], _MM_SHUFFLE(1, 0, 3, 2));
    // all ones if the lanes are equal, all zeros otherwise
    const auto equal_mask = _mm_cmpeq_epi64(s[1], swapped);

    s[1] = _mm_xor_si128(s[1], _mm_and_si128(equal_mask, key_mask));
}

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
    const __m128i inc = _mm_set_epi64x(wyprimes::_wyp[1], wyprimes::_wyp[0]); // NOLINT(cppcoreguidelines-narrowing-conversions)

    __m128i dst = s[0];
    s[0] = _mm_add_epi64(s[0], inc); // NOLINT(portability-simd-intrinsics)

    for (int r = 0; r < Nr; ++r)
    {
        dst = _mm_aesenc_si128(dst, s[1]);
    }

    return uint128_from_m128i(dst);
}

#else

#warning "__AES__ not defined"

#endif
