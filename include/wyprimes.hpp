// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// wyhash & wyrand primes
/**
* \file
* \author Steven Ward
* \sa https://github.com/wangyi-fudan/wyhash/blob/master/wyhash.h#L144
*/

#pragma once

#include <array>
#include <cstdint>

namespace
{

inline constexpr std::array<uint64_t, 4> _wyp{
    0x2d358dccaa6c78a5, // prime (popcount = 32)
    0x8bb84b93962eacc9, // prime (popcount = 32)
    0x4b33a62ed433d4a3, // prime (popcount = 32)
    0x4d5a2da51de1aa47, // prime (popcount = 32)
};
static_assert((_wyp[0] & 1) != 0, "must be odd");
static_assert((_wyp[1] & 1) != 0, "must be odd");
static_assert((_wyp[2] & 1) != 0, "must be odd");
static_assert((_wyp[3] & 1) != 0, "must be odd");

}
