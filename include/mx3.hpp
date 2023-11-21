// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// mx3 PRNG
/**
\file
\author Steven Ward
\sa https://github.com/jonmaiga/mx3/blob/master/mx3.h
*/

#pragma once

#include "def_urbg_class.hpp"

DEF_URBG_CLASS(mx3, uint64_t, uint64_t)
{
	static constexpr uint64_t M1 = 0xbea225f9eb34556d; // not prime (popcount = 36)
	static_assert(M1 & 1, "must be odd");
	static constexpr unsigned int S1 = 32;
	static constexpr unsigned int S2 = 29;
	static constexpr unsigned int S3 = 32;
	static constexpr unsigned int S4 = 29;

	// NOTE: Their algorithm adds M1 to the seed.
	// NOTE: Their state is named "_counter".
	auto x = s++;

	x ^= x >> S1; x *= M1;
	x ^= x >> S2; x *= M1;
	x ^= x >> S3; x *= M1;
	x ^= x >> S4;
	return x;
}
