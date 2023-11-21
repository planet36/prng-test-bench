// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Romu PRNG
/**
\file
\author Steven Ward
\sa https://www.romu-random.org/code.c
*/

#pragma once

#include "def_urbg_class.hpp"

DEF_URBG_CLASS(romu_quad, SINGLE_ARG(std::array<uint64_t, 4>), uint64_t)
{
	static constexpr uint64_t M1 = 0xd3833e804f4c574b; // not prime (popcount = 31)
	static_assert(M1 & 1, "must be odd");
	static constexpr unsigned int R1 = 52;
	static constexpr unsigned int R2 = 19;

	const auto old_s = s;

	s[0] = M1 * old_s[3] + 1;                  // a-mult (SDW)
	s[1] = old_s[3] + std::rotl(old_s[0], R1); // b-rotl, c-add
	s[2] = old_s[2] - old_s[1];                // d-sub
	s[3] = std::rotl(old_s[2] + old_s[0], R2); // e-add, f-rotl (SDW)
	return old_s[1];
}

DEF_URBG_CLASS(romu_trio, SINGLE_ARG(std::array<uint64_t, 3>), uint64_t)
{
	static constexpr uint64_t M1 = 0xd3833e804f4c574b; // not prime (popcount = 31)
	static_assert(M1 & 1, "must be odd");
	static constexpr unsigned int R1 = 12;
	static constexpr unsigned int R2 = 44;

	const auto old_s = s;

	s[0] = M1 * old_s[2] + 1; // (SDW)
	s[1] = std::rotl(old_s[1] - old_s[0], R1); // (SDW)
	s[2] = std::rotl(old_s[2] - old_s[1], R2); // (SDW)
	return old_s[0];
}

DEF_URBG_CLASS(romu_duo, SINGLE_ARG(std::array<uint64_t, 2>), uint64_t)
{
	static constexpr uint64_t M1 = 0xd3833e804f4c574b; // not prime (popcount = 31)
	static_assert(M1 & 1, "must be odd");
	static constexpr unsigned int R1 = 36;
	static constexpr unsigned int R2 = 15;

	const auto old_s = s;

	s[0] = M1 * old_s[1] + 1; // (SDW)
	s[1] = std::rotl(old_s[1], R1) + std::rotl(old_s[1], R2) - old_s[0]; // (SDW)
	return old_s[0];
}

DEF_URBG_CLASS(romu_duo_jr, SINGLE_ARG(std::array<uint64_t, 2>), uint64_t)
{
	static constexpr uint64_t M1 = 0xd3833e804f4c574b; // not prime (popcount = 31)
	static_assert(M1 & 1, "must be odd");
	static constexpr unsigned int R1 = 27;

	const auto old_s = s;

	s[0] = M1 * old_s[1] + 1; // (SDW)
	s[1] = std::rotl(old_s[1] - old_s[0], R1); // (SDW)
	return old_s[0];
}

DEF_URBG_CLASS(romu_quad32, SINGLE_ARG(std::array<uint32_t, 4>), uint32_t)
{
	static constexpr uint32_t M1 = 0xc61d672b; // not prime (popcount = 17)
	static_assert(M1 & 1, "must be odd");
	static constexpr unsigned int R1 = 26;
	static constexpr unsigned int R2 = 9;

	const auto old_s = s;

	s[0] = M1 * old_s[3] + 1;                  // a-mult (SDW)
	s[1] = old_s[3] + std::rotl(old_s[0], R1); // b-rotl, c-add
	s[2] = old_s[2] - old_s[1];                // d-sub
	s[3] = std::rotl(old_s[2] + old_s[0], R2); // e-add, f-rotl (SDW)
	return old_s[1];
}

DEF_URBG_CLASS(romu_trio32, SINGLE_ARG(std::array<uint32_t, 3>), uint32_t)
{
	static constexpr uint32_t M1 = 0xc61d672b; // not prime (popcount = 17)
	static_assert(M1 & 1, "must be odd");
	static constexpr unsigned int R1 = 6;
	static constexpr unsigned int R2 = 22;

	const auto old_s = s;

	s[0] = M1 * old_s[2] + 1; // (SDW)
	s[1] = std::rotl(old_s[1] - old_s[0], R1); // (SDW)
	s[2] = std::rotl(old_s[2] - old_s[1], R2); // (SDW)
	return old_s[0];
}

// XXX: RomuMono32 returns uint16_t
