// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// wyhash primes
/**
\file
\author Steven Ward
\sa https://github.com/wangyi-fudan/wyhash/blob/master/wyhash.h#L144
*/

#pragma once

#include <array>
#include <cstdint>

namespace
{
inline constexpr std::array<uint64_t, 4> _wyp = {
	0xa0761d6478bd642f, // prime (popcount = 32)
	0xe7037ed1a0b428db, // prime (popcount = 32)
	0x8ebc6af09c88c6e3, // prime (popcount = 32)
	0x589965cc75374cc3, // prime (popcount = 32)
};
static_assert((_wyp[0] & 1) != 0, "must be odd");
static_assert((_wyp[1] & 1) != 0, "must be odd");
static_assert((_wyp[2] & 1) != 0, "must be odd");
static_assert((_wyp[3] & 1) != 0, "must be odd");
}
