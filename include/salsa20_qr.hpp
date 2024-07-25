// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Salsa20 quarter-round function
/**
\file
\author Steven Ward
\sa https://cr.yp.to/chacha/chacha-20080128.pdf
*/

#pragma once

#include <bit>
#include <concepts>

template <std::unsigned_integral T>
constexpr void
salsa20_qr(T& a, T& b, T& c, T& d)
{
	b ^= std::rotl(a + d,  7);
	c ^= std::rotl(a + b,  9);
	d ^= std::rotl(b + c, 13);
	a ^= std::rotl(c + d, 18);
}
