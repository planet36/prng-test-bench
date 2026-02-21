// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// xxhash primes
/**
* \file
* \author Steven Ward
* \sa https://github.com/Cyan4973/xxHash/blob/dev/xxhash.h
*/

#pragma once

#include <array>
#include <cstdint>

namespace
{

// https://github.com/Cyan4973/xxHash/blob/release/xxhash.h#L2903
inline constexpr uint32_t XXH_PRIME32_1 = 0x9E3779B1; // prime (popcount = 19)
inline constexpr uint32_t XXH_PRIME32_2 = 0x85EBCA77; // prime (popcount = 19)
inline constexpr uint32_t XXH_PRIME32_3 = 0xC2B2AE3D; // prime (popcount = 17)
inline constexpr uint32_t XXH_PRIME32_4 = 0x27D4EB2F; // prime (popcount = 19)
inline constexpr uint32_t XXH_PRIME32_5 = 0x165667B1; // prime (popcount = 16)

static_assert((XXH_PRIME32_1 & 1) != 0, "must be odd");
static_assert((XXH_PRIME32_2 & 1) != 0, "must be odd");
static_assert((XXH_PRIME32_3 & 1) != 0, "must be odd");
static_assert((XXH_PRIME32_4 & 1) != 0, "must be odd");
static_assert((XXH_PRIME32_5 & 1) != 0, "must be odd");

// https://github.com/Cyan4973/xxHash/blob/release/xxhash.h#L3454
inline constexpr uint64_t XXH_PRIME64_1 = 0x9E3779B185EBCA87; // prime (popcount = 36)
inline constexpr uint64_t XXH_PRIME64_2 = 0xC2B2AE3D27D4EB4F; // prime (popcount = 36)
inline constexpr uint64_t XXH_PRIME64_3 = 0x165667B19E3779F9; // prime (popcount = 37)
inline constexpr uint64_t XXH_PRIME64_4 = 0x85EBCA77C2B2AE63; // prime (popcount = 35)
inline constexpr uint64_t XXH_PRIME64_5 = 0x27D4EB2F165667C5; // prime (popcount = 35)

static_assert((XXH_PRIME64_1 & 1) != 0, "must be odd");
static_assert((XXH_PRIME64_2 & 1) != 0, "must be odd");
static_assert((XXH_PRIME64_3 & 1) != 0, "must be odd");
static_assert((XXH_PRIME64_4 & 1) != 0, "must be odd");
static_assert((XXH_PRIME64_5 & 1) != 0, "must be odd");

// https://github.com/Cyan4973/xxHash/blob/release/xxhash.h#L4380
inline constexpr uint64_t PRIME_MX1 = 0x165667919E3779F9; // not prime (popcount = 36)
inline constexpr uint64_t PRIME_MX2 = 0x9FB21C651E98DF25; // prime (popcount = 34)

static_assert((PRIME_MX1 & 1) != 0, "must be odd");
static_assert((PRIME_MX2 & 1) != 0, "must be odd");

}
