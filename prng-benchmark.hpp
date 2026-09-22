// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// Helpers shared by the PRNG benchmark programs
/**
* \file
* \author Steven Ward
*/

#pragma once

#include "seed_seq.hpp"
#include "urbg_base_class.hpp"

#include <random>

/// Construct a randomly seeded \c std engine
template <typename URBG>
requires std::uniform_random_bit_generator<URBG> && (!my_urbg<URBG>)
[[nodiscard]] URBG
make_random_seeded()
{
    return random_device_seeded<URBG>();
}

/// Construct a randomly seeded PRNG of mine
/**
* The default constructor fills the state from \c arc4random_buf.
*/
template <my_urbg URBG>
[[nodiscard]] URBG
make_random_seeded()
{
    return URBG{};
}
