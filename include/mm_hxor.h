// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Get the horizontal XOR of packed unsigned integers
/**
* \file
* \author Steven Ward
*/

#pragma once

#if defined(__x86_64__) && defined(__SSE4_1__)

#include <immintrin.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Get the horizontal XOR of packed unsigned 32-bit integers
static inline uint32_t
mm_hxor_epu32(const __m128i a)
{
    return (uint32_t)_mm_extract_epi32(a, 0) ^ (uint32_t)_mm_extract_epi32(a, 1) ^
           (uint32_t)_mm_extract_epi32(a, 2) ^ (uint32_t)_mm_extract_epi32(a, 3);
}

/// Get the horizontal XOR of packed unsigned 64-bit integers
static inline uint64_t
mm_hxor_epu64(const __m128i a)
{
    return (uint64_t)_mm_extract_epi64(a, 0) ^ (uint64_t)_mm_extract_epi64(a, 1);
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif
