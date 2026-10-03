// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// the number of uniformly random bits in one output of a URBG
/**
* \file
* \author Steven Ward
*
* A URBG's \c result_type can be wider than the range of its outputs.  For example,
* \c std::mt19937 returns \c std::uint_fast32_t, which is 64 bits wide in libstdc++ on
* x86-64, but every output fits in 32 bits.  The number of bits that are worth testing
* depends on <code>G::min()</code> and <code>G::max()</code>, not on the width of the type.
*/

#pragma once

#include <bit>
#include <concepts>
#include <cstdint>
#include <limits>
#include <random>

/// the largest value of <code>g() - G::min()</code> for a URBG \c G
template <std::uniform_random_bit_generator G>
inline constexpr G::result_type urbg_range =
    static_cast<G::result_type>(G::max() - G::min());

/// the number of uniformly random bits in a value drawn uniformly from 0 to \a range
/**
* This is floor(log2(\a range + 1)).  When the number of values is not a power of 2, the
* top bit is not uniform, and a value that sets it has to be rejected.  For example,
* \c std::minstd_rand has 2^31 - 2 values, so it gives 30 uniform bits, not 31.
*
* \a range + 1 is never computed, because it wraps to 0 when \a range is the maximum of
* its type.
*/
template <std::unsigned_integral T>
[[nodiscard]] constexpr int
uniform_bits(const T range) noexcept
{
    const int width = std::bit_width(range);

    // The number of values is a power of 2 exactly when every bit below the width is set.
    return (std::popcount(range) == width) ? width : width - 1;
}

/// the number of uniformly random bits in one output of the URBG \c G
template <std::uniform_random_bit_generator G>
inline constexpr int urbg_uniform_bits = uniform_bits(urbg_range<G>);

static_assert(uniform_bits(std::uint8_t{1}) == 1);
static_assert(uniform_bits(std::uint8_t{2}) == 1);
static_assert(uniform_bits(std::uint8_t{3}) == 2);
static_assert(uniform_bits(std::numeric_limits<std::uint8_t>::max()) == 8);
static_assert(uniform_bits(std::numeric_limits<std::uint64_t>::max()) == 64);
static_assert(uniform_bits(std::numeric_limits<std::uint64_t>::max() - 1) == 63);
#if defined(__SIZEOF_INT128__)
static_assert(uniform_bits(std::numeric_limits<__uint128_t>::max()) == 128);
static_assert(uniform_bits(std::numeric_limits<__uint128_t>::max() - 1) == 127);
#endif

// std::default_random_engine is std::minstd_rand0 in libstdc++.
static_assert(urbg_uniform_bits<std::default_random_engine> == 30);
static_assert(urbg_uniform_bits<std::knuth_b              > == 30);
static_assert(urbg_uniform_bits<std::minstd_rand          > == 30);
static_assert(urbg_uniform_bits<std::minstd_rand0         > == 30);
static_assert(urbg_uniform_bits<std::mt19937              > == 32);
static_assert(urbg_uniform_bits<std::mt19937_64           > == 64);
static_assert(urbg_uniform_bits<std::philox4x32           > == 32);
static_assert(urbg_uniform_bits<std::philox4x64           > == 64);
static_assert(urbg_uniform_bits<std::ranlux24             > == 24);
static_assert(urbg_uniform_bits<std::ranlux24_base        > == 24);
static_assert(urbg_uniform_bits<std::ranlux48             > == 48);
static_assert(urbg_uniform_bits<std::ranlux48_base        > == 48);
