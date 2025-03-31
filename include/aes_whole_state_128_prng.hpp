// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

#pragma once

#include "aes128-utils.h"
#include "simd-union.hpp"

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

/// A PRNG that uses AES instructions
template <bool enc, size_t Nk, size_t Nr>
class aes_whole_state_128_prng
{
	static_assert(Nk >= 1);
	static_assert(Nr >= 1);
	static_assert(Nk * Nr >= 2, "must do at least 2 rounds of AES enc/dec");

private:
	__m128i x{}; ///< The state
	__m128i keys[Nk]{};

public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(x)>;

	aes_whole_state_128_prng()
	{
		static_assert(sizeof(*this) <= 256,
		    "getentropy will fail if more than 256 bytes are requested");
		reseed();
	}

	explicit aes_whole_state_128_prng(const seed_bytes_type& bytes)
	{
		reseed();
		(void)std::memcpy(&x, bytes.data(), sizeof(x));
	}

	/// Assign random bytes to the data members via \c getentropy.
	void reseed()
	{
		if (getentropy(this, sizeof(*this)) < 0)
			err(EXIT_FAILURE, "getentropy");
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

	/// Get the next PRNG output via AES encryption or decryption.
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
		return simd128i{.xmm = dst}.u128[0];
	}
};

using aes_whole_state_128_prng_dec_k1_r2 = aes_whole_state_128_prng<false, 1, 2>;
using aes_whole_state_128_prng_dec_k1_r3 = aes_whole_state_128_prng<false, 1, 3>;
using aes_whole_state_128_prng_dec_k2_r1 = aes_whole_state_128_prng<false, 2, 1>;
using aes_whole_state_128_prng_dec_k3_r1 = aes_whole_state_128_prng<false, 3, 1>;
using aes_whole_state_128_prng_enc_k1_r2 = aes_whole_state_128_prng< true, 1, 2>;
using aes_whole_state_128_prng_enc_k1_r3 = aes_whole_state_128_prng< true, 1, 3>;
using aes_whole_state_128_prng_enc_k2_r1 = aes_whole_state_128_prng< true, 2, 1>;
using aes_whole_state_128_prng_enc_k3_r1 = aes_whole_state_128_prng< true, 3, 1>;

static_assert(std::uniform_random_bit_generator<aes_whole_state_128_prng_dec_k1_r2>);
static_assert(std::uniform_random_bit_generator<aes_whole_state_128_prng_dec_k1_r3>);
static_assert(std::uniform_random_bit_generator<aes_whole_state_128_prng_dec_k2_r1>);
static_assert(std::uniform_random_bit_generator<aes_whole_state_128_prng_dec_k3_r1>);
static_assert(std::uniform_random_bit_generator<aes_whole_state_128_prng_enc_k1_r2>);
static_assert(std::uniform_random_bit_generator<aes_whole_state_128_prng_enc_k1_r3>);
static_assert(std::uniform_random_bit_generator<aes_whole_state_128_prng_enc_k2_r1>);
static_assert(std::uniform_random_bit_generator<aes_whole_state_128_prng_enc_k3_r1>);

#else
#warning "__AES__ not defined"
#endif
