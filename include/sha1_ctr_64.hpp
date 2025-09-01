// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// A PRNG that uses SHA-1 instructions
/**
* \file
* \author Steven Ward
*/

#pragma once

#if defined(__SHA__)

#include "abstract_urbg_class.hpp"
#include "hxor.h"
#include "make_odd.h"
#include "scaled-const.h"
#include "simd-array.hpp"

#include <array>
#include <cstdint>
#include <immintrin.h>
#include <random>

/// 4*4 rounds of SHA-1
static __m128i
sha1_rnds4x4(__m128i a, __m128i b)
{
    a = _mm_sha1rnds4_epu32(a, b, 0);
    b = _mm_sha1rnds4_epu32(a, b, 1);
    a = _mm_sha1rnds4_epu32(a, b, 2);
    b = _mm_sha1rnds4_epu32(a, b, 3);
    return b;
}

struct sha1_ctr_64 final : public AbstractURBG<arr_m128i<2>, uint64_t>
{
protected:
    // s[0] is the state/counter
    // s[1] is the increment (must be odd)

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

    sha1_ctr_64()
    {
        init();
    }

    explicit sha1_ctr_64(const state_type& new_s) : AbstractURBG(new_s)
    {
        init();
    }

    explicit sha1_ctr_64(const seed_bytes_type& bytes) : AbstractURBG(bytes)
    {
        init();
    }

    result_type next() override
    {
        __m128i dst = s[0];
        s[0] = _mm_add_epi64(s[0], s[1]);
        dst = sha1_rnds4x4(dst, dst);
        return mm_hxor_epu64(dst);
    }
};

static_assert(std::uniform_random_bit_generator<sha1_ctr_64>);

#else

#warning "__SHA__ not defined"

#endif
