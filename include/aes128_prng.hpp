// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

#pragma once

#include "aes128_prng.h"
#include "union_128.h"

#include <array>
#include <concepts>
#include <cstdint>
#include <cstring>
#include <limits>
#include <random>
#include <type_traits>
#include <immintrin.h>

#if defined(__AES__)

// Must use -std=gnu++XX for these static_asserts to succeed.
// https://stackoverflow.com/a/71710062/1892784

static_assert(std::integral<__int128_t>);
static_assert(std::signed_integral<__int128_t>);

static_assert(std::integral<__uint128_t>);
static_assert(std::unsigned_integral<__uint128_t>);

#define DEF_AES_PRNG_CLASS(METHOD)                                                         \
class aes128_prng_ ## METHOD                                                               \
{                                                                                          \
private:                                                                                   \
	aes128_prng state;                                                                     \
public:                                                                                    \
	using result_type = __uint128_t;                                                       \
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;                          \
	aes128_prng_ ## METHOD() { aes128_prng_reseed(&state); }                               \
	aes128_prng_ ## METHOD(const seed_bytes_type& bytes) {                                 \
		aes128_prng_reseed(&state);                                                        \
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }                      \
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); } \
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); } \
	constexpr result_type operator()() { return next(); }                                  \
	result_type next() {                                                                   \
		const __m128i result = aes128_prng_next_ ## METHOD(&state);                        \
		return union_128{.xmm = result}.u128;                                              \
	}                                                                                      \
};                                                                                         \
static_assert(std::uniform_random_bit_generator<aes128_prng_ ## METHOD>);

class aes128_prng_enc_dm_n1
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_enc_dm_n1() { aes128_prng_reseed(&state); }
	aes128_prng_enc_dm_n1(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_enc_dm_n1(&state);
		return union_128{.xmm = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_enc_dm_n1>);

class aes128_prng_enc_dm_n2
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_enc_dm_n2() { aes128_prng_reseed(&state); }
	aes128_prng_enc_dm_n2(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_enc_dm_n2(&state);
		return union_128{.xmm = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_enc_dm_n2>);

class aes128_prng_enc_dm_n3
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_enc_dm_n3() { aes128_prng_reseed(&state); }
	aes128_prng_enc_dm_n3(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_enc_dm_n3(&state);
		return union_128{.xmm = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_enc_dm_n3>);

class aes128_prng_enc_dm_n4
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_enc_dm_n4() { aes128_prng_reseed(&state); }
	aes128_prng_enc_dm_n4(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_enc_dm_n4(&state);
		return union_128{.xmm = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_enc_dm_n4>);

class aes128_prng_enc_mix_n1
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_enc_mix_n1() { aes128_prng_reseed(&state); }
	aes128_prng_enc_mix_n1(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_enc_mix_n1(&state);
		return union_128{.xmm = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_enc_mix_n1>);

class aes128_prng_enc_mix_n2
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_enc_mix_n2() { aes128_prng_reseed(&state); }
	aes128_prng_enc_mix_n2(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_enc_mix_n2(&state);
		return union_128{.xmm = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_enc_mix_n2>);

class aes128_prng_enc_mix_n3
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_enc_mix_n3() { aes128_prng_reseed(&state); }
	aes128_prng_enc_mix_n3(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_enc_mix_n3(&state);
		return union_128{.xmm = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_enc_mix_n3>);

class aes128_prng_enc_mix_n4
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_enc_mix_n4() { aes128_prng_reseed(&state); }
	aes128_prng_enc_mix_n4(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_enc_mix_n4(&state);
		return union_128{.xmm = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_enc_mix_n4>);

class aes128_prng_dec_dm_n1
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_dec_dm_n1() { aes128_prng_reseed(&state); }
	aes128_prng_dec_dm_n1(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_dec_dm_n1(&state);
		return union_128{.xmm = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_dec_dm_n1>);

class aes128_prng_dec_dm_n2
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_dec_dm_n2() { aes128_prng_reseed(&state); }
	aes128_prng_dec_dm_n2(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_dec_dm_n2(&state);
		return union_128{.xmm = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_dec_dm_n2>);

class aes128_prng_dec_dm_n3
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_dec_dm_n3() { aes128_prng_reseed(&state); }
	aes128_prng_dec_dm_n3(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_dec_dm_n3(&state);
		return union_128{.xmm = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_dec_dm_n3>);

class aes128_prng_dec_dm_n4
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_dec_dm_n4() { aes128_prng_reseed(&state); }
	aes128_prng_dec_dm_n4(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_dec_dm_n4(&state);
		return union_128{.xmm = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_dec_dm_n4>);

class aes128_prng_dec_mix_n1
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_dec_mix_n1() { aes128_prng_reseed(&state); }
	aes128_prng_dec_mix_n1(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_dec_mix_n1(&state);
		return union_128{.xmm = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_dec_mix_n1>);

class aes128_prng_dec_mix_n2
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_dec_mix_n2() { aes128_prng_reseed(&state); }
	aes128_prng_dec_mix_n2(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_dec_mix_n2(&state);
		return union_128{.xmm = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_dec_mix_n2>);

class aes128_prng_dec_mix_n3
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_dec_mix_n3() { aes128_prng_reseed(&state); }
	aes128_prng_dec_mix_n3(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_dec_mix_n3(&state);
		return union_128{.xmm = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_dec_mix_n3>);

class aes128_prng_dec_mix_n4
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_dec_mix_n4() { aes128_prng_reseed(&state); }
	aes128_prng_dec_mix_n4(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_dec_mix_n4(&state);
		return union_128{.xmm = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_dec_mix_n4>);

#else
#warning "__AES__ not defined"
#endif
