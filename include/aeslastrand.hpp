// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// aesenclastrand, aesdeclastrand PRNGs
/**
\file
\author Steven Ward
\sa https://github.com/dragontamer/AESRand/blob/master/AESRand_Paper/AESRand.hpp
\sa https://www.intel.com/content/dam/doc/white-paper/advanced-encryption-standard-new-instructions-set-paper.pdf
\sa https://www.felixcloutier.com/x86/aesenc
\sa https://www.felixcloutier.com/x86/aesdec
*/

#pragma once

#include "byteprimes.hpp"
#include "def_urbg_class.hpp"
#include "union_128.h"

#include <cstdint>
#include <immintrin.h>

#if defined(__AES__)
DEF_URBG_CLASS(aesenclastrand, __m128i, __uint128_t)
{
	const __m128i roundKey = _mm_set_epi64x(byteprimes[1], byteprimes[0]);
	s = _mm_add_epi64(s, roundKey);
	__m128i result = _mm_aesenc_si128(s, roundKey);
	result = _mm_aesenclast_si128(result, roundKey);
	return union_128{.xmm = result}.u128;

	//return result[0]; // failed at 2 GB
	//return result[1]; // failed at 8 GB
	//return result[0] ^ result[1]; // good thru 1 TB
	//return result[0] + result[1]; // good thru 1 TB
	//return result[0] - result[1]; // good thru 1 TB
	//return result[1] - result[0]; // good thru 1 TB
}

DEF_URBG_CLASS(aesdeclastrand, __m128i, __uint128_t)
{
	const __m128i roundKey = _mm_set_epi64x(byteprimes[1], byteprimes[0]);
	s = _mm_add_epi64(s, roundKey);
	__m128i result = _mm_aesdec_si128(s, roundKey);
	result = _mm_aesdeclast_si128(result, roundKey);
	return union_128{.xmm = result}.u128;

	//return result[0]; // failed at 256 MB
	//return result[1]; // failed at 256 MB
	//return result[0] ^ result[1]; // good thru 1 TB
	//return result[0] + result[1]; // good thru 1 TB
	//return result[0] - result[1]; // good thru 1 TB
	//return result[1] - result[0]; // good thru 1 TB
}
#else
#warning "__AES__ not defined"
#endif
