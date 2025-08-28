// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// mumx_mumx_rrxx_1 PRNG
/**
* \file
* \author Steven Ward
* \sa https://github.com/martinus/better-faster-stronger-mixer/blob/master/include/mixer/mumx_mumx_rrxx_1.h
*/

#pragma once

#include "def_urbg_class.hpp"
#include "mum.hpp"
#include "mum-primes.hpp"

#include <bit>
#include <cstdint>

DEF_URBG_CLASS(mumx_mumx_rrxx_1, uint64_t, uint64_t)
{
    static constexpr uint64_t inc = _mum_primes[0];
    static_assert((inc & 1) != 0, "must be odd");

    static constexpr uint64_t a = 0xd14fff8ace476a59; // not prime (popcount = 37)
    static_assert(a & 1, "must be odd");
    static constexpr unsigned int R1 = 25;
    static constexpr unsigned int R2 = 47;

    // mumx(x,a) is not bijective, it only has ~64% coverage. Using data parallel execution, in the
    // meantime we calculate the rrx (rotate & rotate & xor & xor):
    //
    // x ^ std::rotr(x, 25) ^ std::rotr(x ^ b, 47)
    //
    // So we get two differently mixed results, both are the input to another final mumx operation
    // to mix them to together.
    //
    // See http://mostlymangling.blogspot.com/2020/01/nasam-not-another-strange-acronym-mixer.html

    auto x = s;
    s += inc; // (SDW)

    return mumx(mumx(x, a), x ^ std::rotr(x, R1) ^ std::rotr(x ^ a, R2));
}
