// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// rrxmrrxmsx_0 PRNG
/**
* \file
* \author Steven Ward
* \sa https://mostlymangling.blogspot.com/2019/01/better-stronger-mixer-and-test-procedure.html
*/

#pragma once

#include "abstract_urbg_class.hpp"

#include <bit>
#include <cstdint>

// New mixer, "rrxmrrxmsx_0", failing one subtest of RR-64-40-TF2-0.94.
// With a unit counter starting at 0, it has passed 128 TB of
// PractRand 0.94 -tf 2 without anomalies found past 2 TB.
DEF_URBG_SUBCLASS(rrxmrrxmsx_0, uint64_t, uint64_t)

/// prepare the initial state
void rrxmrrxmsx_0::init()
{
}

rrxmrrxmsx_0::result_type rrxmrrxmsx_0::next()
{
    constexpr uint64_t M1 = 0xa24baed4963ee407; // not prime (popcount = 32)
    constexpr uint64_t M2 = 0x9fb21c651e98df25; // prime (popcount = 34)
    static_assert(M1 & 1, "must be odd");
    static_assert(M2 & 1, "must be odd");
    constexpr unsigned int R1 = 25;
    constexpr unsigned int R2 = 50;
    constexpr unsigned int R3 = 24;
    constexpr unsigned int R4 = 49;
    constexpr unsigned int S1 = 28;

    auto x = s++; // (SDW)

    x ^= std::rotr(x, R1) ^ std::rotr(x, R2);
    x *= M1;
    x ^= std::rotr(x, R3) ^ std::rotr(x, R4);
    x *= M2;
    x ^= x >> S1;
    return x;
}
