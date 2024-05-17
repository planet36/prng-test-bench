// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Carry-less Multiplication (CLMUL) PRNG
/**
\file
\author Steven Ward
\sa https://en.wikipedia.org/wiki/CLMUL_instruction_set
\sa https://www.felixcloutier.com/x86/pclmulqdq
*/

#pragma once

#include "byteprimes.hpp"
#include "clmum.hpp"
#include "def_urbg_class.hpp"
#include "simd-types.hpp"

#include <cstdint>

DEF_URBG_CLASS(clmulrand, simd128, uint64_t)
{
	static constexpr simd128 inc{.u64vec{byteprimes[0], byteprimes[1]}};
	static_assert(inc.u64vec[0] & 1, "must be odd");
	static_assert(inc.u64vec[1] & 1, "must be odd");

	s.u64vec += inc.u64vec;
	return clmums(s);
}
