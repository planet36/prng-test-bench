// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Union of size 128 bits with integer data members
/**
\file
\author Steven Ward
*/

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <immintrin.h>

union alignas(__m128i) simd_128
{
	std::array<std::byte, 16 / sizeof(std::byte)> bytes;
	std::array<uint8_t, 16 / sizeof(uint8_t)> u8;
	std::array<uint16_t, 16 / sizeof(uint16_t)> u16;
	std::array<uint32_t, 16 / sizeof(uint32_t)> u32;
	std::array<uint64_t, 16 / sizeof(uint64_t)> u64;
#if defined(__SIZEOF_INT128__)
	std::array<__uint128_t, 16 / sizeof(__uint128_t)> u128;
#endif
	__m128i xmm;
};

static_assert(sizeof(simd_128) == 16);
