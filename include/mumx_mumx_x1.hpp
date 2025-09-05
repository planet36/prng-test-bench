// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// mumx_mumx_x1 PRNG
/**
* \file
* \author Steven Ward
* \sa https://github.com/martinus/better-faster-stronger-mixer/blob/master/include/mixer/mumxmumxx1.h
*/

#pragma once

#include "abstract_urbg_class.hpp"
#include "mum.hpp"
#include "mum-primes.hpp"

#include <cstdint>

DEF_URBG_SUBCLASS(mumx_mumx_x1, uint64_t, uint64_t)

/// prepare the initial state
void mumx_mumx_x1::init()
{
}

mumx_mumx_x1::result_type mumx_mumx_x1::next()
{
    constexpr uint64_t inc = _mum_primes[0]; // inc=1 yields failures
    static_assert((inc & 1) != 0, "must be odd");

    constexpr uint64_t a = 0x2ca7aea0ebd71d49; // not prime (popcount = 34)
    constexpr uint64_t b = 0x9e49b5a3555f2295; // not prime (popcount = 33)

    auto x = s;
    s += inc; // (SDW)

    return mumx(mumx(x, a), x ^ b);
}
