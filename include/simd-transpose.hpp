// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Transpose an array of 128-bit integer vectors (treating it as a square matrix of integers) using SSE2 intrinsics
/**
* \file
* \author Steven Ward
*/

#pragma once

#if defined(__x86_64__) && defined(__SSE2__)

#include "simd-array.hpp"

#include <immintrin.h>

static inline void
transpose([[maybe_unused]] arr_m128i<1>& x)
{
    // NOP
}

/// Transpose \a x (treating it as a 2x2 matrix of \c uint64_t) using SSE2 intrinsics
static void
transpose(arr_m128i<2>& x)
{
    const __m128i AB_0 = _mm_unpacklo_epi64(x[0], x[1]);
    const __m128i AB_1 = _mm_unpackhi_epi64(x[0], x[1]);

    x[0] = AB_0;
    x[1] = AB_1;
}

/// Transpose \a x (treating it as a 4x4 matrix of \c uint32_t) using SSE2 intrinsics
/**
* \sa https://randombit.net/bitbashing/posts/integer_matrix_transpose_in_sse2.html
*/
static void
transpose(arr_m128i<4>& x)
{
    const __m128i AB_01 = _mm_unpacklo_epi32(x[0], x[1]);
    const __m128i AB_23 = _mm_unpackhi_epi32(x[0], x[1]);
    const __m128i CD_01 = _mm_unpacklo_epi32(x[2], x[3]);
    const __m128i CD_23 = _mm_unpackhi_epi32(x[2], x[3]);

    x[0] = _mm_unpacklo_epi64(AB_01, CD_01); // ABCD_0
    x[1] = _mm_unpackhi_epi64(AB_01, CD_01); // ABCD_1
    x[2] = _mm_unpacklo_epi64(AB_23, CD_23); // ABCD_2
    x[3] = _mm_unpackhi_epi64(AB_23, CD_23); // ABCD_3
}

/// Transpose \a x (treating it as a 4x4 matrix of \c uint32_t) using the \c _MM_TRANSPOSE4_PS macro
/**
* \sa https://community.intel.com/t5/Intel-ISA-Extensions/4x4-matrix-transpose-using-sse2-intrinsics/m-p/785883#M353
* > Cast your __m128i variables into __m128 variables (using _mm_castsi128_ps),
* > use the macro _MM_TRANSPOSE_PS, then cast back using _mm_castps_si128.
*/
static void
transpose_macro(arr_m128i<4>& x)
{
    __m128 A = _mm_castsi128_ps(x[0]);
    __m128 B = _mm_castsi128_ps(x[1]);
    __m128 C = _mm_castsi128_ps(x[2]);
    __m128 D = _mm_castsi128_ps(x[3]);

    _MM_TRANSPOSE4_PS(A, B, C, D);

    x[0] = _mm_castps_si128(A);
    x[1] = _mm_castps_si128(B);
    x[2] = _mm_castps_si128(C);
    x[3] = _mm_castps_si128(D);
}

/// Transpose \a x (treating it as an 8x8 matrix of \c uint16_t) using SSE2 intrinsics
/**
* \sa https://stackoverflow.com/a/4951060/1892784
*/
static void
transpose(arr_m128i<8>& x)
{
    const __m128i AB_03 = _mm_unpacklo_epi16(x[0], x[1]);
    const __m128i AB_47 = _mm_unpackhi_epi16(x[0], x[1]);
    const __m128i CD_03 = _mm_unpacklo_epi16(x[2], x[3]);
    const __m128i CD_47 = _mm_unpackhi_epi16(x[2], x[3]);
    const __m128i EF_03 = _mm_unpacklo_epi16(x[4], x[5]);
    const __m128i EF_47 = _mm_unpackhi_epi16(x[4], x[5]);
    const __m128i GH_03 = _mm_unpacklo_epi16(x[6], x[7]);
    const __m128i GH_47 = _mm_unpackhi_epi16(x[6], x[7]);

    const __m128i ABCD_01 = _mm_unpacklo_epi32(AB_03, CD_03);
    const __m128i ABCD_23 = _mm_unpackhi_epi32(AB_03, CD_03);
    const __m128i ABCD_45 = _mm_unpacklo_epi32(AB_47, CD_47);
    const __m128i ABCD_67 = _mm_unpackhi_epi32(AB_47, CD_47);
    const __m128i EFGH_01 = _mm_unpacklo_epi32(EF_03, GH_03);
    const __m128i EFGH_23 = _mm_unpackhi_epi32(EF_03, GH_03);
    const __m128i EFGH_45 = _mm_unpacklo_epi32(EF_47, GH_47);
    const __m128i EFGH_67 = _mm_unpackhi_epi32(EF_47, GH_47);

    x[0] = _mm_unpacklo_epi64(ABCD_01, EFGH_01); // ABCDEFGH_0
    x[1] = _mm_unpackhi_epi64(ABCD_01, EFGH_01); // ABCDEFGH_1
    x[2] = _mm_unpacklo_epi64(ABCD_23, EFGH_23); // ABCDEFGH_2
    x[3] = _mm_unpackhi_epi64(ABCD_23, EFGH_23); // ABCDEFGH_3
    x[4] = _mm_unpacklo_epi64(ABCD_45, EFGH_45); // ABCDEFGH_4
    x[5] = _mm_unpackhi_epi64(ABCD_45, EFGH_45); // ABCDEFGH_5
    x[6] = _mm_unpacklo_epi64(ABCD_67, EFGH_67); // ABCDEFGH_6
    x[7] = _mm_unpackhi_epi64(ABCD_67, EFGH_67); // ABCDEFGH_7
}

/// Transpose \a x (treating it as a 16x16 matrix of \c uint8_t) using SSE2 intrinsics
/**
* \sa https://codereview.stackexchange.com/questions/295941/16x16-integer-matrix-transpose-using-sse2-intrinsics-in-c
*/
static void
transpose(arr_m128i<16>& x)
{
    const __m128i AB_07 = _mm_unpacklo_epi8(x[0x0], x[0x1]);
    const __m128i AB_8f = _mm_unpackhi_epi8(x[0x0], x[0x1]);
    const __m128i CD_07 = _mm_unpacklo_epi8(x[0x2], x[0x3]);
    const __m128i CD_8f = _mm_unpackhi_epi8(x[0x2], x[0x3]);
    const __m128i EF_07 = _mm_unpacklo_epi8(x[0x4], x[0x5]);
    const __m128i EF_8f = _mm_unpackhi_epi8(x[0x4], x[0x5]);
    const __m128i GH_07 = _mm_unpacklo_epi8(x[0x6], x[0x7]);
    const __m128i GH_8f = _mm_unpackhi_epi8(x[0x6], x[0x7]);
    const __m128i IJ_07 = _mm_unpacklo_epi8(x[0x8], x[0x9]);
    const __m128i IJ_8f = _mm_unpackhi_epi8(x[0x8], x[0x9]);
    const __m128i KL_07 = _mm_unpacklo_epi8(x[0xa], x[0xb]);
    const __m128i KL_8f = _mm_unpackhi_epi8(x[0xa], x[0xb]);
    const __m128i MN_07 = _mm_unpacklo_epi8(x[0xc], x[0xd]);
    const __m128i MN_8f = _mm_unpackhi_epi8(x[0xc], x[0xd]);
    const __m128i OP_07 = _mm_unpacklo_epi8(x[0xe], x[0xf]);
    const __m128i OP_8f = _mm_unpackhi_epi8(x[0xe], x[0xf]);

    const __m128i ABCD_03 = _mm_unpacklo_epi16(AB_07, CD_07);
    const __m128i ABCD_47 = _mm_unpackhi_epi16(AB_07, CD_07);
    const __m128i ABCD_8b = _mm_unpacklo_epi16(AB_8f, CD_8f);
    const __m128i ABCD_cf = _mm_unpackhi_epi16(AB_8f, CD_8f);
    const __m128i EFGH_03 = _mm_unpacklo_epi16(EF_07, GH_07);
    const __m128i EFGH_47 = _mm_unpackhi_epi16(EF_07, GH_07);
    const __m128i EFGH_8b = _mm_unpacklo_epi16(EF_8f, GH_8f);
    const __m128i EFGH_cf = _mm_unpackhi_epi16(EF_8f, GH_8f);
    const __m128i IJKL_03 = _mm_unpacklo_epi16(IJ_07, KL_07);
    const __m128i IJKL_47 = _mm_unpackhi_epi16(IJ_07, KL_07);
    const __m128i IJKL_8b = _mm_unpacklo_epi16(IJ_8f, KL_8f);
    const __m128i IJKL_cf = _mm_unpackhi_epi16(IJ_8f, KL_8f);
    const __m128i MNOP_03 = _mm_unpacklo_epi16(MN_07, OP_07);
    const __m128i MNOP_47 = _mm_unpackhi_epi16(MN_07, OP_07);
    const __m128i MNOP_8b = _mm_unpacklo_epi16(MN_8f, OP_8f);
    const __m128i MNOP_cf = _mm_unpackhi_epi16(MN_8f, OP_8f);

    const __m128i ABCDEFGH_01 = _mm_unpacklo_epi32(ABCD_03, EFGH_03);
    const __m128i ABCDEFGH_23 = _mm_unpackhi_epi32(ABCD_03, EFGH_03);
    const __m128i ABCDEFGH_45 = _mm_unpacklo_epi32(ABCD_47, EFGH_47);
    const __m128i ABCDEFGH_67 = _mm_unpackhi_epi32(ABCD_47, EFGH_47);
    const __m128i ABCDEFGH_89 = _mm_unpacklo_epi32(ABCD_8b, EFGH_8b);
    const __m128i ABCDEFGH_ab = _mm_unpackhi_epi32(ABCD_8b, EFGH_8b);
    const __m128i ABCDEFGH_cd = _mm_unpacklo_epi32(ABCD_cf, EFGH_cf);
    const __m128i ABCDEFGH_ef = _mm_unpackhi_epi32(ABCD_cf, EFGH_cf);
    const __m128i IJKLMNOP_01 = _mm_unpacklo_epi32(IJKL_03, MNOP_03);
    const __m128i IJKLMNOP_23 = _mm_unpackhi_epi32(IJKL_03, MNOP_03);
    const __m128i IJKLMNOP_45 = _mm_unpacklo_epi32(IJKL_47, MNOP_47);
    const __m128i IJKLMNOP_67 = _mm_unpackhi_epi32(IJKL_47, MNOP_47);
    const __m128i IJKLMNOP_89 = _mm_unpacklo_epi32(IJKL_8b, MNOP_8b);
    const __m128i IJKLMNOP_ab = _mm_unpackhi_epi32(IJKL_8b, MNOP_8b);
    const __m128i IJKLMNOP_cd = _mm_unpacklo_epi32(IJKL_cf, MNOP_cf);
    const __m128i IJKLMNOP_ef = _mm_unpackhi_epi32(IJKL_cf, MNOP_cf);

    x[0x0] = _mm_unpacklo_epi64(ABCDEFGH_01, IJKLMNOP_01); // ABCDEFGHIJKLMNOP_0
    x[0x1] = _mm_unpackhi_epi64(ABCDEFGH_01, IJKLMNOP_01); // ABCDEFGHIJKLMNOP_1
    x[0x2] = _mm_unpacklo_epi64(ABCDEFGH_23, IJKLMNOP_23); // ABCDEFGHIJKLMNOP_2
    x[0x3] = _mm_unpackhi_epi64(ABCDEFGH_23, IJKLMNOP_23); // ABCDEFGHIJKLMNOP_3
    x[0x4] = _mm_unpacklo_epi64(ABCDEFGH_45, IJKLMNOP_45); // ABCDEFGHIJKLMNOP_4
    x[0x5] = _mm_unpackhi_epi64(ABCDEFGH_45, IJKLMNOP_45); // ABCDEFGHIJKLMNOP_5
    x[0x6] = _mm_unpacklo_epi64(ABCDEFGH_67, IJKLMNOP_67); // ABCDEFGHIJKLMNOP_6
    x[0x7] = _mm_unpackhi_epi64(ABCDEFGH_67, IJKLMNOP_67); // ABCDEFGHIJKLMNOP_7
    x[0x8] = _mm_unpacklo_epi64(ABCDEFGH_89, IJKLMNOP_89); // ABCDEFGHIJKLMNOP_8
    x[0x9] = _mm_unpackhi_epi64(ABCDEFGH_89, IJKLMNOP_89); // ABCDEFGHIJKLMNOP_9
    x[0xa] = _mm_unpacklo_epi64(ABCDEFGH_ab, IJKLMNOP_ab); // ABCDEFGHIJKLMNOP_a
    x[0xb] = _mm_unpackhi_epi64(ABCDEFGH_ab, IJKLMNOP_ab); // ABCDEFGHIJKLMNOP_b
    x[0xc] = _mm_unpacklo_epi64(ABCDEFGH_cd, IJKLMNOP_cd); // ABCDEFGHIJKLMNOP_c
    x[0xd] = _mm_unpackhi_epi64(ABCDEFGH_cd, IJKLMNOP_cd); // ABCDEFGHIJKLMNOP_d
    x[0xe] = _mm_unpacklo_epi64(ABCDEFGH_ef, IJKLMNOP_ef); // ABCDEFGHIJKLMNOP_e
    x[0xf] = _mm_unpackhi_epi64(ABCDEFGH_ef, IJKLMNOP_ef); // ABCDEFGHIJKLMNOP_f
}

#endif
