// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

#pragma once

#include "simd-array.hpp"
#include "simd-transpose.hpp"
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

/// A PRNG that uses AES instructions
template <size_t Nk, size_t Nr>
class aes_half_state_64_prng
{
	static constexpr size_t Ns = 1; ///< The number of elements in the state

	static_assert(Nk >= 1);
	static_assert(Nr >= 1);
	static_assert(Nk * Nr >= 2, "must do at least 2 rounds of AES");

private:
	arr_m128i<Ns> x{}; ///< The state
	arr_m128i<Nk> keys{};

public:
	using result_type = uint64_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(x)>;

	aes_half_state_64_prng()
	{
		static_assert(sizeof(*this) <= 256,
		    "getentropy will fail if more than 256 bytes are requested");
		reseed();
	}

	explicit aes_half_state_64_prng(const seed_bytes_type& bytes)
	{
		reseed();
		(void)std::memcpy(x.data(), bytes.data(), sizeof(x));
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
		for (size_t r = 0; r < Nr; ++r)
		{
			for (size_t k = 0; k < Nk; ++k)
			{
				if constexpr (Ns > 1)
					transpose(x);

				for (size_t i = 0; i < Ns; ++i)
				{
					x[i] = _mm_aesenc_si128(x[i], keys[k]);
				}
			}
		}

		return _mm_cvtsi128_si64x(x[0]);
	}
};

using aes_half_state_64_k1_r2 = aes_half_state_64_prng<1, 2>;
using aes_half_state_64_k1_r3 = aes_half_state_64_prng<1, 3>;
using aes_half_state_64_k2_r1 = aes_half_state_64_prng<2, 1>;
using aes_half_state_64_k3_r1 = aes_half_state_64_prng<3, 1>;

static_assert(std::uniform_random_bit_generator<aes_half_state_64_k1_r2>);
static_assert(std::uniform_random_bit_generator<aes_half_state_64_k1_r3>);
static_assert(std::uniform_random_bit_generator<aes_half_state_64_k2_r1>);
static_assert(std::uniform_random_bit_generator<aes_half_state_64_k3_r1>);

#else
#warning "__AES__ not defined"
#endif
