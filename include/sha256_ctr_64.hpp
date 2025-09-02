// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// A PRNG that uses SHA-256 instructions
/**
* \file
* \author Steven Ward
*/

#pragma once

#if defined(__SHA__)

#include "abstract_urbg_class.hpp"
#include "hxor.h"
#include "make_odd.h"
#include "mm_equal.h"
#include "scaled-const.h"
#include "simd-array.hpp"

#include <array>
#include <cstdint>
#include <immintrin.h>
#include <random>

/**
* Adapted from
* https://git.savannah.gnu.org/cgit/gnulib.git/tree/lib/sha256.c#n267
*/
static const arr_m128i<32> sha256_round_constants {
    _mm_setr_epi32(0x428a2f98, 0x71374491, 0, 0), _mm_setr_epi32(0xb5c0fbcf, 0xe9b5dba5, 0, 0),
    _mm_setr_epi32(0x3956c25b, 0x59f111f1, 0, 0), _mm_setr_epi32(0x923f82a4, 0xab1c5ed5, 0, 0),
    _mm_setr_epi32(0xd807aa98, 0x12835b01, 0, 0), _mm_setr_epi32(0x243185be, 0x550c7dc3, 0, 0),
    _mm_setr_epi32(0x72be5d74, 0x80deb1fe, 0, 0), _mm_setr_epi32(0x9bdc06a7, 0xc19bf174, 0, 0),
    _mm_setr_epi32(0xe49b69c1, 0xefbe4786, 0, 0), _mm_setr_epi32(0x0fc19dc6, 0x240ca1cc, 0, 0),
    _mm_setr_epi32(0x2de92c6f, 0x4a7484aa, 0, 0), _mm_setr_epi32(0x5cb0a9dc, 0x76f988da, 0, 0),
    _mm_setr_epi32(0x983e5152, 0xa831c66d, 0, 0), _mm_setr_epi32(0xb00327c8, 0xbf597fc7, 0, 0),
    _mm_setr_epi32(0xc6e00bf3, 0xd5a79147, 0, 0), _mm_setr_epi32(0x06ca6351, 0x14292967, 0, 0),
    _mm_setr_epi32(0x27b70a85, 0x2e1b2138, 0, 0), _mm_setr_epi32(0x4d2c6dfc, 0x53380d13, 0, 0),
    _mm_setr_epi32(0x650a7354, 0x766a0abb, 0, 0), _mm_setr_epi32(0x81c2c92e, 0x92722c85, 0, 0),
    _mm_setr_epi32(0xa2bfe8a1, 0xa81a664b, 0, 0), _mm_setr_epi32(0xc24b8b70, 0xc76c51a3, 0, 0),
    _mm_setr_epi32(0xd192e819, 0xd6990624, 0, 0), _mm_setr_epi32(0xf40e3585, 0x106aa070, 0, 0),
    _mm_setr_epi32(0x19a4c116, 0x1e376c08, 0, 0), _mm_setr_epi32(0x2748774c, 0x34b0bcb5, 0, 0),
    _mm_setr_epi32(0x391c0cb3, 0x4ed8aa4a, 0, 0), _mm_setr_epi32(0x5b9cca4f, 0x682e6ff3, 0, 0),
    _mm_setr_epi32(0x748f82ee, 0x78a5636f, 0, 0), _mm_setr_epi32(0x84c87814, 0x8cc70208, 0, 0),
    _mm_setr_epi32(0x90befffa, 0xa4506ceb, 0, 0), _mm_setr_epi32(0xbef9a3f7, 0xc67178f2, 0, 0),
};

/// 2*4 rounds of SHA-256
static __m128i
sha256_rnds2x4(__m128i a, __m128i b)
{
    a = _mm_sha256rnds2_epu32(a, b, sha256_round_constants[0]);
    b = _mm_sha256rnds2_epu32(a, b, sha256_round_constants[1]);
    a = _mm_sha256rnds2_epu32(a, b, sha256_round_constants[2]);
    b = _mm_sha256rnds2_epu32(a, b, sha256_round_constants[3]);
    return b;
}

struct sha256_ctr_64 final : public AbstractURBG<arr_m128i<2>, uint64_t>
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

        // both increment values must be unique (and odd)
        if (mm_all_equal_epi64(s[1]))
        {
            // most significant elem first
            const auto mask_inc = _mm_set_epi64x(FLOOR_SCALED_FRAC_SQRT_3,
                                                 FLOOR_SCALED_FRAC_SQRT_2);

            // make unique
            s[1] = _mm_xor_si128(s[1], mask_inc);

            s[1] = mm_make_odd_epu64(s[1]);
        }
    }

public:

    sha256_ctr_64()
    {
        init();
    }

    explicit sha256_ctr_64(const state_type& new_s) : AbstractURBG(new_s)
    {
        init();
    }

    explicit sha256_ctr_64(const seed_bytes_type& bytes) : AbstractURBG(bytes)
    {
        init();
    }

    result_type next() override
    {
        __m128i dst = s[0];
        s[0] = _mm_add_epi64(s[0], s[1]);
        dst = sha256_rnds2x4(dst, dst);
        return mm_hxor_epu64(dst);
    }
};

static_assert(std::uniform_random_bit_generator<sha256_ctr_64>);

#else

#warning "__SHA__ not defined"

#endif
