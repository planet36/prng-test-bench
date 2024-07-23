// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Macro that defines a Uniform Random Bit Generator class
/**
\file
\author Steven Ward
\sa https://en.cppreference.com/w/cpp/named_req/UniformRandomBitGenerator
\sa https://en.cppreference.com/w/cpp/named_req/RandomNumberEngine
\sa https://eel.is/c++draft/rand.req.urng
\sa https://eel.is/c++draft/rand.req.eng
Note: This does not meet the requirements of a random number engine.
The default ctor does not create an engine with the same initial state as all
other default-constructed engines of the same type.
*/

#pragma once

#include "fill_rand.hpp"

#include <array>
#include <cstdint>
#include <cstring>
#include <limits>
#include <type_traits>

// https://stackoverflow.com/a/13842612
#define SINGLE_ARG(...) __VA_ARGS__
// Use SINGLE_ARG when a macro arg has a comma.

// XXX: must define member function "next()" immediately after calling this macro
#define DEF_URBG_CLASS(CLASS_NAME, STATE_TYPE, RESULT_TYPE)              \
struct CLASS_NAME                                                        \
{                                                                        \
	using state_type = STATE_TYPE;                                       \
	using result_type = RESULT_TYPE;                                     \
                                                                         \
private:                                                                 \
	state_type s{};                                                      \
	void zeroize()                                                       \
	{                                                                    \
		(void)std::memset(&s, 0, sizeof(state_type));                    \
	}                                                                    \
                                                                         \
public:                                                                  \
	using seed_bytes_type = std::array<uint8_t, sizeof(state_type)>;     \
	/* https://eel.is/c++draft/rand.req.urng */                          \
	static_assert(std::is_unsigned_v<result_type>);                      \
	static constexpr result_type min()                                   \
	{                                                                    \
		return std::numeric_limits<result_type>::min();                  \
	}                                                                    \
	static constexpr result_type max()                                   \
	{                                                                    \
		return std::numeric_limits<result_type>::max();                  \
	}                                                                    \
	constexpr result_type operator()()                                   \
	{                                                                    \
		return next();                                                   \
	}                                                                    \
	/* https://eel.is/c++draft/rand.req.eng#3.1 */                       \
	static_assert(sizeof(state_type) % sizeof(result_type) == 0);        \
	/* dtor */                                                           \
	~CLASS_NAME()                                                        \
	{                                                                    \
		zeroize();                                                       \
	}                                                                    \
	/* ctors */                                                          \
	constexpr CLASS_NAME() noexcept                                      \
	{                                                                    \
		seed();                                                          \
	}                                                                    \
	explicit constexpr CLASS_NAME(const state_type& new_s) noexcept      \
	{                                                                    \
		seed(new_s);                                                     \
	}                                                                    \
	explicit constexpr CLASS_NAME(const seed_bytes_type& bytes) noexcept \
	{                                                                    \
		seed(bytes);                                                     \
	}                                                                    \
	CLASS_NAME(const CLASS_NAME&) = default;                             \
	/* copy assignment operator */                                       \
	CLASS_NAME& operator=(const CLASS_NAME&) = default;                  \
	/* seed functions */                                                 \
	void seed() noexcept                                                 \
	{                                                                    \
		fill_rand(s);                                                    \
	}                                                                    \
	constexpr void seed(const state_type& new_s) noexcept                \
	{                                                                    \
		s = new_s;                                                       \
	}                                                                    \
	void seed(const seed_bytes_type& bytes) noexcept                     \
	{                                                                    \
		(void)std::memcpy(&s, bytes.data(), sizeof(state_type));         \
	}                                                                    \
	/* non-static member function declaration */                         \
	constexpr result_type next(); /* XXX: must define this below */      \
};                                                                       \
constexpr auto CLASS_NAME::next() -> result_type
