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
#include "simd-types.hpp"
#include "xxhprimes.hpp"

#include <cstdint>
#include <immintrin.h>

#if defined(__AES__)
DEF_URBG_CLASS(aesimcrand, simd128, uint64_t)
{
	static constexpr simd128 inc{.u64arr{xxh_prime64[0], xxh_prime64[1]}};
	static constexpr simd128 inc2{.u64arr{xxh_prime64[2], xxh_prime64[3]}};
	s.u64vec += inc.u64vec;
	__m128i result = __builtin_ia32_aesimc128(s.i64vec);
	result += inc2.i64vec;
	result = __builtin_ia32_aesimc128(result);
	result += inc.i64vec;
	result = __builtin_ia32_aesimc128(result);

	//return result[0]; // fails at 4 GB
	//return result[1]; // fails at 4 GB
	//return result[0] ^ result[1]; // fails at 64 GB
	//return result[0] + result[1]; // fails at 64 GB
	//return result[0] - result[1]; // fails at 64 GB
	return result[1] - result[0]; // fails at 64 GB
}
#else
#warning "__AES__ not defined"
#endif
