// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// seeds and seed sequences used by the test suite
/**
* \file
* \author Steven Ward
*/

#pragma once

#include <array>
#include <cstdint>
#include <random>

template <std::uniform_random_bit_generator URBG>
constexpr auto
get_seed_bytes_zero()
{
    constexpr auto num_seed_bytes = sizeof(typename URBG::seed_bytes_type);
    return std::array<uint8_t, num_seed_bytes>{};
}

template <std::uniform_random_bit_generator URBG>
constexpr auto
get_seed_bytes_pattern()
{
    typename URBG::seed_bytes_type bytes{};
    bytes.fill(0xAA);
    return bytes;
}
