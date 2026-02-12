// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// A PRNG that uses AES instructions
/**
* \file
* \author Steven Ward
*/

#pragma once

#if defined(__AES__)

#include "abstract_urbg_class.hpp"
#include "make_odd.h"
#include "mm_cast.hpp"
#include "mm_equal.h"
#include "scaled-const.h"
#include "simd-array.hpp"

#include <cstdint>
#include <immintrin.h>
#include <random>

namespace aes128_ctr
{

using state_type = arr_m128i<3>;

// s[0] is the state/counter
// s[1] is the increment (must be odd)
// s[2] is the key

/// prepare the initial state
/**
* Every odd integer is coprime with every power of 2.
* Therefore, \c inc shall be made odd.
*/
void
prepare_initial_state(state_type& s)
{
    s[1] = mm_make_odd_epu64(s[1]);

    // both increment values must be unique (and odd)
    if (mm_all_equal_epi64(s[1]))
    {
        // most significant elem first
        const auto mask_inc =
            _mm_set_epi64x(FLOOR_SCALED_FRAC_SQRT_3, FLOOR_SCALED_FRAC_SQRT_2);

        // make unique
        s[1] = _mm_xor_si128(s[1], mask_inc);

        s[1] = mm_make_odd_epu64(s[1]);
    }

    // the key must have more than 1 unique byte
    if (mm_all_equal_epi8(s[2]))
    {
        // most significant elem first
        const auto mask_key =
            _mm_set_epi64x(FLOOR_SCALED_FRAC_SQRT_7, FLOOR_SCALED_FRAC_SQRT_5);

        s[2] = _mm_xor_si128(s[2], mask_key);
    }
}

/// advance the state of the PRNG, and generate a pseudo-random value
__m128i
advance_state_and_generate_value(state_type& s)
{
    // must do at least 3 rounds of AES
    constexpr unsigned int Nr = 3;
    static_assert(Nr >= 3);

    __m128i dst = s[0];
    s[0] = _mm_add_epi64(s[0], s[1]);

    for (unsigned int r = 0; r < Nr; ++r)
    {
        dst = _mm_aesenc_si128(dst, s[2]);
    }

    return dst;
}

}

DEF_URBG_SUBCLASS(aes128_ctr_64, aes128_ctr::state_type, uint64_t)

/// prepare the initial state
void
aes128_ctr_64::init()
{
    aes128_ctr::prepare_initial_state(s);
}

aes128_ctr_64::result_type
aes128_ctr_64::next()
{
    return uint64_from_m128i(aes128_ctr::advance_state_and_generate_value(s));
}

#if !defined(__SIZEOF_INT128__)
#error "__SIZEOF_INT128__ not defined"
#endif

DEF_URBG_SUBCLASS(aes128_ctr_128, aes128_ctr::state_type, __uint128_t)

/// prepare the initial state
void
aes128_ctr_128::init()
{
    aes128_ctr::prepare_initial_state(s);
}

aes128_ctr_128::result_type
aes128_ctr_128::next()
{
    return uint128_from_m128i(aes128_ctr::advance_state_and_generate_value(s));
}

#else

#warning "__AES__ not defined"

#endif
