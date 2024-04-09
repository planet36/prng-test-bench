// SPDX-FileCopyrightText: David Blackman
// SPDX-FileCopyrightText: Sebastiano Vigna (vigna@acm.org)
// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: CC0-1.0

/// xoroshiro PRNG
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

#define DEF_JUMP                                                     \
	void jump()                                                      \
	{                                                                \
		using T = state_type::value_type;                            \
		state_type new_s;                                            \
		new_s.fill(0);                                               \
		for (const auto x : JUMP)                                    \
		{                                                            \
			for (int b = 0; b < std::numeric_limits<T>::digits; b++) \
			{                                                        \
				if (x & (T{1} << b))                                 \
					for (size_t i = 0; i < s.size(); i++)            \
					{                                                \
						new_s[i] ^= s[i];                            \
					}                                                \
				(void)next();                                        \
			}                                                        \
		}                                                            \
		s = new_s;                                                   \
	}

#define DEF_LONG_JUMP                                                \
	void long_jump()                                                 \
	{                                                                \
		using T = state_type::value_type;                            \
		state_type new_s;                                            \
		new_s.fill(0);                                               \
		for (const auto x : LONG_JUMP)                               \
		{                                                            \
			for (int b = 0; b < std::numeric_limits<T>::digits; b++) \
			{                                                        \
				if (x & (T{1} << b))                                 \
					for (size_t i = 0; i < s.size(); i++)            \
					{                                                \
						new_s[i] ^= s[i];                            \
					}                                                \
				(void)next();                                        \
			}                                                        \
		}                                                            \
		s = new_s;                                                   \
	}

#define DEF_JUMP_2                                                   \
	void jump()                                                      \
	{                                                                \
		using T = state_type::value_type;                            \
		state_type new_s;                                            \
		new_s.fill(0);                                               \
		for (const auto x : JUMP)                                    \
		{                                                            \
			for (int b = 0; b < std::numeric_limits<T>::digits; b++) \
			{                                                        \
				if (x & (T{1} << b))                                 \
					for (size_t i = 0; i < s.size(); i++)            \
					{                                                \
						new_s[i] ^= s[(i + p) % s.size()];           \
					}                                                \
				(void)next();                                        \
			}                                                        \
		}                                                            \
		for (size_t i = 0; i < s.size(); i++)                        \
		{                                                            \
			s[(i + p) % s.size()] = new_s[i];                        \
		}                                                            \
	}

#define DEF_LONG_JUMP_2                                              \
	void long_jump()                                                 \
	{                                                                \
		using T = state_type::value_type;                            \
		state_type new_s;                                            \
		new_s.fill(0);                                               \
		for (const auto x : LONG_JUMP)                               \
		{                                                            \
			for (int b = 0; b < std::numeric_limits<T>::digits; b++) \
			{                                                        \
				if (x & (T{1} << b))                                 \
					for (size_t i = 0; i < s.size(); i++)            \
					{                                                \
						new_s[i] ^= s[(i + p) % s.size()];           \
					}                                                \
				(void)next();                                        \
			}                                                        \
		}                                                            \
		for (size_t i = 0; i < s.size(); i++)                        \
		{                                                            \
			s[(i + p) % s.size()] = new_s[i];                        \
		}                                                            \
	}

/** This is xoroshiro64** 1.0, our 32-bit all-purpose, rock-solid, small-state
 * generator. It is extremely fast and it passes all tests we are aware of, but
 * its state space is not large enough for any parallel application.
 *
 * For generating just single-precision (i.e., 32-bit) floating-point numbers,
 * xoroshiro64* is even faster.
 *
 * The state must be seeded so that it is not everywhere zero.
 */
struct xoroshiro64starstar
{
	using state_type = std::array<uint32_t, 2>;
	using result_type = uint32_t;

DEF_URBG_CLASS_DETAILS(xoroshiro64starstar)

	// XXX: must not give zero seed

	constexpr result_type next()
	{
		const auto s0 = s[0];
		auto s1 = s[1];
		const auto result = std::rotl(s0 * 0x9E3779BB, 5) * 5;

		s1 ^= s0;
		s[0] = std::rotl(s0, 26) ^ s1 ^ (s1 << 9); // a, b
		s[1] = std::rotl(s1, 13); // c

		return result;
	}
};

/** This is xoroshiro128++ 1.0, one of our all-purpose, rock-solid, small-state
 * generators. It is extremely (sub-ns) fast and it passes all tests we are
 * aware of, but its state space is large enough only for mild parallelism.
 *
 * For generating just floating-point numbers, xoroshiro128+ is even faster
 * (but it has a very mild bias, see notes in the comments).
 *
 * The state must be seeded so that it is not everywhere zero. If you have a
 * 64-bit seed, we suggest to seed a splitmix64 generator and use its output to
 * fill s.
 */
struct xoroshiro128plusplus
{
	using state_type = std::array<uint64_t, 2>;
	using result_type = uint64_t;

private:
	static constexpr state_type JUMP{
	    0x2bd7a6a6e99c2ddc, 0x0992ccaf6a6fca05};
	static constexpr state_type LONG_JUMP{
	    0x360fd5f2cf8d5d99, 0x9c6e6877736c46e3};

DEF_URBG_CLASS_DETAILS(xoroshiro128plusplus)

	// XXX: must not give zero seed

	constexpr result_type next()
	{
		const auto s0 = s[0];
		auto s1 = s[1];
		const auto result = std::rotl(s0 + s1, 17) + s0;

		s1 ^= s0;
		s[0] = std::rotl(s0, 49) ^ s1 ^ (s1 << 21); // a, b
		s[1] = std::rotl(s1, 28); // c

		return result;
	}

	/** This is the jump function for the generator. It is equivalent to 2^64
	 * calls to next(); it can be used to generate 2^64 non-overlapping
	 * subsequences for parallel computations.
	 */
	DEF_JUMP

	/** This is the long-jump function for the generator. It is equivalent to
	 * 2^96 calls to next(); it can be used to generate 2^32 starting points,
	 * from each of which jump() will generate 2^32 non-overlapping
	 * subsequences for parallel distributed computations.
	 */
	DEF_LONG_JUMP
};

/** This is xoroshiro128** 1.0, one of our all-purpose, rock-solid, small-state
 * generators. It is extremely (sub-ns) fast and it passes all tests we are
 * aware of, but its state space is large enough only for mild parallelism.
 *
 * For generating just floating-point numbers, xoroshiro128+ is even faster
 * (but it has a very mild bias, see notes in the comments).
 *
 * The state must be seeded so that it is not everywhere zero. If you have a
 * 64-bit seed, we suggest to seed a splitmix64 generator and use its output to
 * fill s.
 */
struct xoroshiro128starstar
{
	using state_type = std::array<uint64_t, 2>;
	using result_type = uint64_t;

private:
	static constexpr state_type JUMP{
	    0xdf900294d8f554a5, 0x170865df4b3201fc};
	static constexpr state_type LONG_JUMP{
	    0xd2a98b26625eee7b, 0xdddf9b1090aa7ac1};

DEF_URBG_CLASS_DETAILS(xoroshiro128starstar)

	// XXX: must not give zero seed

	constexpr result_type next()
	{
		const auto s0 = s[0];
		auto s1 = s[1];
		const auto result = std::rotl(s0 * 5, 7) * 9;

		s1 ^= s0;
		s[0] = std::rotl(s0, 24) ^ s1 ^ (s1 << 16); // a, b
		s[1] = std::rotl(s1, 37); // c

		return result;
	}

	/** This is the jump function for the generator. It is equivalent to 2^64
	 * calls to next(); it can be used to generate 2^64 non-overlapping
	 * subsequences for parallel computations.
	 */
	DEF_JUMP

	/** This is the long-jump function for the generator. It is equivalent to
	 * 2^96 calls to next(); it can be used to generate 2^32 starting points,
	 * from each of which jump() will generate 2^32 non-overlapping
	 * subsequences for parallel distributed computations.
	 */
	DEF_LONG_JUMP
};

/** This is xoroshiro1024++ 1.0, one of our all-purpose, rock-solid,
 * large-state generators. It is extremely fast and it passes all tests we are
 * aware of. Its state however is too large--in general, the xoshiro256 family
 * should be preferred.
 *
 * For generating just floating-point numbers, xoroshiro1024* is even faster
 * (but it has a very mild bias, see notes in the comments).
 *
 * The state must be seeded so that it is not everywhere zero. If you have a
 * 64-bit seed, we suggest to seed a splitmix64 generator and use its output to
 * fill s.
 */
struct xoroshiro1024plusplus
{
	using state_type = std::array<uint64_t, 16>;
	using result_type = uint64_t;

private:
	unsigned int p{};
	static constexpr state_type JUMP{
	    0x931197d8e3177f17, 0xb59422e0b9138c5f, 0xf06a6afb49d668bb,
	    0xacb8a6412c8a1401, 0x12304ec85f0b3468, 0xb7dfe7079209891e,
	    0x405b7eec77d9eb14, 0x34ead68280c44e4a, 0xe0e4ba3e0ac9e366,
	    0x8f46eda8348905b7, 0x328bf4dbad90d6ff, 0xc8fd6fb31c9effc3,
	    0xe899d452d4b67652, 0x45f387286ade3205, 0x03864f454a8920bd,
	    0xa68fa28725b1b384};
	static constexpr state_type LONG_JUMP{
	    0x7374156360bbf00f, 0x4630c2efa3b3c1f6, 0x6654183a892786b1,
	    0x94f7bfcbfb0f1661, 0x27d8243d3d13eb2d, 0x9701730f3dfb300f,
	    0x2f293baae6f604ad, 0xa661831cb60cd8b6, 0x68280c77d9fe008c,
	    0x50554160f5ba9459, 0x2fc20b17ec7b2a9a, 0x49189bbdc8ec9f8f,
	    0x92a65bca41852cc1, 0xf46820dd0509c12a, 0x52b00c35fbf92185,
	    0x1e5b3b7f589e03c1};

DEF_URBG_CLASS_DETAILS(xoroshiro1024plusplus)

	// XXX: must not give zero seed

	constexpr result_type next()
	{
		const auto q = p;
		p = (p + 1) % s.size();
		const auto s0 = s[p];
		auto s15 = s[q];
		const auto result = std::rotl(s0 + s15, 23) + s15;

		s15 ^= s0;
		s[q] = std::rotl(s0, 25) ^ s15 ^ (s15 << 27);
		s[p] = std::rotl(s15, 36);

		return result;
	}

	/** This is the jump function for the generator. It is equivalent to 2^512
	 * calls to next(); it can be used to generate 2^512 non-overlapping
	 * subsequences for parallel computations.
	 */
	DEF_JUMP_2

	/** This is the long-jump function for the generator. It is equivalent to
	 * 2^768 calls to next(); it can be used to generate 2^256 starting points,
	 * from each of which jump() will generate 2^256 non-overlapping
	 * subsequences for parallel distributed computations.
	 */
	DEF_LONG_JUMP_2
};

/** This is xoroshiro1024** 1.0, one of our all-purpose, rock-solid,
 * large-state generators. It is extremely fast and it passes all tests we are
 * aware of. Its state however is too large--in general, the xoshiro256 family
 * should be preferred.
 *
 * For generating just floating-point numbers, xoroshiro1024* is even faster
 * (but it has a very mild bias, see notes in the comments).
 *
 * The state must be seeded so that it is not everywhere zero. If you have a
 * 64-bit seed, we suggest to seed a splitmix64 generator and use its output to
 * fill s.
 */
struct xoroshiro1024starstar
{
	using state_type = std::array<uint64_t, 16>;
	using result_type = uint64_t;

private:
	unsigned int p{};
	static constexpr state_type JUMP{
	    0x931197d8e3177f17, 0xb59422e0b9138c5f, 0xf06a6afb49d668bb,
	    0xacb8a6412c8a1401, 0x12304ec85f0b3468, 0xb7dfe7079209891e,
	    0x405b7eec77d9eb14, 0x34ead68280c44e4a, 0xe0e4ba3e0ac9e366,
	    0x8f46eda8348905b7, 0x328bf4dbad90d6ff, 0xc8fd6fb31c9effc3,
	    0xe899d452d4b67652, 0x45f387286ade3205, 0x03864f454a8920bd,
	    0xa68fa28725b1b384};
	static constexpr state_type LONG_JUMP{
	    0x7374156360bbf00f, 0x4630c2efa3b3c1f6, 0x6654183a892786b1,
	    0x94f7bfcbfb0f1661, 0x27d8243d3d13eb2d, 0x9701730f3dfb300f,
	    0x2f293baae6f604ad, 0xa661831cb60cd8b6, 0x68280c77d9fe008c,
	    0x50554160f5ba9459, 0x2fc20b17ec7b2a9a, 0x49189bbdc8ec9f8f,
	    0x92a65bca41852cc1, 0xf46820dd0509c12a, 0x52b00c35fbf92185,
	    0x1e5b3b7f589e03c1};

DEF_URBG_CLASS_DETAILS(xoroshiro1024starstar)

	// XXX: must not give zero seed

	constexpr result_type next()
	{
		const auto q = p;
		p = (p + 1) % s.size();
		const auto s0 = s[p];
		auto s15 = s[q];
		const auto result = std::rotl(s0 * 5, 7) * 9;

		s15 ^= s0;
		s[q] = std::rotl(s0, 25) ^ s15 ^ (s15 << 27);
		s[p] = std::rotl(s15, 36);

		return result;
	}

	/** This is the jump function for the generator. It is equivalent to 2^512
	 * calls to next(); it can be used to generate 2^512 non-overlapping
	 * subsequences for parallel computations.
	 */
	DEF_JUMP_2

	/** This is the long-jump function for the generator. It is equivalent to
	 * 2^768 calls to next(); it can be used to generate 2^256 starting points,
	 * from each of which jump() will generate 2^256 non-overlapping
	 * subsequences for parallel distributed computations.
	 */
	DEF_LONG_JUMP_2
};

#undef DEF_JUMP
#undef DEF_LONG_JUMP
#undef DEF_JUMP_2
#undef DEF_LONG_JUMP_2
