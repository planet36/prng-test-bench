// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

#pragma once

#if defined(__AES__)

#include "hxor.h"
#include "fill_rand.hpp"
#include "make_odd.h"
#include "simd-array.hpp"
#include "simd-transpose.hpp"

#include <array>
#include <cstdint>
#include <cstring>
#include <err.h>
#include <immintrin.h>
#include <limits>
#include <random>

/// A PRNG that uses AES instructions
struct aes128_ctr_64
{
protected:
    arr_m128i<3> s{};
    // s[0] is the state/counter
    // s[1] is the increment (must be odd)
    // s[2] is the key

    /**
    * Every odd integer is coprime with every power of 2.
    * Therefore, \c inc shall be made odd.
    */
    void init()
    {
        s[1] = mm_make_odd_epu64(s[1]);
    }

public:
    using result_type = uint64_t;
    using seed_bytes_type = std::array<uint8_t, sizeof(s)>;

    aes128_ctr_64()
    {
        fill_rand(s);
        init();
    }

    explicit aes128_ctr_64(const seed_bytes_type& bytes)
    {
        (void)std::memcpy(std::data(s), std::data(bytes), sizeof(s));
        init();
    }

    static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }

    static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }

    result_type operator()() { return next(); }

    result_type next()
    {
        // must do at least 2 rounds of AES
        constexpr unsigned int Nr = 2;
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
