// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

#pragma once

#if defined(__AES__)

#include "hxor.h"
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
#include <unistd.h>

/// A PRNG that uses AES instructions
template <unsigned int Nk, unsigned int Nr>
struct aes128_ctr_64_prng
{
    static_assert(Nk >= 1);
    static_assert(Nr >= 1);
    static_assert(Nk * Nr >= 2, "must do at least 2 rounds of AES");

protected:
    arr_m128i<2+Nk> s{};
    // s[0] is the state/counter
    // s[1] is the increment (must be odd)
    // s[2...] are the keys

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

    /// Assign random bytes to the data members via \c getentropy.
    aes128_ctr_64_prng()
    {
        static_assert(sizeof(s) <= 256,
                      "getentropy will fail if more than 256 bytes are requested");

        if (getentropy(std::data(s), sizeof(s)) < 0)
            err(EXIT_FAILURE, "getentropy");

        init();
    }

    explicit aes128_ctr_64_prng(const seed_bytes_type& bytes)
    {
        (void)std::memcpy(std::data(s), std::data(bytes), sizeof(s));
        init();
    }

    static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }

    static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }

    result_type operator()() { return next(); }

    /// Get the next PRNG output via AES encryption or decryption.
    result_type next()
    {
        __m128i dst = s[0];
        s[0] = _mm_add_epi64(s[0], s[1]);

        for (unsigned int r = 0; r < Nr; ++r)
        {
            for (unsigned int k = 0; k < Nk; ++k)
            {
                dst = _mm_aesenc_si128(dst, s[2+k]);
            }
        }

        return mm_hxor_epu64(dst);
    }
};

using aes128_ctr_64 = aes128_ctr_64_prng<1, 2>;

static_assert(std::uniform_random_bit_generator<aes128_ctr_64>);

#else

#warning "__AES__ not defined"

#endif
