// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// AES InvMixColumns PRNG
/**
\file
\author Steven Ward
\sa https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#text=aesimc
\sa https://www.intel.com/content/dam/doc/white-paper/advanced-encryption-standard-new-instructions-set-paper.pdf
\sa https://www.felixcloutier.com/x86/aesimc
*/

#pragma once

#include "def_urbg_class.hpp"
#include "union_128.h"
#include "xxhprimes.hpp"

#include <cstdint>
#include <immintrin.h>

#if defined(__AES__)
DEF_URBG_CLASS(aesimcrand, __m128i, __uint128_t)
{
	const __m128i inc = _mm_set_epi64x(xxh_prime64[1], xxh_prime64[0]);
	const __m128i inc2 = _mm_set_epi64x(xxh_prime64[3], xxh_prime64[2]);
	s = _mm_add_epi64(s, inc);
	__m128i result = __builtin_ia32_aesimc128(s);
	result = _mm_add_epi64(result, inc2);
	result = __builtin_ia32_aesimc128(result);
	result = _mm_add_epi64(result, inc);
	result = __builtin_ia32_aesimc128(result);
	return union_128{.xmm = result}.u128;

	//return result[0]; // fails at 4 GB
	//return result[1]; // fails at 4 GB
	//return result[0] ^ result[1]; // fails at 64 GB
	//return result[0] + result[1]; // fails at 64 GB
	//return result[0] - result[1]; // fails at 64 GB
	//return result[1] - result[0]; // fails at 64 GB
}
#else
#warning "__AES__ not defined"
#endif
