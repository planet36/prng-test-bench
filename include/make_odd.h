// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Make packed unsigned integers odd
/**
* \file
* \author Steven Ward
*/

#pragma once

#if defined(__x86_64__)

#include <immintrin.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SSE2__)

/// Make the packed unsigned 8-bit integers odd
static inline __m128i
mm_make_odd_epu8(const __m128i a)
{
    return _mm_or_si128(a, _mm_set1_epi8(1));
}

/// Make the packed unsigned 16-bit integers odd
static inline __m128i
mm_make_odd_epu16(const __m128i a)
{
    return _mm_or_si128(a, _mm_set1_epi16(1));
}

/// Make the packed unsigned 32-bit integers odd
static inline __m128i
mm_make_odd_epu32(const __m128i a)
{
    return _mm_or_si128(a, _mm_set1_epi32(1));
}

/// Make the packed unsigned 64-bit integers odd
static inline __m128i
mm_make_odd_epu64(const __m128i a)
{
    return _mm_or_si128(a, _mm_set1_epi64x(1));
}

#endif

#if defined(__AVX2__)

/// Make the packed unsigned 8-bit integers odd
static inline __m256i
mm256_make_odd_epu8(const __m256i a)
{
    return _mm256_or_si256(a, _mm256_set1_epi8(1));
}

/// Make the packed unsigned 16-bit integers odd
static inline __m256i
mm256_make_odd_epu16(const __m256i a)
{
    return _mm256_or_si256(a, _mm256_set1_epi16(1));
}

/// Make the packed unsigned 32-bit integers odd
static inline __m256i
mm256_make_odd_epu32(const __m256i a)
{
    return _mm256_or_si256(a, _mm256_set1_epi32(1));
}

/// Make the packed unsigned 64-bit integers odd
static inline __m256i
mm256_make_odd_epu64(const __m256i a)
{
    return _mm256_or_si256(a, _mm256_set1_epi64x(1));
}

#endif

#if defined(__AVX512F__)

/// Make the packed unsigned 8-bit integers odd
static inline __m512i
mm512_make_odd_epu8(const __m512i a)
{
    return _mm512_or_si512(a, _mm512_set1_epi8(1));
}

/// Make the packed unsigned 16-bit integers odd
static inline __m512i
mm512_make_odd_epu16(const __m512i a)
{
    return _mm512_or_si512(a, _mm512_set1_epi16(1));
}

/// Make the packed unsigned 32-bit integers odd
static inline __m512i
mm512_make_odd_epu32(const __m512i a)
{
    return _mm512_or_si512(a, _mm512_set1_epi32(1));
}

/// Make the packed unsigned 64-bit integers odd
static inline __m512i
mm512_make_odd_epu64(const __m512i a)
{
    return _mm512_or_si512(a, _mm512_set1_epi64(1));
}

#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif
