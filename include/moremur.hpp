// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// moremur PRNG
/**
\file
\author Steven Ward
\sa https://mostlymangling.blogspot.com/2019/12/stronger-better-morer-moremur-better.html
*/

#pragma once

#include "def_urbg_class.hpp"
#include "scaled-const.hpp"

#include <cstdint>

DEF_URBG_CLASS(moremur, uint64_t, uint64_t)
{
	static constexpr uint64_t inc = GoldenRatio_64;
	static_assert((inc & 1) != 0, "must be odd");

	static constexpr uint64_t M1 = 0x3c79ac492ba7b653; // not prime (popcount = 34)
	static constexpr uint64_t M2 = 0x1c69b3f74ac4ae35; // not prime (popcount = 34)
	static constexpr unsigned int S1 = 27;
	static constexpr unsigned int S2 = 33;
	static constexpr unsigned int S3 = 27;
	static_assert((M1 & 1) != 0, "must be odd");
	static_assert((M2 & 1) != 0, "must be odd");

	auto x = s;
	s += inc; // (SDW)

	x ^= x >> S1;
	x *= M1;
	x ^= x >> S2;
	x *= M2;
	x ^= x >> S3;
	return x;
}
