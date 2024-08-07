// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

#pragma once

#include "aes128-utils.h"
#include "union_128.h"

#include <array>
#include <concepts>
#include <cstdint>
#include <cstring>
#include <err.h>
#include <immintrin.h>
#include <limits>
#include <random>
#include <type_traits>
#include <unistd.h>

#if defined(__AES__)

// Must use -std=gnu++XX for these static_asserts to succeed.
// https://stackoverflow.com/a/71710062/1892784

static_assert(std::integral<__int128_t>);
static_assert(std::signed_integral<__int128_t>);

static_assert(std::integral<__uint128_t>);
static_assert(std::unsigned_integral<__uint128_t>);

template <bool enc, size_t Nk, size_t Nr>
requires (Nk * Nr >= 2)
class aes128_prng
{
private:
	__m128i x;
	__m128i c; // must be odd
	__m128i keys[Nk];

public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(x)>;

	aes128_prng()
	{
		reseed();
	}

	aes128_prng(const seed_bytes_type& bytes)
	{
		reseed();
		(void)std::memcpy(&x, bytes.data(), sizeof(x));
	}

	void reseed()
	{
		if (getentropy(this, sizeof(*this)) < 0)
		{
			err(EXIT_FAILURE, "getentropy");
		}

		// Every odd integer is coprime with every power of 2.  Ensure c is odd.
		this->c = mm_make_odd_epu64(this->c);
	}

	static constexpr result_type min()
	{
		return std::numeric_limits<result_type>::min();
	}

	static constexpr result_type max()
	{
		return std::numeric_limits<result_type>::max();
	}

	result_type operator()()
	{
		return next();
	}

	result_type next()
	{
		__m128i dst;
		if constexpr (enc)
		{
			dst = aes128_enc(this->x, this->keys, Nk, Nr);
		}
		else
		{
			dst = aes128_dec(this->x, this->keys, Nk, Nr);
		}
		this->x = _mm_add_epi64(this->x, this->c);
		return union_128{.xmm = dst}.u128;
	}
};

using aes128_prng_dec_k1_r2 = aes128_prng<false, 1, 2>;
using aes128_prng_dec_k2_r1 = aes128_prng<false, 2, 1>;
using aes128_prng_enc_k1_r2 = aes128_prng<true, 1, 2>;
using aes128_prng_enc_k2_r1 = aes128_prng<true, 2, 1>;

static_assert(std::uniform_random_bit_generator<aes128_prng_dec_k1_r2>);
static_assert(std::uniform_random_bit_generator<aes128_prng_dec_k2_r1>);
static_assert(std::uniform_random_bit_generator<aes128_prng_enc_k1_r2>);
static_assert(std::uniform_random_bit_generator<aes128_prng_enc_k2_r1>);

#else
#warning "__AES__ not defined"
#endif
