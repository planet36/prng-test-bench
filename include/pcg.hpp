// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// PCG PRNG
/**
* \file
* \author Steven Ward
* \sa https://www.pcg-random.org/
*/

#pragma once

#include "abstract_urbg_class.hpp"
#include "int_join.hpp"

#include <array>
#include <bit>
#include <cstdint>

/*
* Multiplier and increment values taken from here:
* https://github.com/imneme/pcg-cpp/blob/master/include/pcg_random.hpp#L158
* https://github.com/imneme/pcg-cpp/blob/master/include/pcg_random.hpp#L161
*/

namespace
{

inline constexpr std::array<uint64_t, 3> pcg_const64 = {
    0x5851f42d4c957f2d, // not prime (popcount = 33)
    0x14057b7ef767814f, // not prime (popcount = 35)
    0xda942042e4dd58b5, // not prime (popcount = 29)
};
static_assert((pcg_const64[0] & 1) != 0, "must be odd");
static_assert((pcg_const64[1] & 1) != 0, "must be odd");
static_assert((pcg_const64[2] & 1) != 0, "must be odd");

#if defined(__SIZEOF_INT128__)
inline constexpr std::array<__uint128_t, 2> pcg_const128 = {
    int_join(UINT64_C(0x2360ed051fc65da4),
             UINT64_C(0x4385df649fccf645)),   // not prime (popcount = 65)
    int_join(pcg_const64[0], pcg_const64[1]), // not prime (popcount = 68)
};
static_assert((pcg_const128[0] & 1) != 0, "must be odd");
static_assert((pcg_const128[1] & 1) != 0, "must be odd");
#endif

}

/// PCG-XSH-RR
DEF_URBG_SUBCLASS(pcg32, uint64_t, uint32_t)

/// prepare the initial state
void
pcg32::init()
{}

pcg32::result_type
pcg32::next()
{
    constexpr state_type mul = pcg_const64[0];
    static_assert((mul & 1) != 0, "must be odd");
    constexpr state_type inc = pcg_const64[1];
    static_assert((inc & 1) != 0, "must be odd");

    const auto old_s = s;
    s = s * mul + inc;

    // (5 + 32) / 2 == 18
    // 32 - 5 == 27
    // 64 - 5 == 59
    const result_type output = (old_s ^ (old_s >> 18)) >> 27;
    const int rot = old_s >> 59;
    return std::rotr(output, rot);
}

/// PCG-XSH-RS
DEF_URBG_SUBCLASS(pcg32_fast, uint64_t, uint32_t)

/// prepare the initial state
void
pcg32_fast::init()
{}

pcg32_fast::result_type
pcg32_fast::next()
{
    constexpr state_type mul = pcg_const64[0];
    static_assert((mul & 1) != 0, "must be odd");
    constexpr state_type inc = pcg_const64[1];
    static_assert((inc & 1) != 0, "must be odd");

    const auto old_s = s;
    s = s * mul + inc;

    // 32 - 3 - 7 == 22
    // 64 - 3 == 61
    const result_type output = (old_s ^ (old_s >> 22)) >> (22 + (old_s >> 61));
    return output;
}

#if defined(__SIZEOF_INT128__)
/// PCG-XSL-RR
DEF_URBG_SUBCLASS(pcg64, __uint128_t, uint64_t)

/// prepare the initial state
void
pcg64::init()
{}

pcg64::result_type
pcg64::next()
{
    /* "There is no support in GCC for expressing an integer constant of
    *  type __int128 for targets with long long integer less than 128 bits
    *  wide."
    *  https://gcc.gnu.org/onlinedocs/gcc/_005f_005fint128.html
    */
    constexpr __uint128_t mul = pcg_const128[0];
    static_assert((mul & 1) != 0, "must be odd");
    constexpr __uint128_t inc = pcg_const128[1];
    static_assert((inc & 1) != 0, "must be odd");

    const auto old_s = s;
    s = s * mul + inc;

    // 128 / 2 == 64
    // 128 - 6 == 122
    const result_type output = old_s ^ (old_s >> 64);
    const int rot = old_s >> 122;
    return std::rotr(output, rot);
}

/// PCG-DXSM
/**
* Adapted from
* https://github.com/numpy/numpy/issues/13635#issuecomment-506088698
* https://github.com/imneme/pcg-cpp/blob/master/include/pcg_random.hpp#L1031
* https://dotat.at/@/2023-06-21-pcg64-dxsm.html
* https://dotat.at/cgi/git/pcg-dxsm.git/blob/HEAD:/pcg64_dxsm.h
* https://github.com/imneme/pcg-cpp/commit/871d0494ee9c9a7b7c43f753e3d8ca47c26f8005
*/
DEF_URBG_SUBCLASS(pcg64dxsm, __uint128_t, uint64_t)

/// prepare the initial state
void
pcg64dxsm::init()
{}

pcg64dxsm::result_type
pcg64dxsm::next()
{
    // "cheap multiplier"
    constexpr uint64_t mul = pcg_const64[2];
    static_assert((mul & 1) != 0, "must be odd");
    constexpr __uint128_t inc = pcg_const128[1];
    static_assert((inc & 1) != 0, "must be odd");

    const auto old_s = s;
    s = s * mul + inc;

    auto hi = static_cast<uint64_t>(old_s >> 64);
    const auto lo = static_cast<uint64_t>(old_s | 1);

    hi ^= hi >> 32; // 64 / 2 == 32
    hi *= mul;
    hi ^= hi >> 48; // 3 * (64 / 4) == 48
    hi *= lo;

    return hi;
}
#endif
