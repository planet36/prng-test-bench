// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// A PRNG that uses SHA-256 instructions
/**
* \file
* \author Steven Ward
*/

#pragma once

#if defined(__SHA__)

#include "mm_cast.hpp"
#include "sha2.h"
#include "simd_types.hpp"
#include "urbg_base_class.hpp"
#include "wyprimes.hpp"

#include <immintrin.h>

/**
* \sa https://www.felixcloutier.com/x86/sha256rnds2
* > Note that only the two lower dwords of XMM0 are used by the instruction.
*/
// NOLINTBEGIN(cppcoreguidelines-narrowing-conversions)
static const simd_arr_t<32> sha256_round_constants{
    _mm_setr_epi32(SHA_256_C[ 0], SHA_256_C[ 1], 0, 0),
    _mm_setr_epi32(SHA_256_C[ 2], SHA_256_C[ 3], 0, 0),
    _mm_setr_epi32(SHA_256_C[ 4], SHA_256_C[ 5], 0, 0),
    _mm_setr_epi32(SHA_256_C[ 6], SHA_256_C[ 7], 0, 0),
    _mm_setr_epi32(SHA_256_C[ 8], SHA_256_C[ 9], 0, 0),
    _mm_setr_epi32(SHA_256_C[10], SHA_256_C[11], 0, 0),
    _mm_setr_epi32(SHA_256_C[12], SHA_256_C[13], 0, 0),
    _mm_setr_epi32(SHA_256_C[14], SHA_256_C[15], 0, 0),
    _mm_setr_epi32(SHA_256_C[16], SHA_256_C[17], 0, 0),
    _mm_setr_epi32(SHA_256_C[18], SHA_256_C[19], 0, 0),
    _mm_setr_epi32(SHA_256_C[20], SHA_256_C[21], 0, 0),
    _mm_setr_epi32(SHA_256_C[22], SHA_256_C[23], 0, 0),
    _mm_setr_epi32(SHA_256_C[24], SHA_256_C[25], 0, 0),
    _mm_setr_epi32(SHA_256_C[26], SHA_256_C[27], 0, 0),
    _mm_setr_epi32(SHA_256_C[28], SHA_256_C[29], 0, 0),
    _mm_setr_epi32(SHA_256_C[30], SHA_256_C[31], 0, 0),
    _mm_setr_epi32(SHA_256_C[32], SHA_256_C[33], 0, 0),
    _mm_setr_epi32(SHA_256_C[34], SHA_256_C[35], 0, 0),
    _mm_setr_epi32(SHA_256_C[36], SHA_256_C[37], 0, 0),
    _mm_setr_epi32(SHA_256_C[38], SHA_256_C[39], 0, 0),
    _mm_setr_epi32(SHA_256_C[40], SHA_256_C[41], 0, 0),
    _mm_setr_epi32(SHA_256_C[42], SHA_256_C[43], 0, 0),
    _mm_setr_epi32(SHA_256_C[44], SHA_256_C[45], 0, 0),
    _mm_setr_epi32(SHA_256_C[46], SHA_256_C[47], 0, 0),
    _mm_setr_epi32(SHA_256_C[48], SHA_256_C[49], 0, 0),
    _mm_setr_epi32(SHA_256_C[50], SHA_256_C[51], 0, 0),
    _mm_setr_epi32(SHA_256_C[52], SHA_256_C[53], 0, 0),
    _mm_setr_epi32(SHA_256_C[54], SHA_256_C[55], 0, 0),
    _mm_setr_epi32(SHA_256_C[56], SHA_256_C[57], 0, 0),
    _mm_setr_epi32(SHA_256_C[58], SHA_256_C[59], 0, 0),
    _mm_setr_epi32(SHA_256_C[60], SHA_256_C[61], 0, 0),
    _mm_setr_epi32(SHA_256_C[62], SHA_256_C[63], 0, 0),
};
// NOLINTEND(cppcoreguidelines-narrowing-conversions)

/// 2*4 rounds of SHA-256
/**
* \sa https://www.felixcloutier.com/x86/sha256rnds2
* \sa https://github.com/noloader/SHA-Intrinsics/blob/master/sha256-x86.c
*/
[[nodiscard]] static __m128i
sha256_rnds2x4(__m128i a, __m128i b)
{
    a = _mm_sha256rnds2_epu32(a, b, sha256_round_constants[0]);
    b = _mm_sha256rnds2_epu32(b, a, sha256_round_constants[1]);
    a = _mm_sha256rnds2_epu32(a, b, sha256_round_constants[2]);
    b = _mm_sha256rnds2_epu32(b, a, sha256_round_constants[3]);
    return b;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
DEF_URBG_SUBCLASS(sha256_ctr_128, __m128i, __uint128_t)
#pragma GCC diagnostic pop

/// Prepare the initial state
void
sha256_ctr_128::init()
{
}

sha256_ctr_128::result_type
sha256_ctr_128::next()
{
    const __m128i inc = wyprimes::vec128_01();

    __m128i dst = s;
    s = _mm_add_epi64(s, inc);
    dst = sha256_rnds2x4(dst, s);

    return uint128_from_m128i(dst);
}

#else

#warning "__SHA__ not defined"

#endif
