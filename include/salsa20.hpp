// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Salsal20
/**
\file
\author Steven Ward
\sa https://cr.yp.to/chacha/chacha-20080128.pdf
\sa https://datatracker.ietf.org/doc/html/rfc7539#section-2.1
*/

#pragma once

#include "salsa20_qr.hpp"
#include "def_urbg_class.hpp"
#include "union_128.h"
#include "xxhprimes.hpp"

#include <array>
#include <cstdint>

DEF_URBG_CLASS(salsa20, SINGLE_ARG(std::array<uint32_t, 4>), __uint128_t)
{
	static constexpr std::array<uint32_t, 4> inc {
		xxh_prime32[0],
		xxh_prime32[1],
		xxh_prime32[2],
		xxh_prime32[3],
	};

	union_128 result;
	result.u32[0] = s[0];
	result.u32[1] = s[1];
	result.u32[2] = s[2];
	result.u32[3] = s[3];

	s[0] += inc[0];
	s[1] += inc[1];
	s[2] += inc[2];
	s[3] += inc[3];

	salsa20_qr(result.u32[0], result.u32[1], result.u32[2], result.u32[3]);
	salsa20_qr(result.u32[1], result.u32[2], result.u32[3], result.u32[0]);
	salsa20_qr(result.u32[2], result.u32[3], result.u32[0], result.u32[1]);
	salsa20_qr(result.u32[3], result.u32[0], result.u32[1], result.u32[2]);

	return result.u128;
}
