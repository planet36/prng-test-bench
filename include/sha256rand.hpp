// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// sha256msg1rand, sha256msg2rand PRNGs
/**
\file
\author Steven Ward
*/

#pragma once

#include "def_urbg_class.hpp"
#include "simd-types.hpp"
#include "xxhprimes.hpp"

#include <immintrin.h>

#if defined(__SHA__)
DEF_URBG_CLASS(sha256msg1rand, SINGLE_ARG(std::array<simd128, 2>), uint64_t)
{
	static constexpr simd128 inc{.u64 {xxh_prime64[0], xxh_prime64[1]}};
	s[0].u64 += inc.u64;
	s[1].i64 = _mm_sha256msg1_epu32(s[0].i64, s[1].i64);
	return s[1].u64[0];
}

DEF_URBG_CLASS(sha256msg2rand, SINGLE_ARG(std::array<simd128, 2>), uint64_t)
{
	static constexpr simd128 inc{.u64 {xxh_prime64[0], xxh_prime64[1]}};
	s[0].u64 += inc.u64;
	s[1].i64 = _mm_sha256msg2_epu32(s[0].i64, s[1].i64);
	return s[1].u64[0];
}
#else
#warning "__SHA__ not defined"
#endif
