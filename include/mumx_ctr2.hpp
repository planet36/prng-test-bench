// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// MUMX CTRx2 PRNGs
/**
* \file
* \author Steven Ward
*/

#pragma once

#include "mum.hpp"
#include "urbg_base_class.hpp"
#include "wyprimes.hpp"

#include <array>
#include <cstdint>

DEF_URBG_SUBCLASS(mumx_ctr2, SINGLE_ARG(std::array<uint64_t, 2>), uint64_t)

/// Prepare the initial state
void
mumx_ctr2::init()
{}

mumx_ctr2::result_type
mumx_ctr2::next()
{
    constexpr std::array<uint64_t, 2> inc{wyprimes::_wyp[0], wyprimes::_wyp[1]};

    s[0] += inc[0];
    s[1] += inc[1];
    return mumx(s[0], s[1]);
}
