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
    typename URBG::seed_bytes_type bytes{};
    return bytes;
}

template <std::uniform_random_bit_generator URBG>
constexpr auto
get_seed_bytes_pattern()
{
    typename URBG::seed_bytes_type bytes{};
    bytes.fill(0xAA);
    return bytes;
}
