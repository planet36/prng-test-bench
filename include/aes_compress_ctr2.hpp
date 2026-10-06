// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// PRNGs that use AES instructions
/**
* \file
* \author Steven Ward
*
* Each PRNG compresses 2 counters into 1 output with rounds of AES.  The number after
* "compress_r" in its name is the number of rounds.  In the state, s[0] is the first
* counter and s[1] is the second counter.
*/

#pragma once

#if defined(__AES__)

#include "mm_cast.hpp"
#include "simd_compress.hpp"
#include "simd_types.hpp"
#include "urbg_base_class.hpp"
#include "wyprimes.hpp"

#include <immintrin.h>

#if !defined(__SIZEOF_INT128__)
#error "__SIZEOF_INT128__ not defined"
#endif

/// Advance the counters in \a s
inline void
aes_compress_ctr2_advance(simd_arr_t<2>& s)
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
        wyprimes::vec128_01(),
        wyprimes::vec128_23(),
    };

    s[0] = _mm_add_epi64(s[0], inc[0]); // NOLINT(portability-simd-intrinsics)
    s[1] = _mm_add_epi64(s[1], inc[1]); // NOLINT(portability-simd-intrinsics)
}

DEF_URBG_SUBCLASS(aes_compress_r2_ctr2_128, simd_arr_t<2>, __uint128_t)

/// Prepare the initial state
void
aes_compress_r2_ctr2_128::init()
{
}

aes_compress_r2_ctr2_128::result_type
aes_compress_r2_ctr2_128::next()
{
    aes_compress_ctr2_advance(s);
    return uint128_from_m128i(simd_compress_aes_enc_r2(s[0], s[1]));
}

DEF_URBG_SUBCLASS(aes_compress_r3_ctr2_128, simd_arr_t<2>, __uint128_t)

/// Prepare the initial state
void
aes_compress_r3_ctr2_128::init()
{
}

aes_compress_r3_ctr2_128::result_type
aes_compress_r3_ctr2_128::next()
{
    aes_compress_ctr2_advance(s);
    return uint128_from_m128i(simd_compress_aes_enc_r3(s[0], s[1]));
}

#else

#warning "__AES__ not defined"

#endif
