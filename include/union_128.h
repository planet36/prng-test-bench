// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

#pragma once

#include <immintrin.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

union union_128
{
	uint8_t u8[16];
	uint16_t u16[8];
	uint32_t u32[4];
	uint64_t u64[2];
#if defined(__SIZEOF_INT128__)
	__int128_t i128;
	__uint128_t u128;
#endif
	__m128i xmm;
};

#ifdef __cplusplus
} // extern "C"
#endif
