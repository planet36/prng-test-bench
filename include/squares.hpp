// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// squares32, squares64 PRNGs
/**
\file
\author Steven Ward
\sa https://arxiv.org/pdf/2004.06278.pdf
\sa https://www.arxiv-vanity.com/papers/2004.06278/
*/

#pragma once

#include "def_urbg_class.hpp"
#include "xxhprimes.hpp"

#include <bit>
#include <cstdint>

DEF_URBG_CLASS(squares32, uint64_t, uint32_t)
{
	static constexpr uint64_t key = xxh_prime64[0];
	static_assert(key & 1, "must be odd");

	++s; // (SDW)

	auto x = s * key;
	const auto y = x;
	const auto z = y + key;

	// round 1
	x = x * x + y;
	x = std::rotl(x, 32);

	// round 2
	x = x * x + z;
	x = std::rotl(x, 32);

	// round 3
	x = x * x + y;
	x = std::rotl(x, 32);

	// round 4
	return (x * x + z) >> 32;
}

DEF_URBG_CLASS(squares64, uint64_t, uint64_t)
{
	static constexpr uint64_t key = xxh_prime64[0];
	static_assert(key & 1, "must be odd");

	++s; // (SDW)

	auto x = s * key;
	const auto y = x;
	const auto z = y + key;

	// round 1
	x = x * x + y;
	x = std::rotl(x, 32);

	// round 2
	x = x * x + z;
	x = std::rotl(x, 32);

	// round 3
	x = x * x + y;
	x = std::rotl(x, 32);

	// round 4
	x = x * x + z;
	const auto t = x;
	x = std::rotl(x, 32);

	// round 5
	return t ^ ((x * x + y) >> 32);
}
