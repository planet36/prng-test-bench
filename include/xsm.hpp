// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// xsm32, xsm64 PRNGs
/**
* \file
* \author Steven Ward
* \sa https://github.com/planet36/PractRand/blob/main/src/RNGs/xsm.cpp
*/

#pragma once

#include "abstract_urbg_class.hpp"
#include "xxhprimes.hpp"

#include <array>
#include <bit>
#include <cstdint>

// source:
// PractRand-pre0.95/src/RNGs/xsm.cpp
DEF_URBG_SUBCLASS(xsm32, SINGLE_ARG(std::array<uint32_t, 2>), uint32_t)

/// prepare the initial state
void xsm32::init()
{
}

xsm32::result_type xsm32::next()
{
    //const uint32_t K = 0xE99569B5;  // 11101001100101010110100110110101 473  205 - best value found so far
    //const uint32_t K = 0x299529B5;  // 00101001100101010010100110110101 470  202
    //const uint32_t K = 0x6595a395;  // 01100101100101011010001110010101 465  204 - original high quality version
    //const uint32_t K = 0xD251CF2D;  // 11010010010100011100111100101101 463  196
    //const uint32_t K = 0x08304E2D;  // 00001000001100000100111000101101 463  194
    //const uint32_t K = 0xCA566EB5;  // 11001010010101100110111010101101 458  201
    //const uint32_t K = 0xD250CE2D;  // 11010010010100001100111000101101 456  194
    //const uint32_t K = 0x21042185;  // 00100001000001000010000110000101 421  188 - now the middle quality version
    //const uint32_t K = 0x21052045;  // 00100001000001010010000001000101 373  161 - original reduced quality version
    //const uint32_t K = 0x01002045;  // 00000001000000000010000001000101 363  157 - lowest quality version tested

    static constexpr uint32_t M1 = 0xd251cf2d; // not prime (popcount = 17)
    static constexpr uint32_t M2 = 0x299529b5; // not prime (popcount = 15)
    static_assert(M1 & 1, "must be odd");
    static_assert(M2 & 1, "must be odd");
    static constexpr unsigned int R1 = 9;
    static constexpr unsigned int R2 = 19;
    static constexpr unsigned int R3 = 16;
    static constexpr unsigned int S1 = 16;

    result_type result = s[1] ^ std::rotl(s[1] + s[0], R1);
    result ^= std::rotl(result + xxh_prime32[1], R2);
    result *= M1;
    {
        // step forward
        const auto tmp = s[0] + xxh_prime32[1];
        s[0] += xxh_prime32[0];
        s[1] += tmp + ((s[0] < xxh_prime32[0]) ? 1 : 0);
    }
    result ^= std::rotl(result + s[1], R3);
    result *= M2;
    result ^= result >> S1;
    return result;
}

// source:
// PractRand-pre0.95/src/RNGs/xsm.cpp
DEF_URBG_SUBCLASS(xsm64, SINGLE_ARG(std::array<uint64_t, 2>), uint64_t)

/// prepare the initial state
void xsm64::init()
{
}

xsm64::result_type xsm64::next()
{
    static constexpr uint64_t M1 = 0xa3ec647659359acd; // not prime (popcount = 34)
    static_assert(M1 & 1, "must be odd");
    static constexpr unsigned int R1 = 16;
    static constexpr unsigned int R2 = 40;
    static constexpr unsigned int R3 = 32;
    static constexpr unsigned int S1 = 32;

    result_type result = s[1] ^ std::rotl(s[1] + s[0], R1);
    result ^= std::rotl(result + xxh_prime64[1], R2);
    result *= M1;
    {
        // step forward
        const auto tmp = s[0] + xxh_prime64[1];
        s[0] += xxh_prime64[0];
        s[1] += tmp + ((s[0] < xxh_prime64[0]) ? 1 : 0);
    }
    result ^= std::rotl(result + s[1], R3);
    result *= M1;
    result ^= result >> S1;
    return result;
}
