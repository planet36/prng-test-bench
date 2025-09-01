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
#include "hxor.h"
#include "make_odd.h"
#include "scaled-const.h"
#include "simd-array.hpp"
#include "simd-transpose.hpp"

#include <array>
#include <cstdint>
#include <immintrin.h>
#include <random>

struct aes128_ctr_64 final : public AbstractURBG<arr_m128i<3>, uint64_t>
{
protected:
    // s[0] is the state/counter
    // s[1] is the increment (must be odd)
    // s[2] is the key

    /// prepare the initial state
    /**
    * Every odd integer is coprime with every power of 2.
    * Therefore, \c inc shall be made odd.
    */
    void init()
    {
        s[1] = mm_make_odd_epu64(s[1]);

        uint64_t inc_0 = _mm_extract_epi64(s[1], 0);
        uint64_t inc_1 = _mm_extract_epi64(s[1], 1);

        // both increment values must be unique (and odd)
        if (inc_0 == inc_1)
        {
            // make unique
            inc_0 ^= FLOOR_SCALED_FRAC_SQRT_2;
            inc_1 ^= FLOOR_SCALED_FRAC_SQRT_3;

            // make odd
            inc_0 |= 1;
            inc_1 |= 1;

            // most significant elem first
            s[1] = _mm_set_epi64x(inc_1, inc_0);
        }
    }

public:

    aes128_ctr_64()
    {
        init();
    }

    explicit aes128_ctr_64(const state_type& new_s) : AbstractURBG(new_s)
    {
        init();
    }

    explicit aes128_ctr_64(const seed_bytes_type& bytes) : AbstractURBG(bytes)
    {
        init();
    }

    result_type next() override
    {
        // must do at least 2 rounds of AES
        constexpr unsigned int Nr = 2;
        static_assert(Nr >= 2);

        __m128i dst = s[0];
        s[0] = _mm_add_epi64(s[0], s[1]);

        for (unsigned int r = 0; r < Nr; ++r)
        {
            dst = _mm_aesenc_si128(dst, s[2]);
        }

        return mm_hxor_epu64(dst);
    }
};

static_assert(std::uniform_random_bit_generator<aes128_ctr_64>);

#else

#warning "__AES__ not defined"

#endif
