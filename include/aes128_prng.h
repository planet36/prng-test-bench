// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// AES-128 PRNG
/**
\file
\author Steven Ward
*/

#pragma once

#include <err.h>
#include <immintrin.h>
#include <stdint.h>
#include <unistd.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

/// Do \c _mm_aesenc_si128 \a N times on data \a a with key \a key
/**
\pre \a N must be at least \c 1.
\param a the data
\param key the key
\param N the number of rounds of encryption to perform
*/
static __m128i
aes128_enc_mix(__m128i a, const __m128i key, const unsigned int N)
{
	for (unsigned int round = 0; round < N; ++round)
	{
		a = _mm_aesenc_si128(a, key);
	}
	return a;
}

/// Do \c _mm_aesdec_si128 \a N times on data \a a with key \a key
/**
\pre \a N must be at least \c 1.
\param a the data
\param key the key
\param N the number of rounds of decryption to perform
*/
static __m128i
aes128_dec_mix(__m128i a, const __m128i key, const unsigned int N)
{
	for (unsigned int round = 0; round < N; ++round)
	{
		a = _mm_aesdec_si128(a, key);
	}
	return a;
}

/// Davies-Meyer single-block-length compression function that uses AES as the block cipher
/**
\sa https://en.wikipedia.org/wiki/One-way_compression_function#Davies%E2%80%93Meyer
\pre \a N must be at least \c 1.
\param H the previous hash value
\param m the block of the message
\param N the number of rounds of encryption to perform
\return the next hash value
*/
static inline __m128i
aes128_enc_davies_meyer(const __m128i H, const __m128i m, const unsigned int N)
{
	return _mm_xor_si128(H, aes128_enc_mix(H, m, N));
}

/// Davies-Meyer single-block-length compression function that uses AES as the block cipher
/**
\sa https://en.wikipedia.org/wiki/One-way_compression_function#Davies%E2%80%93Meyer
\pre \a N must be at least \c 1.
\param H the previous hash value
\param m the block of the message
\param N the number of rounds of decryption to perform
\return the next hash value
*/
static inline __m128i
aes128_dec_davies_meyer(const __m128i H, const __m128i m, const unsigned int N)
{
	return _mm_xor_si128(H, aes128_dec_mix(H, m, N));
}

/// A PRNG that uses AES instructions
/**
* This does \em not satisfy the following:
*  - named requirement \c UniformRandomBitGenerator
*  - concept \c std::uniform_random_bit_generator
* \sa https://en.cppreference.com/w/cpp/named_req/UniformRandomBitGenerator
* \sa https://en.cppreference.com/w/cpp/numeric/random/uniform_random_bit_generator
*/
struct aes128_prng
{
	__m128i x;
	__m128i c; // must be odd
	__m128i key;
};

typedef struct aes128_prng aes128_prng;

/// Get the minimum value that could be generated.
static inline __m128i
aes128_prng_min() { return _mm_setzero_si128(); }

/// Get the maximum value that could be generated.
static inline __m128i
aes128_prng_max() { return _mm_set1_epi64x(UINT64_MAX); }

// getentropy will fail if more than 256 bytes are requested.
static_assert(sizeof(aes128_prng) <= 256);

/// Assign random bytes to the data members via \c getentropy.
static void
aes128_prng_reseed(aes128_prng* _this)
{
	if (getentropy(_this, sizeof(*_this)) < 0)
	{ err(EXIT_FAILURE, "getentropy"); }

	// Every odd integer is coprime with every power of 2.  Ensure c is odd.
	_this->c =_mm_or_si128(_this->c, _mm_set_epi64x(1, 1));
}

/// Get the next PRNG output.
static __m128i
aes128_prng_next(aes128_prng* _this)
{
	constexpr unsigned int N = 2;
	static_assert(N >= 2);

	const __m128i dst = aes128_enc_mix(_this->x, _this->key, N);
	_this->x = _mm_add_epi64(_this->x, _this->c);

	return dst;
}

static __m128i
aes128_prng_next_enc_mix_n1(aes128_prng* _this)
{
	const __m128i dst = aes128_enc_mix(_this->x, _this->key, 1);
	_this->x = _mm_add_epi64(_this->x, _this->c);
	return dst;
}

static __m128i
aes128_prng_next_enc_mix_n2(aes128_prng* _this)
{
	const __m128i dst = aes128_enc_mix(_this->x, _this->key, 2);
	_this->x = _mm_add_epi64(_this->x, _this->c);
	return dst;
}

static __m128i
aes128_prng_next_enc_mix_n3(aes128_prng* _this)
{
	const __m128i dst = aes128_enc_mix(_this->x, _this->key, 3);
	_this->x = _mm_add_epi64(_this->x, _this->c);
	return dst;
}

static __m128i
aes128_prng_next_enc_mix_n4(aes128_prng* _this)
{
	const __m128i dst = aes128_enc_mix(_this->x, _this->key, 4);
	_this->x = _mm_add_epi64(_this->x, _this->c);
	return dst;
}

static __m128i
aes128_prng_next_enc_dm_n1(aes128_prng* _this)
{
	const __m128i dst = aes128_enc_davies_meyer(_this->x, _this->key, 1);
	_this->x = _mm_add_epi64(_this->x, _this->c);
	return dst;
}

static __m128i
aes128_prng_next_enc_dm_n2(aes128_prng* _this)
{
	const __m128i dst = aes128_enc_davies_meyer(_this->x, _this->key, 2);
	_this->x = _mm_add_epi64(_this->x, _this->c);
	return dst;
}

static __m128i
aes128_prng_next_enc_dm_n3(aes128_prng* _this)
{
	const __m128i dst = aes128_enc_davies_meyer(_this->x, _this->key, 3);
	_this->x = _mm_add_epi64(_this->x, _this->c);
	return dst;
}

static __m128i
aes128_prng_next_enc_dm_n4(aes128_prng* _this)
{
	const __m128i dst = aes128_enc_davies_meyer(_this->x, _this->key, 4);
	_this->x = _mm_add_epi64(_this->x, _this->c);
	return dst;
}

static __m128i
aes128_prng_next_dec_mix_n1(aes128_prng* _this)
{
	const __m128i dst = aes128_dec_mix(_this->x, _this->key, 1);
	_this->x = _mm_add_epi64(_this->x, _this->c);
	return dst;
}

static __m128i
aes128_prng_next_dec_mix_n2(aes128_prng* _this)
{
	const __m128i dst = aes128_dec_mix(_this->x, _this->key, 2);
	_this->x = _mm_add_epi64(_this->x, _this->c);
	return dst;
}

static __m128i
aes128_prng_next_dec_mix_n3(aes128_prng* _this)
{
	const __m128i dst = aes128_dec_mix(_this->x, _this->key, 3);
	_this->x = _mm_add_epi64(_this->x, _this->c);
	return dst;
}

static __m128i
aes128_prng_next_dec_mix_n4(aes128_prng* _this)
{
	const __m128i dst = aes128_dec_mix(_this->x, _this->key, 4);
	_this->x = _mm_add_epi64(_this->x, _this->c);
	return dst;
}

static __m128i
aes128_prng_next_dec_dm_n1(aes128_prng* _this)
{
	const __m128i dst = aes128_dec_davies_meyer(_this->x, _this->key, 1);
	_this->x = _mm_add_epi64(_this->x, _this->c);
	return dst;
}

static __m128i
aes128_prng_next_dec_dm_n2(aes128_prng* _this)
{
	const __m128i dst = aes128_dec_davies_meyer(_this->x, _this->key, 2);
	_this->x = _mm_add_epi64(_this->x, _this->c);
	return dst;
}

static __m128i
aes128_prng_next_dec_dm_n3(aes128_prng* _this)
{
	const __m128i dst = aes128_dec_davies_meyer(_this->x, _this->key, 3);
	_this->x = _mm_add_epi64(_this->x, _this->c);
	return dst;
}

static __m128i
aes128_prng_next_dec_dm_n4(aes128_prng* _this)
{
	const __m128i dst = aes128_dec_davies_meyer(_this->x, _this->key, 4);
	_this->x = _mm_add_epi64(_this->x, _this->c);
	return dst;
}

#pragma GCC diagnostic pop
