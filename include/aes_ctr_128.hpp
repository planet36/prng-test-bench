// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

#pragma once

#include "make_odd.h"
#include "simd-array.hpp"
#include "simd-transpose.hpp"
#include "simd-union.hpp"

#include <array>
#include <concepts>
#include <cstdint>
#include <cstring>
#include <err.h>
#include <immintrin.h>
#include <limits>
#include <random>
#include <type_traits>
#include <unistd.h>

#if defined(__AES__)

// Must use -std=gnu++XX for these static_asserts to succeed.
// https://stackoverflow.com/a/71710062/1892784

static_assert(std::integral<__int128_t>);
static_assert(std::signed_integral<__int128_t>);

static_assert(std::integral<__uint128_t>);
static_assert(std::unsigned_integral<__uint128_t>);

/// A PRNG that uses AES instructions
template <unsigned int Nk, unsigned int Nr>
class aes_ctr_128_prng
{
    static_assert(Nk >= 1);
    static_assert(Nr >= 1);
    static_assert(Nk * Nr >= 2, "must do at least 2 rounds of AES");

private:
    arr_m128i<Nk> keys{};
    __m128i ctr{}; ///< The state/counter
    __m128i inc{}; ///< The increment (must be odd)

public:
    using result_type = __uint128_t;
    using seed_bytes_type = std::array<uint8_t, sizeof(ctr)>;

    aes_ctr_128_prng()
    {
        static_assert(sizeof(*this) <= 256,
                      "getentropy will fail if more than 256 bytes are requested");
        reseed();
    }

    explicit aes_ctr_128_prng(const seed_bytes_type& bytes)
    {
        reseed();
        (void)std::memcpy(&ctr, bytes.data(), sizeof(ctr));
    }

    /// Assign random bytes to the data members via \c getentropy.
    /**
    * Every odd integer is coprime with every power of 2.
    * Therefore, \c inc shall be made odd.
    */
    void reseed()
    {
        if (getentropy(this, sizeof(*this)) < 0)
            err(EXIT_FAILURE, "getentropy");
        inc = mm_make_odd_epu64(inc);
    }

    static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }

    static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }

    result_type operator()() { return next(); }

    /// Get the next PRNG output via AES encryption or decryption.
    result_type next()
    {
        __m128i dst = ctr;
        ctr = _mm_add_epi64(ctr, inc);

        for (unsigned int r = 0; r < Nr; ++r)
        {
            for (unsigned int k = 0; k < Nk; ++k)
            {
                dst = _mm_aesenc_si128(dst, keys[k]);
            }
        }

        return simd128i{.xmm = dst}.u128[0];
    }
};

using aes_ctr_128_k1_r2 = aes_ctr_128_prng<1, 2>;
using aes_ctr_128_k1_r3 = aes_ctr_128_prng<1, 3>;
using aes_ctr_128_k2_r1 = aes_ctr_128_prng<2, 1>;
using aes_ctr_128_k3_r1 = aes_ctr_128_prng<3, 1>;

static_assert(std::uniform_random_bit_generator<aes_ctr_128_k1_r2>);
static_assert(std::uniform_random_bit_generator<aes_ctr_128_k1_r3>);
static_assert(std::uniform_random_bit_generator<aes_ctr_128_k2_r1>);
static_assert(std::uniform_random_bit_generator<aes_ctr_128_k3_r1>);

#else
#warning "__AES__ not defined"
#endif
