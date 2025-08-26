// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

#pragma once

#include "hxor.h"
#include "make_odd.h"
#include "simd-array.hpp"

#include <array>
#include <cstdint>
#include <cstring>
#include <err.h>
#include <immintrin.h>
#include <limits>
#include <unistd.h>

#if defined(__SHA__)

/// 16 rounds of SHA-1
inline __m128i
sha1_rnds4x4(__m128i a, __m128i b)
{
    a = _mm_sha1rnds4_epu32(a, b, 0);
    a = _mm_sha1rnds4_epu32(a, b, 1);
    a = _mm_sha1rnds4_epu32(a, b, 2);
    a = _mm_sha1rnds4_epu32(a, b, 3);
    return a;
}

/// A PRNG that uses SHA-1 instructions
class sha1_ctr_128
{
private:
    arr_m128i<2> s{};
    // s[0] is the state/counter
    // s[1] is the increment (must be odd)

public:
    using result_type = uint64_t;
    using seed_bytes_type = std::array<uint8_t, sizeof(s)>;

    sha1_ctr_128()
    {
        static_assert(sizeof(s) <= 256,
                      "getentropy will fail if more than 256 bytes are requested");
        reseed();
    }

    explicit sha1_ctr_128(const seed_bytes_type& bytes)
    {
        reseed();
        (void)std::memcpy(&s[0], bytes.data(), sizeof(s));
    }

    /// Assign random bytes to the data members via \c getentropy.
    /**
    * Every odd integer is coprime with every power of 2.
    * Therefore, \c inc shall be made odd.
    */
    void reseed()
    {
        if (getentropy(&s[0], sizeof(s)) < 0)
            err(EXIT_FAILURE, "getentropy");
        s[1] = mm_make_odd_epu64(s[1]);
    }

    static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }

    static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }

    result_type operator()() { return next(); }

    /// Get the next PRNG output via SHA-1 instructions.
    result_type next()
    {
        __m128i dst = s[0];
        s[0] = _mm_add_epi64(s[0], s[1]);
        dst = sha1_rnds4x4(dst, dst);
        return mm_hxor_epu64(dst);
    }
};

#else
#warning "__SHA__ not defined"
#endif
