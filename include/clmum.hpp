// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Carry-less MUltiply and Mix ("MUM") functions
/**
\file
\author Steven Ward
\sa https://en.wikipedia.org/wiki/CLMUL_instruction_set
\sa https://www.felixcloutier.com/x86/pclmulqdq
*/

#pragma once

#include "mum.hpp"

#include <cstdint>
#include <immintrin.h>

#if defined(__PCLMUL__)
inline void
clmul(__m128i& a)
{
	// https://clang.llvm.org/doxygen/____wmmintrin__pclmul_8h.html
	// https://github.com/gcc-mirror/gcc/blob/master/gcc/config/i386/wmmintrin.h#L103
	// __m128i _mm_clmulepi64_si128(__m128i a, __m128i b, int imm8)
	// imm8: 0x00 => b[0] * a[0]
	// imm8: 0x01 => b[0] * a[1]
	// imm8: 0x10 => b[1] * a[0]
	// imm8: 0x11 => b[1] * a[1]

	// MSB in result is always 0
	a = _mm_clmulepi64_si128(a, a, 0x10);
}

inline uint64_t
clmumx(__m128i a)
{
	clmul(a);
	return _mm_extract_epi64(a, 1) ^ _mm_extract_epi64(a, 0);
}

inline uint64_t
clmuma(__m128i a)
{
	clmul(a);
	return _mm_extract_epi64(a, 1) + _mm_extract_epi64(a, 0);
}

inline uint64_t
clmums(__m128i a)
{
	clmul(a);
	return _mm_extract_epi64(a, 1) - _mm_extract_epi64(a, 0);
}

void
clmul(uint64_t& hi, uint64_t& lo)
{
	__m128i result = _mm_set_epi64x(hi, lo); // order of hi, lo doesn't matter

	clmul(result);

	hi = _mm_extract_epi64(result, 1);
	lo = _mm_extract_epi64(result, 0);
}

inline uint64_t
clmumx(uint64_t a, uint64_t b)
{
	clmul(a, b);
	return a ^ b;
}

inline uint64_t
clmuma(uint64_t a, uint64_t b)
{
	clmul(a, b);
	return a + b;
}

inline uint64_t
clmums(uint64_t a, uint64_t b)
{
	clmul(a, b);
	return a - b;
}
#else
#warning "__PCLMUL__ not defined"
#endif
