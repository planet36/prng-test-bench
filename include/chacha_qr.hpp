// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// ChaCha quarter-round function
/**
\file
\author Steven Ward
\sa https://cr.yp.to/chacha/chacha-20080128.pdf
\sa https://datatracker.ietf.org/doc/html/rfc7539#section-2.1
*/

#pragma once

#include <bit>
#include <concepts>

template <std::unsigned_integral T>
constexpr void
chacha_qr(T& a, T& b, T& c, T& d)
{
	a += b; d = std::rotl(a ^ d, 16);
	c += d; b = std::rotl(b ^ c, 12);
	a += b; d = std::rotl(a ^ d,  8);
	c += d; b = std::rotl(b ^ c,  7);
}
