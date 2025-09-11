// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Cast \c __m128i to different types
/**
* \file
* \author Steven Ward
*/

#pragma once

#include <bit>
#include <cstdint>
#include <immintrin.h>

#if defined(__x86_64__) && defined(__SSE2__)

static inline uint64_t
uint64_from_m128i(__m128i a) { return _mm_cvtsi128_si64(a); }

static inline __m128i
m128i_from_uint64(uint64_t hi, uint64_t lo) { return _mm_set_epi64x(hi, lo); }

#endif

#if defined(__SIZEOF_INT128__)

static inline __uint128_t
uint128_from_m128i(__m128i a) { return std::bit_cast<__uint128_t>(a); }

static inline __m128i
m128i_from_uint128(__uint128_t a) { return std::bit_cast<__m128i>(a); }

#endif
