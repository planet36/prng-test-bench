// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Functions to test equality of __m128i
/**
* \file
* \author Steven Ward
* \sa http://0x80.pl/notesen/2021-02-02-all-bytes-in-reg-are-equal.html
*/

#pragma once

#if defined(__x86_64__)

#include <immintrin.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Determine if the both of the given __m128i are equal
static bool
mm_equal(const __m128i a, const __m128i b)
{
    return memcmp(&a, &b, sizeof(__m128i)) == 0;
}

#if defined(__AVX2__)

/// Determine if all packed 8-bit integers of the given __m128i are equal
static bool
mm_all_equal_epi8(const __m128i a)
{
    return mm_equal(a, _mm_broadcastb_epi8(a));
}

/// Determine if all packed 16-bit integers of the given __m128i are equal
static bool
mm_all_equal_epi16(const __m128i a)
{
    return mm_equal(a, _mm_broadcastw_epi16(a));
}

/// Determine if all packed 32-bit integers of the given __m128i are equal
static bool
mm_all_equal_epi32(const __m128i a)
{
    return mm_equal(a, _mm_broadcastd_epi32(a));
}

/// Determine if all packed 64-bit integers of the given __m128i are equal
static bool
mm_all_equal_epi64(const __m128i a)
{
    return mm_equal(a, _mm_broadcastq_epi64(a));
}

#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif
