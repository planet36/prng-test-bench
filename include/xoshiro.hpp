// SPDX-FileCopyrightText: David Blackman
// SPDX-FileCopyrightText: Sebastiano Vigna (vigna@acm.org)
// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: CC0-1.0

/// xoshiro PRNG
/**
\file
\author Steven Ward
\sa https://prng.di.unimi.it/
\sa https://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
*/

#pragma once

#include "def_urbg_class_details.hpp"

#include <array>
#include <bit>
#include <cstdint>

/** This is xoshiro128++ 1.0, one of our 32-bit all-purpose, rock-solid
 * generators. It has excellent speed, a state size (128 bits) that is large
 * enough for mild parallelism, and it passes all tests we are aware of.
 *
 * For generating just single-precision (i.e., 32-bit) floating-point numbers,
 * xoshiro128+ is even faster.
 *
 * The state must be seeded so that it is not everywhere zero.
 */
struct xoshiro128plusplus
{
	using state_type = std::array<uint32_t, 4>;
	using result_type = uint32_t;

private:
DEF_URBG_CLASS_DETAILS(xoshiro128plusplus)

	// XXX: must not give zero seed

	constexpr result_type next()
	{
		const auto result = std::rotl(s[0] + s[3], 7) + s[0];
		const auto t = s[1] << 9;

		s[2] ^= s[0];
		s[3] ^= s[1];
		s[1] ^= s[2];
		s[0] ^= s[3];

		s[2] ^= t;

		s[3] = std::rotl(s[3], 11);

		return result;
	}
};

/** This is xoshiro128** 1.1, one of our 32-bit all-purpose, rock-solid
 * generators. It has excellent speed, a state size (128 bits) that is large
 * enough for mild parallelism, and it passes all tests we are aware of.
 *
 * Note that version 1.0 had mistakenly s[0] instead of s[1] as state word
 * passed to the scrambler.
 *
 * For generating just single-precision (i.e., 32-bit) floating-point numbers,
 * xoshiro128+ is even faster.
 *
 * The state must be seeded so that it is not everywhere zero.
 */
struct xoshiro128starstar
{
	using state_type = std::array<uint32_t, 4>;
	using result_type = uint32_t;

private:
DEF_URBG_CLASS_DETAILS(xoshiro128starstar)

	// XXX: must not give zero seed

	constexpr result_type next()
	{
		const auto result = std::rotl(s[1] * 5, 7) * 9;
		const auto t = s[1] << 9;

		s[2] ^= s[0];
		s[3] ^= s[1];
		s[1] ^= s[2];
		s[0] ^= s[3];

		s[2] ^= t;

		s[3] = std::rotl(s[3], 11);

		return result;
	}
};

/** This is xoshiro256++ 1.0, one of our all-purpose, rock-solid generators. It
 * has excellent (sub-ns) speed, a state (256 bits) that is large enough for
 * any parallel application, and it passes all tests we are aware of.
 *
 * For generating just floating-point numbers, xoshiro256+ is even faster.
 *
 * The state must be seeded so that it is not everywhere zero. If you have a
 * 64-bit seed, we suggest to seed a splitmix64 generator and use its output to
 * fill s.
 */
struct xoshiro256plusplus
{
	using state_type = std::array<uint64_t, 4>;
	using result_type = uint64_t;

private:
DEF_URBG_CLASS_DETAILS(xoshiro256plusplus)

	// XXX: must not give zero seed

	constexpr result_type next()
	{
		const auto result = std::rotl(s[0] + s[3], 23) + s[0];
		const auto t = s[1] << 17;

		s[2] ^= s[0];
		s[3] ^= s[1];
		s[1] ^= s[2];
		s[0] ^= s[3];

		s[2] ^= t;

		s[3] = std::rotl(s[3], 45);

		return result;
	}
};

/** This is xoshiro256** 1.0, one of our all-purpose, rock-solid generators. It
 * has excellent (sub-ns) speed, a state (256 bits) that is large enough for
 * any parallel application, and it passes all tests we are aware of.
 *
 * For generating just floating-point numbers, xoshiro256+ is even faster.
 *
 * The state must be seeded so that it is not everywhere zero. If you have a
 * 64-bit seed, we suggest to seed a splitmix64 generator and use its output to
 * fill s.
 */
struct xoshiro256starstar
{
	using state_type = std::array<uint64_t, 4>;
	using result_type = uint64_t;

private:
DEF_URBG_CLASS_DETAILS(xoshiro256starstar)

	// XXX: must not give zero seed

	constexpr result_type next()
	{
		const auto result = std::rotl(s[1] * 5, 7) * 9;
		const auto t = s[1] << 17;

		s[2] ^= s[0];
		s[3] ^= s[1];
		s[1] ^= s[2];
		s[0] ^= s[3];

		s[2] ^= t;

		s[3] = std::rotl(s[3], 45);

		return result;
	}
};

/** This is xoshiro512++ 1.0, one of our all-purpose, rock-solid generators. It
 * has excellent (about 1ns) speed, a state (512 bits) that is large enough for
 * any parallel application, and it passes all tests we are aware of.
 *
 * For generating just floating-point numbers, xoshiro512+ is even faster.
 *
 * The state must be seeded so that it is not everywhere zero. If you have a
 * 64-bit seed, we suggest to seed a splitmix64 generator and use its output to
 * fill s.
 */
struct xoshiro512plusplus
{
	using state_type = std::array<uint64_t, 8>;
	using result_type = uint64_t;

private:
DEF_URBG_CLASS_DETAILS(xoshiro512plusplus)

	// XXX: must not give zero seed

	constexpr result_type next()
	{
		const auto result = std::rotl(s[0] + s[2], 17) + s[2];
		const auto t = s[1] << 11;

		s[2] ^= s[0];
		s[5] ^= s[1];
		s[1] ^= s[2];
		s[7] ^= s[3];
		s[3] ^= s[4];
		s[4] ^= s[5];
		s[0] ^= s[6];
		s[6] ^= s[7];

		s[6] ^= t;

		s[7] = std::rotl(s[7], 21);

		return result;
	}
};

/** This is xoshiro512** 1.0, one of our all-purpose, rock-solid generators
 * with increased state size. It has excellent (about 1ns) speed, a state (512
 * bits) that is large enough for any parallel application, and it passes all
 * tests we are aware of.
 *
 * For generating just floating-point numbers, xoshiro512+ is even faster.
 *
 * The state must be seeded so that it is not everywhere zero. If you have a
 * 64-bit seed, we suggest to seed a splitmix64 generator and use its output to
 * fill s.
 */
struct xoshiro512starstar
{
	using state_type = std::array<uint64_t, 8>;
	using result_type = uint64_t;

private:
DEF_URBG_CLASS_DETAILS(xoshiro512starstar)

	// XXX: must not give zero seed

	constexpr result_type next()
	{
		const auto result = std::rotl(s[1] * 5, 7) * 9;
		const auto t = s[1] << 11;

		s[2] ^= s[0];
		s[5] ^= s[1];
		s[1] ^= s[2];
		s[7] ^= s[3];
		s[3] ^= s[4];
		s[4] ^= s[5];
		s[0] ^= s[6];
		s[6] ^= s[7];

		s[6] ^= t;

		s[7] = std::rotl(s[7], 21);

		return result;
	}
};
