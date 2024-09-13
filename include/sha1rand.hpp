// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// sha1msg1rand, sha1msg2rand PRNGs
/**
\file
\author Steven Ward
*/

#pragma once

#include "def_urbg_class.hpp"
#include "simd-types.hpp"
#include "xxhprimes.hpp"

#include <array>
#include <cstdint>
#include <immintrin.h>

#if defined(__SHA__)
DEF_URBG_CLASS(sha1msg1rand, SINGLE_ARG(std::array<simd128, 2>), uint64_t)
{
	static constexpr simd128 inc{.u64 {xxh_prime64[0], xxh_prime64[1]}};
	s[0].u64 += inc.u64;
	s[1].i64 = _mm_sha1msg1_epu32(s[0].i64, s[1].i64);
	return s[1].u64[0];
}

DEF_URBG_CLASS(sha1msg2rand, SINGLE_ARG(std::array<simd128, 2>), uint64_t)
{
	static constexpr simd128 inc{.u64 {xxh_prime64[0], xxh_prime64[1]}};
	s[0].u64 += inc.u64;
	s[1].i64 = _mm_sha1msg2_epu32(s[0].i64, s[1].i64);
	return s[1].u64[0];
}
#else
#warning "__SHA__ not defined"
#endif
