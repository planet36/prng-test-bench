// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// wyhash & wyrand primes
/**
* \file
* \author Steven Ward
* \sa https://github.com/wangyi-fudan/wyhash/blob/master/wyhash.h#L144
*/

#pragma once

#if defined(__x86_64__)
#include <immintrin.h>
#endif
#include <array>
#include <bit>
#include <cstdint>

namespace wyprimes
{

inline constexpr std::array<std::uint64_t, 4> _wyp{
    0x2d358dccaa6c78a5, // prime (popcount = 32)
    0x8bb84b93962eacc9, // prime (popcount = 32)
    0x4b33a62ed433d4a3, // prime (popcount = 32)
    0x4d5a2da51de1aa47, // prime (popcount = 32)
};

static_assert((_wyp[0] & 1) != 0, "must be odd");
static_assert((_wyp[1] & 1) != 0, "must be odd");
static_assert((_wyp[2] & 1) != 0, "must be odd");
static_assert((_wyp[3] & 1) != 0, "must be odd");

static_assert(std::popcount(_wyp[0]) == 32, "popcount must be 32");
static_assert(std::popcount(_wyp[1]) == 32, "popcount must be 32");
static_assert(std::popcount(_wyp[2]) == 32, "popcount must be 32");
static_assert(std::popcount(_wyp[3]) == 32, "popcount must be 32");

#if defined(__x86_64__) && defined(__SSE2__)
[[nodiscard]] inline __m128i
vec128_01()
{
    // most significant elem first
    return _mm_set_epi64x(static_cast<std::int64_t>(_wyp[1]),
                          static_cast<std::int64_t>(_wyp[0]));
}

[[nodiscard]] inline __m128i
vec128_23()
{
    // most significant elem first
    return _mm_set_epi64x(static_cast<std::int64_t>(_wyp[3]),
                          static_cast<std::int64_t>(_wyp[2]));
}
#endif

#if defined(__x86_64__) && defined(__AVX__)
[[nodiscard]] inline __m256i
vec256()
{
    // most significant elem first
    return _mm256_set_epi64x(static_cast<std::int64_t>(_wyp[3]),
                             static_cast<std::int64_t>(_wyp[2]),
                             static_cast<std::int64_t>(_wyp[1]),
                             static_cast<std::int64_t>(_wyp[0]));
}
#endif

} // namespace wyprimes
