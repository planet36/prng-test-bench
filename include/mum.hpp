// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// MUltiply and Mix ("MUM") functions
/**
\file
\author Steven Ward
\sa https://github.com/vnmakarov/mum-hash
\sa https://github.com/vnmakarov/mum-hash/blob/master/mum.h#L104
\sa https://github.com/vnmakarov/mum-hash/blob/master/mum-prng.h#L74
\sa https://github.com/gcc-mirror/gcc/blob/master/gcc/config/i386/bmi2intrin.h#L83
*/

#pragma once

#include "int_bytes.hpp"

#include <array>
#include <concepts>
#include <cstdint>
#include <limits>

namespace
{
// https://github.com/vnmakarov/mum-hash/blob/master/mum.h#L83

/* Here are different primes randomly generated with the equal
   probability of their bit values.  They are used to randomize input
   values.  */
inline constexpr uint64_t _mum_hash_step_prime   = 0x2e0bb864e9ea7df5; // prime (popcount = 36)
inline constexpr uint64_t _mum_key_step_prime    = 0xcdb32970830fcaa1; // prime (popcount = 30)
inline constexpr uint64_t _mum_block_start_prime = 0xc42b5e2e6480b23b; // prime (popcount = 29)
inline constexpr uint64_t _mum_unroll_prime      = 0x7b51ec3d22f7096f; // prime (popcount = 36)
inline constexpr uint64_t _mum_tail_prime        = 0xaf47d47c99b1461b; // prime (popcount = 34)
inline constexpr uint64_t _mum_finish_prime1     = 0xa9a7ae7ceff79f3f; // prime (popcount = 45)
inline constexpr uint64_t _mum_finish_prime2     = 0xaf47d47c99b1461b; // prime (popcount = 34)
static_assert((_mum_hash_step_prime   & 1) != 0, "must be odd");
static_assert((_mum_key_step_prime    & 1) != 0, "must be odd");
static_assert((_mum_block_start_prime & 1) != 0, "must be odd");
static_assert((_mum_unroll_prime      & 1) != 0, "must be odd");
static_assert((_mum_tail_prime        & 1) != 0, "must be odd");
static_assert((_mum_finish_prime1     & 1) != 0, "must be odd");
static_assert((_mum_finish_prime2     & 1) != 0, "must be odd");

inline constexpr std::array<uint64_t, 16> _mum_primes = {
	0x9ebdcae10d981691, // prime (popcount = 31)
	0x32b9b9b97a27ac7d, // prime (popcount = 37)
	0x29b5584d83d35bbd, // prime (popcount = 34)
	0x4b04e0e61401255f, // prime (popcount = 25)
	0x25e8f7b1f1c9d027, // prime (popcount = 34)
	0x80d4c8c000f3e881, // prime (popcount = 22)
	0xbd1255431904b9dd, // prime (popcount = 30)
	0x8a3bd4485eee6d81, // prime (popcount = 32)
	0x3bc721b2aad05197, // prime (popcount = 31)
	0x71b1a19b907d6e33, // prime (popcount = 33)
	0x525e6c1084a8534b, // prime (popcount = 26)
	0x9e4c2cd340c1299f, // prime (popcount = 29)
	0xde3add92e94caa37, // prime (popcount = 36)
	0x7e14eadb1f65311d, // prime (popcount = 35)
	0x3f5aa40f89812853, // prime (popcount = 28)
	0x33b15a3b587d15c9, // prime (popcount = 33)
};
static_assert((_mum_primes[ 0] & 1) != 0, "must be odd");
static_assert((_mum_primes[ 1] & 1) != 0, "must be odd");
static_assert((_mum_primes[ 2] & 1) != 0, "must be odd");
static_assert((_mum_primes[ 3] & 1) != 0, "must be odd");
static_assert((_mum_primes[ 4] & 1) != 0, "must be odd");
static_assert((_mum_primes[ 5] & 1) != 0, "must be odd");
static_assert((_mum_primes[ 6] & 1) != 0, "must be odd");
static_assert((_mum_primes[ 7] & 1) != 0, "must be odd");
static_assert((_mum_primes[ 8] & 1) != 0, "must be odd");
static_assert((_mum_primes[ 9] & 1) != 0, "must be odd");
static_assert((_mum_primes[10] & 1) != 0, "must be odd");
static_assert((_mum_primes[11] & 1) != 0, "must be odd");
static_assert((_mum_primes[12] & 1) != 0, "must be odd");
static_assert((_mum_primes[13] & 1) != 0, "must be odd");
static_assert((_mum_primes[14] & 1) != 0, "must be odd");
static_assert((_mum_primes[15] & 1) != 0, "must be odd");
}

/// Multiply \a hi and \a lo and return the high and low parts of the product
template <std::unsigned_integral T>
constexpr void
mul(T& hi, T& lo)
{
	// When T is uint64_t, this is optimized the same as _mulx_u64.
	using T2 = next_larger<T>;
	const T2 r = static_cast<T2>(hi) * static_cast<T2>(lo);
	hi = static_cast<T>(r >> std::numeric_limits<T>::digits);
	lo = static_cast<T>(r);
}

/// Multiply \a a and \a b and return the XOR of the high and low parts of the product
template <std::unsigned_integral T>
constexpr T
mumx(T a, T b)
{
	mul(a, b);
	return a ^ b;
}

/// Multiply \a a and \a b and return the sum of the high and low parts of the product
template <std::unsigned_integral T>
constexpr T
muma(T a, T b)
{
	mul(a, b);
	return a + b;
}

/// Multiply \a a and \a b and return the difference of the high and low parts of the product
template <std::unsigned_integral T>
constexpr T
mums(T a, T b)
{
	mul(a, b);
	return a - b;
}
