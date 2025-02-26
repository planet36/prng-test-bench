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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
	__m128i xmm;
#pragma GCC diagnostic pop
};

static_assert(sizeof(simd_128) == 16);

union alignas(__m256i) simd_256
{
	std::array<std::byte, 32 / sizeof(std::byte)> bytes;
	std::array<uint8_t, 32 / sizeof(uint8_t)> u8;
	std::array<uint16_t, 32 / sizeof(uint16_t)> u16;
	std::array<uint32_t, 32 / sizeof(uint32_t)> u32;
	std::array<uint64_t, 32 / sizeof(uint64_t)> u64;
#if defined(__SIZEOF_INT128__)
	std::array<__uint128_t, 32 / sizeof(__uint128_t)> u128;
#endif
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
	std::array<__m128i, 32 / sizeof(__m128i)> xmm;
	__m256i ymm;
#pragma GCC diagnostic pop
};

static_assert(sizeof(simd_256) == 32);

union alignas(__m512i) simd_512
{
	std::array<std::byte, 64 / sizeof(std::byte)> bytes;
	std::array<uint8_t, 64 / sizeof(uint8_t)> u8;
	std::array<uint16_t, 64 / sizeof(uint16_t)> u16;
	std::array<uint32_t, 64 / sizeof(uint32_t)> u32;
	std::array<uint64_t, 64 / sizeof(uint64_t)> u64;
#if defined(__SIZEOF_INT128__)
	std::array<__uint128_t, 64 / sizeof(__uint128_t)> u128;
#endif
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
	std::array<__m128i, 64 / sizeof(__m128i)> xmm;
	std::array<__m256i, 64 / sizeof(__m256i)> ymm;
	__m512i zmm;
#pragma GCC diagnostic pop
};

static_assert(sizeof(simd_512) == 64);
