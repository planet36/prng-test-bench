// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// SIMD types
/**
* \file
* \author Steven Ward
*/

#pragma once

#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>

static_assert(std::endian::native == std::endian::little, "little-endian host required");

#if defined(__x86_64__) && defined(__SSE2__)

#include <immintrin.h>

using uint8x16_t = __m128i;

#if defined(__AVX__)

using uint8x16x2_t = __m256i;

#endif

#elif defined(__aarch64__) && defined(__ARM_NEON)

#include <arm_neon.h>

#else

#error "Architecture not supported"

#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"

template <size_t N>
using simd_arr_t = std::array<uint8x16_t, N>;

#if defined(__x86_64__) && defined(__AVX__)

template <size_t N>
using simd_arr_x2_t = std::array<uint8x16x2_t, N>;

#endif

#pragma GCC diagnostic pop

static_assert(sizeof(float) == 4);
static_assert(sizeof(double) == 8);

/// A 128-bit SIMD vector with a lane view for each integer and floating-point width
/**
* Reading from an inactive union member results in undefined behavior, which is prohibited in
* a constant expression.
*
* \sa https://developer.mozilla.org/en-US/docs/WebAssembly/Reference/Value_types/v128
*/
union alignas(16) simd_union_t
{
    std::array<std::byte    , 16 / sizeof(std::byte    )> bytes{};
    std::array<std::int8_t  , 16 / sizeof(std::int8_t  )>  i8;
    std::array<std::uint8_t , 16 / sizeof(std::uint8_t )>  u8;
    std::array<std::int16_t , 16 / sizeof(std::int16_t )> i16;
    std::array<std::uint16_t, 16 / sizeof(std::uint16_t)> u16;
    std::array<std::int32_t , 16 / sizeof(std::int32_t )> i32;
    std::array<std::uint32_t, 16 / sizeof(std::uint32_t)> u32;
    std::array<std::int64_t , 16 / sizeof(std::int64_t )> i64;
    std::array<std::uint64_t, 16 / sizeof(std::uint64_t)> u64;
    std::array<float        , 16 / sizeof(float        )> f32;
    std::array<double       , 16 / sizeof(double       )> f64;
#if defined(__SIZEOF_INT128__)
    __int128_t  i128;
    __uint128_t u128;
#endif
    uint8x16_t v;
};

static_assert(sizeof(simd_union_t) == 16);
static_assert(alignof(simd_union_t) == 16);
