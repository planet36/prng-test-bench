// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// Seed bytes used by the test suite
/**
* \file
* \author Steven Ward
*/

#pragma once

#include <cstdint>
#include <random>

/// The value of every byte in a pattern seed
inline constexpr uint8_t seed_pattern_byte{0xAA};

/// Return seed bytes for \a URBG that are all 0x00
template <std::uniform_random_bit_generator URBG>
constexpr auto
get_seed_bytes_zero()
{
    return typename URBG::seed_bytes_type{};
}

/// Return seed bytes for \a URBG that are all \c seed_pattern_byte
template <std::uniform_random_bit_generator URBG>
constexpr auto
get_seed_bytes_pattern()
{
    typename URBG::seed_bytes_type bytes{};
    bytes.fill(seed_pattern_byte);
    return bytes;
}
