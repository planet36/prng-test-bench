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

static inline auto
uint64_from_m128i(__m128i a)
{
    return static_cast<uint64_t>(_mm_cvtsi128_si64(a));
}

// same as m128i_from_uint128
static inline auto
m128i_from_uint64(uint64_t lo, uint64_t hi)
{
    return _mm_set_epi64x(hi, lo);
}

#endif

#if defined(__SIZEOF_INT128__)

static inline auto
uint128_from_m128i(__m128i a)
{
    return std::bit_cast<__uint128_t>(a);
}

// same as m128i_from_uint64
static inline auto
m128i_from_uint128(__uint128_t a)
{
    return std::bit_cast<__m128i>(a);
}

#endif
