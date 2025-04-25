// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// mumx_mumx_x2 PRNG
/**
\file
\author Steven Ward
\sa https://github.com/martinus/better-faster-stronger-mixer/blob/master/include/mixer/mumxmumxx2.h
*/

#pragma once

#include "def_urbg_class.hpp"
#include "mum.hpp"

#include <cstdint>

DEF_URBG_CLASS(mumx_mumx_x2, uint64_t, uint64_t)
{
    static constexpr uint64_t inc = _mum_primes[0];
    static_assert((inc & 1) != 0, "must be odd");

    static constexpr uint64_t a = 0x2ca7aea0ebd71d49; // not prime (popcount = 34)
    static constexpr uint64_t b = 0x9e49b5a3555f2295; // not prime (popcount = 33)

    auto x = s;
    s += inc; // (SDW)

    return mumx(mumx(x, a), mumx(x, b));
}
