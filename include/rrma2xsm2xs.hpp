// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// rrma2xsm2xs PRNG
/**
\file
\author Steven Ward
\sa http://mostlymangling.blogspot.com/2020/01/nasam-not-another-strange-acronym-mixer.html
*/

#pragma once

#include "def_urbg_class.hpp"
#include "scaled-const.hpp"

#include <bit>

DEF_URBG_CLASS(rrma2xsm2xs, uint64_t, uint64_t)
{
	static constexpr uint64_t M1 = 0x9e6c63d0676a9a99; // not prime (popcount = 33)
	static_assert(M1 & 1, "must be odd");
	static constexpr uint64_t M2 = 0x9e6d62d06f6a9a9b; // not prime (popcount = 35)
	static_assert(M2 & 1, "must be odd");

	static constexpr auto C = GoldenRatio_64; // (SDW)

	static constexpr unsigned int R1 = 25;
	static constexpr unsigned int R2 = 47;
	static constexpr unsigned int S1 = 23;
	static constexpr unsigned int S2 = 51;

	s ^= std::rotr(s, R1) ^ std::rotr(s, R2);
	s = s * M1 + C; // Avoids trivial fixpoint at 0.
	s ^= (s >> S1) ^ (s >> S2);
	s *= M2;
	s ^= (s >> S1) ^ (s >> S2);

	return s;
}
