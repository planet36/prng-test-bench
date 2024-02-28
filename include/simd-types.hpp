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
	i8x16 i8;
	u8x16 u8;

	i16x8 i16;
	u16x8 u16;

	i32x4 i32;
	u32x4 u32;
	f32x4 f32;

	i64x2 i64;
	u64x2 u64;
	f64x2 f64;

	// An aggregate class has no user-declared or inherited constructors.
	// https://en.cppreference.com/w/cpp/language/aggregate_initialization
	// If ctors are added, then designated initializers can't be used.

	// assignment operators
	constexpr simd128& operator=(const __m128&  x) { f32 = x; return *this; }
	constexpr simd128& operator=(const __m128i& x) { i64 = x; return *this; }
	constexpr simd128& operator=(const __m128d& x) { f64 = x; return *this; }

	// conversion operators
	explicit constexpr operator __m128  () const { return f32; }
	explicit constexpr operator __m128i () const { return i64; }
	explicit constexpr operator __m128d () const { return f64; }

	// named ctors
	static constexpr simd128 from_xmm(const __m128&  x) { return {.f32 = x}; }
	static constexpr simd128 from_xmm(const __m128i& x) { return {.i64 = x}; }
	static constexpr simd128 from_xmm(const __m128d& x) { return {.f64 = x}; }

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
	i8x32 i8;
	u8x32 u8;

	i16x16 i16;
	u16x16 u16;

	i32x8 i32;
	u32x8 u32;
	f32x8 f32;

	i64x4 i64;
	u64x4 u64;
	f64x4 f64;

	// An aggregate class has no user-declared or inherited constructors.
	// https://en.cppreference.com/w/cpp/language/aggregate_initialization
	// If ctors are added, then designated initializers can't be used.

	// assignment operators
	constexpr simd256& operator=(const __m256&  x) { f32 = x; return *this; }
	constexpr simd256& operator=(const __m256i& x) { i64 = x; return *this; }
	constexpr simd256& operator=(const __m256d& x) { f64 = x; return *this; }

	// conversion operators
	explicit constexpr operator __m256  () const { return f32; }
	explicit constexpr operator __m256i () const { return i64; }
	explicit constexpr operator __m256d () const { return f64; }

	// named ctors
	static constexpr simd256 from_ymm(const __m256&  x) { return {.f32 = x}; }
	static constexpr simd256 from_ymm(const __m256i& x) { return {.i64 = x}; }
	static constexpr simd256 from_ymm(const __m256d& x) { return {.f64 = x}; }

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
