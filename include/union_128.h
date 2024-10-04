// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Union of size 128 bits with integer data members
/**
\file
\author Steven Ward
*/

#pragma once

#include <immintrin.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

union union_128
{
	uint8_t u8[16 / sizeof(uint8_t)];
	uint16_t u16[16 / sizeof(uint16_t)];
	uint32_t u32[16 / sizeof(uint32_t)];
	uint64_t u64[16 / sizeof(uint64_t)];
#if defined(__SIZEOF_INT128__)
	__int128_t i128;
	__uint128_t u128;
#endif
	__m128i xmm;
};

#ifdef __cplusplus
} // extern "C"
#endif
