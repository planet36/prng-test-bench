// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// ttwanghash64 PRNG
/**
\file
\author Steven Ward
\sa https://web.archive.org/web/20121102023700/http://www.concentric.net/~ttwang/tech/inthash.htm
*/

#pragma once

#include "def_urbg_class.hpp"

#include <cstdint>

DEF_URBG_CLASS(ttwanghash64, uint64_t, uint64_t)
{
	auto x = s++; // (SDW)

	x = (~x) + (x << 21); // x = (x << 21) - x - 1;
	x ^= (x >> 24);
	x += (x << 3) + (x << 8); // x * 265
	x ^= (x >> 14);
	x += (x << 2) + (x << 4); // x * 21
	x ^= (x >> 28);
	x += (x << 31);
	return x;
}
