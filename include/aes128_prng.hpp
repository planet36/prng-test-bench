// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

#pragma once

#include "aes128_prng.h"

#include <type_traits>
#include <immintrin.h>

#if defined(__AES__)

union i128
{
	__int128_t i128;
	__uint128_t u128;
	__m128i m128i;
};

// Must use -std=gnu++XX for these static_asserts to succeed.
// https://stackoverflow.com/a/71710062/1892784

static_assert(std::integral<__int128_t>);
static_assert(std::signed_integral<__int128_t>);

static_assert(std::integral<__uint128_t>);
static_assert(std::unsigned_integral<__uint128_t>);

class aes128_prng_dm_n1
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_dm_n1() { aes128_prng_reseed(&state); }
	aes128_prng_dm_n1(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_dm_n1(&state);
		return i128{.m128i = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_dm_n1>);

class aes128_prng_dm_n2
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_dm_n2() { aes128_prng_reseed(&state); }
	aes128_prng_dm_n2(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_dm_n2(&state);
		return i128{.m128i = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_dm_n2>);

class aes128_prng_dm_n3
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_dm_n3() { aes128_prng_reseed(&state); }
	aes128_prng_dm_n3(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_dm_n3(&state);
		return i128{.m128i = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_dm_n3>);

class aes128_prng_dm_n4
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_dm_n4() { aes128_prng_reseed(&state); }
	aes128_prng_dm_n4(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_dm_n4(&state);
		return i128{.m128i = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_dm_n4>);

class aes128_prng_mix_n1
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_mix_n1() { aes128_prng_reseed(&state); }
	aes128_prng_mix_n1(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_mix_n1(&state);
		return i128{.m128i = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_mix_n1>);

class aes128_prng_mix_n2
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_mix_n2() { aes128_prng_reseed(&state); }
	aes128_prng_mix_n2(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_mix_n2(&state);
		return i128{.m128i = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_mix_n2>);

class aes128_prng_mix_n3
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_mix_n3() { aes128_prng_reseed(&state); }
	aes128_prng_mix_n3(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_mix_n3(&state);
		return i128{.m128i = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_mix_n3>);

class aes128_prng_mix_n4
{
private:
	aes128_prng state;
public:
	using result_type = __uint128_t;
	using seed_bytes_type = std::array<uint8_t, sizeof(__m128i)>;
	aes128_prng_mix_n4() { aes128_prng_reseed(&state); }
	aes128_prng_mix_n4(const seed_bytes_type& bytes) {
		aes128_prng_reseed(&state);
		(void)std::memcpy(&state.x, bytes.data(), sizeof(__m128i)); }
	static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
	static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
	constexpr result_type operator()() { return next(); }
	result_type next() {
		const __m128i result = aes128_prng_next_mix_n4(&state);
		return i128{.m128i = result}.u128;
	}
};
static_assert(std::uniform_random_bit_generator<aes128_prng_mix_n4>);

#else
#warning "__AES__ not defined"
#endif