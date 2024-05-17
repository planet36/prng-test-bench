// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// SIMD types used with intrinsic/built-in functions
/**
\file
\author Steven Ward
\sa https://gcc.gnu.org/onlinedocs/gcc/Vector-Extensions.html
\sa https://clang.llvm.org/docs/LanguageExtensions.html#vectors-and-extended-vectors
\sa https://gcc.gnu.org/onlinedocs/gcc/Common-Type-Attributes.html
\sa https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html
\sa https://www.intel.com/content/www/us/en/docs/cpp-compiler/developer-guide-reference/2021-8/details-about-sse-intrinsics.html
\sa https://github.com/gcc-mirror/gcc/blob/master/gcc/config/i386/xmmintrin.h
\sa https://github.com/gcc-mirror/gcc/blob/master/gcc/config/i386/emmintrin.h
\sa https://github.com/gcc-mirror/gcc/blob/master/gcc/config/i386/avxintrin.h
\sa https://gcc.gnu.org/onlinedocs/gcc/x86-Built-in-Functions.html
\sa https://developer.arm.com/documentation/den0018/a/NEON-Intrinsics/Vector-data-types-for-NEON-intrinsics?lang=en
*/

#pragma once

#include <array>
#include <cstring>
#include <immintrin.h>

#define DECLARE_SIMD_TYPE(T, TNAME, N)                                   \
	using TNAME##x##N = [[gnu::vector_size(sizeof(T) * (N)),             \
	                      gnu::warn_if_not_aligned(sizeof(T) * (N))]] T; \
	static_assert(sizeof(TNAME##x##N) == sizeof(T) * (N));               \
	static_assert(alignof(TNAME##x##N) == sizeof(T) * (N));              \
	auto to_array(const TNAME##x##N& v)                                  \
	{                                                                    \
		std::array<T, (N)> result{};                                     \
		(void)std::memcpy(result.data(), &v, sizeof(T) * (N));           \
		return result;                                                   \
	}

// 128-bit types

DECLARE_SIMD_TYPE(signed char  , i8, 16)
DECLARE_SIMD_TYPE(unsigned char, u8, 16)

DECLARE_SIMD_TYPE(short         , i16, 8)
DECLARE_SIMD_TYPE(unsigned short, u16, 8)

DECLARE_SIMD_TYPE(int         , i32, 4)
DECLARE_SIMD_TYPE(unsigned int, u32, 4)
DECLARE_SIMD_TYPE(float       , f32, 4) // == __m128

DECLARE_SIMD_TYPE(long long         , i64, 2) // == __m128i
DECLARE_SIMD_TYPE(unsigned long long, u64, 2)
DECLARE_SIMD_TYPE(double            , f64, 2) // == __m128d

union simd128
{
	i8x16 i8vec; std::array<signed char  , 16> i8arr;
	u8x16 u8vec; std::array<unsigned char, 16> u8arr;

	i16x8 i16vec; std::array<short         , 8> i16arr;
	u16x8 u16vec; std::array<unsigned short, 8> u16arr;

	i32x4 i32vec; std::array<int         , 4> i32arr;
	u32x4 u32vec; std::array<unsigned int, 4> u32arr;
	f32x4 f32vec; std::array<float       , 4> f32arr;

	i64x2 i64vec; std::array<long long         , 2> i64arr;
	u64x2 u64vec; std::array<unsigned long long, 2> u64arr;
	f64x2 f64vec; std::array<double            , 2> f64arr;

	// An aggregate class has no user-declared or inherited constructors.
	// https://en.cppreference.com/w/cpp/language/aggregate_initialization
	// If ctors are added, then designated initializers can't be used.

	// assignment operators
	constexpr simd128& operator=(const __m128&  x) { f32vec = x; return *this; }
	constexpr simd128& operator=(const __m128i& x) { i64vec = x; return *this; }
	constexpr simd128& operator=(const __m128d& x) { f64vec = x; return *this; }

	// conversion operators
	explicit constexpr operator __m128  () const { return f32vec; }
	explicit constexpr operator __m128i () const { return i64vec; }
	explicit constexpr operator __m128d () const { return f64vec; }

	// named ctors
	static constexpr simd128 from_xmm(const __m128&  x) { return {.f32vec = x}; }
	static constexpr simd128 from_xmm(const __m128i& x) { return {.i64vec = x}; }
	static constexpr simd128 from_xmm(const __m128d& x) { return {.f64vec = x}; }

	constexpr bool operator==(const simd128& that) const
	{
		return std::memcmp(this, &that, sizeof(simd128)) == 0;
	}

	constexpr bool operator!=(const simd128& that) const
	{
		return !(*this == that);
	}
};

static_assert(sizeof(simd128) == 16);
static_assert(alignof(simd128) == 16);

// 256-bit types

DECLARE_SIMD_TYPE(signed char  , i8, 32)
DECLARE_SIMD_TYPE(unsigned char, u8, 32)

DECLARE_SIMD_TYPE(short         , i16, 16)
DECLARE_SIMD_TYPE(unsigned short, u16, 16)

DECLARE_SIMD_TYPE(int         , i32, 8)
DECLARE_SIMD_TYPE(unsigned int, u32, 8)
DECLARE_SIMD_TYPE(float       , f32, 8) // == __m256

DECLARE_SIMD_TYPE(long long         , i64, 4) // == __m256i
DECLARE_SIMD_TYPE(unsigned long long, u64, 4)
DECLARE_SIMD_TYPE(double            , f64, 4) // == __m256d

union simd256
{
	i8x32 i8vec; std::array<signed char  , 32> i8arr;
	u8x32 u8vec; std::array<unsigned char, 32> u8arr;

	i16x16 i16vec; std::array<short         , 16> i16arr;
	u16x16 u16vec; std::array<unsigned short, 16> u16arr;

	i32x8 i32vec; std::array<int         , 8> i32arr;
	u32x8 u32vec; std::array<unsigned int, 8> u32arr;
	f32x8 f32vec; std::array<float       , 8> f32arr;

	i64x4 i64vec; std::array<long long         , 4> i64arr;
	u64x4 u64vec; std::array<unsigned long long, 4> u64arr;
	f64x4 f64vec; std::array<double            , 4> f64arr;

	// An aggregate class has no user-declared or inherited constructors.
	// https://en.cppreference.com/w/cpp/language/aggregate_initialization
	// If ctors are added, then designated initializers can't be used.

	// assignment operators
	constexpr simd256& operator=(const __m256&  x) { f32vec = x; return *this; }
	constexpr simd256& operator=(const __m256i& x) { i64vec = x; return *this; }
	constexpr simd256& operator=(const __m256d& x) { f64vec = x; return *this; }

	// conversion operators
	explicit constexpr operator __m256  () const { return f32vec; }
	explicit constexpr operator __m256i () const { return i64vec; }
	explicit constexpr operator __m256d () const { return f64vec; }

	// named ctors
	static constexpr simd256 from_ymm(const __m256&  x) { return {.f32vec = x}; }
	static constexpr simd256 from_ymm(const __m256i& x) { return {.i64vec = x}; }
	static constexpr simd256 from_ymm(const __m256d& x) { return {.f64vec = x}; }

	constexpr bool operator==(const simd256& that) const
	{
		return std::memcmp(this, &that, sizeof(simd256)) == 0;
	}

	constexpr bool operator!=(const simd256& that) const
	{
		return !(*this == that);
	}
};

static_assert(sizeof(simd256) == 32);
static_assert(alignof(simd256) == 32);
