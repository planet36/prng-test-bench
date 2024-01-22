// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// aesenclastrand, aesdeclastrand PRNGs
/**
\file
\author Steven Ward
\sa https://github.com/dragontamer/AESRand/blob/master/AESRand_Paper/AESRand.hpp
*/

#pragma once

#include "byteprimes.hpp"
#include "def_urbg_class.hpp"
#include "simd-types.hpp"

#include <cstdint>
#include <immintrin.h>

#if defined(__AES__)
DEF_URBG_CLASS(aesenclastrand, simd128, uint64_t)
{
	static constexpr simd128 roundKey{.u64{byteprimes[0], byteprimes[1]}};
	s.u64 += roundKey.u64;
	const __m128i penultimate = _mm_aesenc_si128(s.i64, roundKey.i64);
	const __m128i result = _mm_aesenclast_si128(penultimate, roundKey.i64);

	//return result[0]; // failed at 2 GB
	//return result[1]; // failed at 8 GB
	return result[0] ^ result[1]; // good thru 1 TB
	//return result[0] + result[1]; // good thru 1 TB
	//return result[0] - result[1]; // good thru 1 TB
	//return result[1] - result[0]; // good thru 1 TB
}

DEF_URBG_CLASS(aesdeclastrand, simd128, uint64_t)
{
	static constexpr simd128 roundKey{.u64{byteprimes[0], byteprimes[1]}};
	s.u64 += roundKey.u64;
	const __m128i penultimate = _mm_aesdec_si128(s.i64, roundKey.i64);
	const __m128i result = _mm_aesdeclast_si128(penultimate, roundKey.i64);

	//return result[0]; // failed at 256 MB
	//return result[1]; // failed at 256 MB
	return result[0] ^ result[1]; // good thru 1 TB
	//return result[0] + result[1]; // good thru 1 TB
	//return result[0] - result[1]; // good thru 1 TB
	//return result[1] - result[0]; // good thru 1 TB
}
#else
#warning "__AES__ not defined"
#endif
