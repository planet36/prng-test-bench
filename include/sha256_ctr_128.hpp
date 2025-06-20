// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

#pragma once

#include "make_odd.h"
#include "simd-union.hpp"

#include <array>
#include <concepts>
#include <cstdint>
#include <cstring>
#include <err.h>
#include <immintrin.h>
#include <limits>
#include <type_traits>
#include <unistd.h>

#if defined(__SHA__)

// Adapted from
// https://git.savannah.gnu.org/cgit/gnulib.git/tree/lib/sha256.c#n267

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
static const std::array<__m128i, 32> sha256_round_constants{
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
#pragma GCC diagnostic pop

/// 16 rounds of SHA-256
inline __m128i
sha256_rnds2x8(__m128i a, __m128i b)
{
    b = _mm_sha256rnds2_epu32(a, b, sha256_round_constants[0]);
    b = _mm_sha256rnds2_epu32(a, b, sha256_round_constants[1]);
    b = _mm_sha256rnds2_epu32(a, b, sha256_round_constants[2]);
    b = _mm_sha256rnds2_epu32(a, b, sha256_round_constants[3]);
    b = _mm_sha256rnds2_epu32(a, b, sha256_round_constants[4]);
    b = _mm_sha256rnds2_epu32(a, b, sha256_round_constants[5]);
    b = _mm_sha256rnds2_epu32(a, b, sha256_round_constants[6]);
    b = _mm_sha256rnds2_epu32(a, b, sha256_round_constants[7]);
    return b;
}

// Must use -std=gnu++XX for these static_asserts to succeed.
// https://stackoverflow.com/a/71710062/1892784

static_assert(std::integral<__int128_t>);
static_assert(std::signed_integral<__int128_t>);

static_assert(std::integral<__uint128_t>);
static_assert(std::unsigned_integral<__uint128_t>);

/// A PRNG that uses SHA-256 instructions
class sha256_ctr_128
{
private:
    __m128i ctr{}; ///< The state/counter
    __m128i inc{}; ///< The increment (must be odd)

public:
    using result_type = __uint128_t;
    using seed_bytes_type = std::array<uint8_t, sizeof(ctr)>;

    sha256_ctr_128()
    {
        static_assert(sizeof(*this) <= 256,
                      "getentropy will fail if more than 256 bytes are requested");
        reseed();
    }

    explicit sha256_ctr_128(const seed_bytes_type& bytes)
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

    /// Get the next PRNG output via SHA-256 instructions.
    result_type next()
    {
        __m128i dst = ctr;
        ctr = _mm_add_epi64(ctr, inc);
        dst = sha256_rnds2x8(dst, dst);
        return simd128i{.xmm = dst}.u128[0];
    }
};

#else
#warning "__SHA__ not defined"
#endif
