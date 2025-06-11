// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// xxhash primes
/**
\file
\author Steven Ward
\sa https://github.com/Cyan4973/xxHash/blob/dev/xxhash.h
*/

#pragma once

#include <array>
#include <cstdint>

namespace
{

// https://github.com/Cyan4973/xxHash/blob/release/xxhash.h#L2560
inline constexpr std::array<uint32_t, 5> xxh_prime32 = {
    0x9e3779b1, // prime (popcount = 19)
    0x85ebca77, // prime (popcount = 19)
    0xc2b2ae3d, // prime (popcount = 17)
    0x27d4eb2f, // prime (popcount = 19)
    0x165667b1, // prime (popcount = 16)
};
static_assert((xxh_prime32[0] & 1) != 0, "must be odd");
static_assert((xxh_prime32[1] & 1) != 0, "must be odd");
static_assert((xxh_prime32[2] & 1) != 0, "must be odd");
static_assert((xxh_prime32[3] & 1) != 0, "must be odd");
static_assert((xxh_prime32[4] & 1) != 0, "must be odd");

// https://github.com/Cyan4973/xxHash/blob/release/xxhash.h#L3093
inline constexpr std::array<uint64_t, 5> xxh_prime64 = {
    0x9e3779b185ebca87, // prime (popcount = 36)
    0xc2b2ae3d27d4eb4f, // prime (popcount = 36)
    0x165667b19e3779f9, // prime (popcount = 37)
    0x85ebca77c2b2ae63, // prime (popcount = 35)
    0x27d4eb2f165667c5, // prime (popcount = 35)
};
static_assert((xxh_prime64[0] & 1) != 0, "must be odd");
static_assert((xxh_prime64[1] & 1) != 0, "must be odd");
static_assert((xxh_prime64[2] & 1) != 0, "must be odd");
static_assert((xxh_prime64[3] & 1) != 0, "must be odd");
static_assert((xxh_prime64[4] & 1) != 0, "must be odd");

}
