// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// sha256msg1rand, sha256msg2rand PRNGs
/**
\file
\author Steven Ward
*/

#pragma once

#include "def_urbg_class.hpp"
#include "simd-union.hpp"
#include "xxhprimes.hpp"

#include <array>
#include <cstdint>
#include <immintrin.h>

#if defined(__SHA__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
DEF_URBG_CLASS(sha256msg1rand, SINGLE_ARG(std::array<__m128i, 2>), __uint128_t)
{
	const __m128i inc = _mm_set_epi64x(xxh_prime64[1], xxh_prime64[0]);
	const __m128i inc2 = _mm_set_epi64x(xxh_prime64[3], xxh_prime64[2]);
	// XXX: Only the least significant 32 bits in s[1] are used.
	const __m128i result = _mm_sha256msg1_epu32(s[0], s[1]);
	s[0] = _mm_add_epi64(s[0], inc);
	s[1] = _mm_add_epi64(s[1], inc2);
	return simd_128{.xmm = result}.u128[0];
}
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
DEF_URBG_CLASS(sha256msg2rand, SINGLE_ARG(std::array<__m128i, 2>), __uint128_t)
{
	const __m128i inc = _mm_set_epi64x(xxh_prime64[1], xxh_prime64[0]);
	const __m128i inc2 = _mm_set_epi64x(xxh_prime64[3], xxh_prime64[2]);
	// XXX: Only the least significant 32 bits in s[1] are used.
	const __m128i result = _mm_sha256msg2_epu32(s[0], s[1]);
	s[0] = _mm_add_epi64(s[0], inc);
	s[1] = _mm_add_epi64(s[1], inc2);
	return simd_128{.xmm = result}.u128[0];
}
#pragma GCC diagnostic pop
#else
#warning "__SHA__ not defined"
#endif
