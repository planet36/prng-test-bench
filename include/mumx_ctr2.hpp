// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// MUMX CTRx2 PRNGs
/**
* \file
* \author Steven Ward
*/

#pragma once

#include "abstract_urbg_class.hpp"
#include "mum.hpp"
#include "xxhprimes.hpp"

#include <array>
#include <cstdint>

DEF_URBG_SUBCLASS(mumx_ctr2, SINGLE_ARG(std::array<uint64_t, 2>), uint64_t)

/// prepare the initial state
void
mumx_ctr2::init()
{}

mumx_ctr2::result_type
mumx_ctr2::next()
{
    constexpr std::array<uint64_t, 2> inc{XXH_PRIME64_1, XXH_PRIME64_2};

    s[0] += inc[0];
    s[1] += inc[1];
    return mumx(s[0], s[1]);
}
