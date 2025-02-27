// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Unions with data members that are standard integers and SIMD types
/**
\file
\author Steven Ward
*/

#pragma once

#include "simd-array.hpp"

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
	arr_m128i<32 / sizeof(__m128i)> xmm;
	__m256i ymm;
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
	arr_m128i<64 / sizeof(__m128i)> xmm;
	arr_m256i<64 / sizeof(__m256i)> ymm;
	__m512i zmm;
};

static_assert(sizeof(simd_512) == 64);

template <size_t N>
inline auto
arr_m128i_to_simd_128(const arr_m128i<N>& x)
{
	std::array<simd_128, N> y;

	// __m128i to simd_128
	// https://gcc.gnu.org/onlinedocs/gcc/Loop-Specific-Pragmas.html#index-pragma-GCC-unroll-n
#pragma GCC unroll N
	for (size_t i = 0; i < N; ++i)
	{
		y[i].xmm = x[i];
	}

	return y;
}

template <size_t N>
inline auto
arr_m256i_to_simd_256(const arr_m256i<N>& x)
{
	std::array<simd_256, N> y;

	// __m256i to simd_256
	// https://gcc.gnu.org/onlinedocs/gcc/Loop-Specific-Pragmas.html#index-pragma-GCC-unroll-n
#pragma GCC unroll N
	for (size_t i = 0; i < N; ++i)
	{
		y[i].xmm = x[i];
	}

	return y;
}

template <size_t N>
inline auto
arr_m512i_to_simd_512(const arr_m512i<N>& x)
{
	std::array<simd_512, N> y;

	// __m512i to simd_512
	// https://gcc.gnu.org/onlinedocs/gcc/Loop-Specific-Pragmas.html#index-pragma-GCC-unroll-n
#pragma GCC unroll N
	for (size_t i = 0; i < N; ++i)
	{
		y[i].xmm = x[i];
	}

	return y;
}

template <size_t N>
inline auto
arr_simd_128_to_m128i(const std::array<simd_128, N>& x)
{
	arr_m128i<N> y;

	// simd_128 to __m128i
	// https://gcc.gnu.org/onlinedocs/gcc/Loop-Specific-Pragmas.html#index-pragma-GCC-unroll-n
#pragma GCC unroll N
	for (size_t i = 0; i < N; ++i)
	{
		y[i] = x[i].xmm;
	}

	return y;
}

template <size_t N>
inline auto
arr_simd_256_to_m256i(const std::array<simd_256, N>& x)
{
	arr_m256i<N> y;

	// simd_256 to __m256i
	// https://gcc.gnu.org/onlinedocs/gcc/Loop-Specific-Pragmas.html#index-pragma-GCC-unroll-n
#pragma GCC unroll N
	for (size_t i = 0; i < N; ++i)
	{
		y[i] = x[i].xmm;
	}

	return y;
}

template <size_t N>
inline auto
arr_simd_512_to_m512i(const std::array<simd_512, N>& x)
{
	arr_m512i<N> y;

	// simd_512 to __m512i
	// https://gcc.gnu.org/onlinedocs/gcc/Loop-Specific-Pragmas.html#index-pragma-GCC-unroll-n
#pragma GCC unroll N
	for (size_t i = 0; i < N; ++i)
	{
		y[i] = x[i].xmm;
	}

	return y;
}
