// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// wyrand PRNG
/**
* \file
* \author Steven Ward
* \sa https://github.com/lemire/testingRNG/blob/master/source/wyrand.h
* \sa https://github.com/wangyi-fudan/wyhash/blob/master/wyhash.h
* \sa https://github.com/rurban/smhasher/blob/master/wyhash.h
*/

#pragma once

#include "def_urbg_class.hpp"
#include "mum.hpp"
#include "wyprimes.hpp"

#include <cstdint>

DEF_URBG_CLASS(wyrand, uint64_t, uint64_t)
{
    s += _wyp[0];
    return mumx(s, s ^ _wyp[1]);
}
