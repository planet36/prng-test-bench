// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// degski32, degski64 PRNGs
/**
\file
\author Steven Ward
*/

#pragma once

#include "def_urbg_class.hpp"
#include "xxhprimes.hpp"

#include <cstdint>

// https://gist.github.com/degski/6e2069d6035ae04d5d6f64981c995ec2#file-invertible_hash_functions-hpp-L29
DEF_URBG_CLASS(degski32, uint32_t, uint32_t)
{
	static constexpr uint32_t inc = xxh_prime32[0]; // inc=1 yields failures
	static_assert(inc & 1, "must be odd");

	static constexpr uint32_t M1 = 0x45d9f3b; // not prime (popcount = 17)
	static constexpr uint32_t M2 = 0x45d9f3b; // not prime (popcount = 17)
	static_assert(M1 & 1, "must be odd");
	static_assert(M2 & 1, "must be odd");
	static constexpr unsigned int S1 = 16;
	static constexpr unsigned int S2 = 16;
	static constexpr unsigned int S3 = 16;

	auto x = s;
	s += inc; // (SDW)

	x ^= x >> S1; x *= M1;
	x ^= x >> S2; x *= M2;
	x ^= x >> S3;
	return x;
}

// https://gist.github.com/degski/6e2069d6035ae04d5d6f64981c995ec2#file-invertible_hash_functions-hpp-L43
DEF_URBG_CLASS(degski64, uint64_t, uint64_t)
{
	static constexpr uint64_t inc = xxh_prime64[0]; // inc=1 yields failures
	static_assert(inc & 1, "must be odd");

	static constexpr uint64_t M1 = 0xd6e8feb86659fd93; // not prime (popcount = 39)
	static constexpr uint64_t M2 = 0xd6e8feb86659fd93; // not prime (popcount = 39)
	static_assert(M1 & 1, "must be odd");
	static_assert(M2 & 1, "must be odd");
	static constexpr unsigned int S1 = 32;
	static constexpr unsigned int S2 = 32;
	static constexpr unsigned int S3 = 32;

	auto x = s;
	s += inc; // (SDW)

	x ^= x >> S1; x *= M1;
	x ^= x >> S2; x *= M2;
	x ^= x >> S3;
	return x;
}
