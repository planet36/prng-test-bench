// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// rrmxmx PRNG
/**
\file
\author Steven Ward
\sa https://mostlymangling.blogspot.com/2019/01/better-stronger-mixer-and-test-procedure.html
*/

#pragma once

#include "def_urbg_class.hpp"

#include <bit>
#include <cstdint>

// Old mixer, my rrmxmx
DEF_URBG_CLASS(rrmxmx, uint64_t, uint64_t)
{
	static constexpr uint64_t M1 = 0x9fb21c651e98df25; // prime (popcount = 34)
	static constexpr uint64_t M2 = 0x9fb21c651e98df25; // prime (popcount = 34)
	static_assert(M1 & 1, "must be odd");
	static_assert(M2 & 1, "must be odd");
	static constexpr unsigned int R1 = 49;
	static constexpr unsigned int R2 = 24;
	static constexpr unsigned int S1 = 28;
	static constexpr unsigned int S2 = 28;

	auto x = s++; // (SDW)

	x ^= std::rotr(x, R1) ^ std::rotr(x, R2);
	x *= M1;
	x ^= x >> S1;
	x *= M2;
	x ^= x >> S2;
	return x;
}
