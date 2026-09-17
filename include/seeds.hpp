// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// Seed bytes used by the test suite
/**
* \file
* \author Steven Ward
*/

#pragma once

#include <random>

template <std::uniform_random_bit_generator URBG>
constexpr auto
get_seed_bytes_zero()
{
    return typename URBG::seed_bytes_type{};
}

template <std::uniform_random_bit_generator URBG>
constexpr auto
get_seed_bytes_pattern()
{
    typename URBG::seed_bytes_type bytes{};
    bytes.fill(0xAA);
    return bytes;
}
