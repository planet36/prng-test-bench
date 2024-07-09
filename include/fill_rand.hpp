// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Fill a buffer with random bytes
/**
\file
\author Steven Ward
*/

#pragma once

#include <ranges>
#include <span>

#if defined(_GLIBCXX_HAVE_ARC4RANDOM)

#include <cstdlib>

// arc4random_buf
// https://www.gnu.org/software/libc/manual/html_node/High-Quality-Random.html

template <typename T>
requires (!std::ranges::contiguous_range<T>)
void
fill_rand(T& x) noexcept
{
	arc4random_buf(&x, sizeof(T));
}

void
fill_rand(std::ranges::contiguous_range auto& container)
{
	auto span_bytes = std::as_writable_bytes(std::span{container});
	arc4random_buf(std::data(span_bytes), span_bytes.size_bytes());
}

#elif defined(_GLIBCXX_HAVE_GETENTROPY)

#include <system_error>
#include <unistd.h>

// getentropy
// https://www.gnu.org/software/libc/manual/html_node/Unpredictable-Bytes.html
// Max num bytes allowed is 256

template <typename T>
requires (!std::ranges::contiguous_range<T> && (sizeof(T) <= 256))
void
fill_rand(T& x)
{
	if (getentropy(&x, sizeof(T)) < 0)
	{
		throw std::system_error(std::make_error_code(std::errc{errno}),
		                        "getentropy");
	}
}

void
fill_rand(std::ranges::contiguous_range auto& container)
{
	auto span_bytes = std::as_writable_bytes(std::span{container});

	if (getentropy(std::data(span_bytes), span_bytes.size_bytes()) < 0)
	{
		throw std::system_error(std::make_error_code(std::errc{errno}),
		                        "getentropy");
	}
}

#else

#include "int_join.hpp"

#include <concepts>
#include <random>
#include <utility>

template <std::unsigned_integral T>
void
fill_rand(T& x)
{
	static std::random_device rd;

	// std::random_device::result_type is unsigned int
	// https://en.cppreference.com/w/cpp/numeric/random/random_device
	if constexpr (sizeof(T) <= sizeof(typename std::random_device::result_type))
		x = rd();
	else if constexpr (sizeof(T) <=
	                   2 * sizeof(typename std::random_device::result_type))
		x = int_join(rd(), rd());
	else
		std::unreachable();
}

template <std::unsigned_integral T, size_t N>
void
fill_rand(std::array<T, N>& container)
{
	static std::random_device rd;

	for (T& x : container)
	{
		// std::random_device::result_type is unsigned int
		// https://en.cppreference.com/w/cpp/numeric/random/random_device
		if constexpr (sizeof(T) <=
		              sizeof(typename std::random_device::result_type))
			x = rd();
		else if constexpr (sizeof(T) <=
		                   2 * sizeof(typename std::random_device::result_type))
			x = int_join(rd(), rd());
		else
			std::unreachable();
	}
}

template <std::unsigned_integral T>
void
fill_rand(std::vector<T>& container)
{
	static std::random_device rd;

	for (T& x : container)
	{
		// std::random_device::result_type is unsigned int
		// https://en.cppreference.com/w/cpp/numeric/random/random_device
		if constexpr (sizeof(T) <=
		              sizeof(typename std::random_device::result_type))
			x = rd();
		else if constexpr (sizeof(T) <=
		                   2 * sizeof(typename std::random_device::result_type))
			x = int_join(rd(), rd());
		else
			std::unreachable();
	}
}

#endif
