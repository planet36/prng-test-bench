// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// LXM PRNG
/**
\file
\author Steven Ward
\sa https://vigna.di.unimi.it/ftp/papers/LXM.pdf
\sa https://github.com/s417-lama/lxm_random/blob/master/include/lxm_random/lxm_random.hpp
\sa https://issues.apache.org/jira/browse/RNG-168
*/

#pragma once

#include "abstract_urbg_class.hpp"
#include "scaled-const.hpp"

#include <bit>
#include <cstdint>

namespace
{

// MurmurHash3
// https://github.com/aappleby/smhasher/wiki/MurmurHash3
inline constexpr uint64_t
mix_murmur(uint64_t h)
{
    h ^= h >> 33;
    h *= 0xff51afd7ed558ccd;
    h ^= h >> 33;
    h *= 0xc4ceb9fe1a85ec53;
    h ^= h >> 33;
    return h;
}

// David Stafford's mixing function
// http://zimbry.blogspot.com/2011/09/better-bit-mixing-improving-on.html
inline constexpr uint64_t
mix_stafford13(uint64_t h)
{
    h ^= h >> 30;
    h *= 0xbf58476d1ce4e5b9;
    h ^= h >> 27;
    h *= 0x94d049bb133111eb;
    h ^= h >> 31;
    return h;
}

// Doug Lea's mixing function
inline constexpr uint64_t
mix_lea(uint64_t h)
{
    h ^= h >> 32;
    h *= 0xdaba0b6eb09322e3;
    h ^= h >> 32;
    h *= 0xdaba0b6eb09322e3;
    h ^= h >> 32;
    return h;
}

}

struct lxm final : public AbstractURBG<std::array<uint64_t, 4>, uint64_t>
{
protected:

    /// prepare the initial state
    /**
    * \sa https://github.com/s417-lama/lxm_random/blob/master/include/lxm_random/lxm_random.hpp#L169-L181
    */
    void init()
    {
        // must be odd
        s[0] |= 1;

        // both s[2] and s[3] cannot be 0
        if ((s[2] == 0) && (s[3] == 0))
        {
            const uint64_t v = s[1] + GoldenRatio_64;
            s[2] = mix_stafford13(v);
            s[3] = mix_stafford13(v + GoldenRatio_64);
        }
    }

public:

    lxm()
    {
        init();
    }

    explicit lxm(const state_type& new_s) : AbstractURBG(new_s)
    {
        init();
    }

    explicit lxm(const seed_bytes_type& bytes) : AbstractURBG(bytes)
    {
        init();
    }

    /**
    * \sa https://github.com/s417-lama/lxm_random/blob/master/include/lxm_random/lxm_random.hpp#L100-L116
    */
    result_type next() override
    {
        static constexpr uint64_t M = 0xd1342543de82ef95; // not prime (popcount = 32)
        static_assert((M & 1) != 0, "must be odd");

        static constexpr unsigned int R1 = 24;
        static constexpr unsigned int R2 = 37;
        static constexpr unsigned int S1 = 16;

        // Mixing function (lea64)
        const uint64_t z = mix_lea(s[1] + s[2]);

        // Update the LCG subgenerator
        s[1] = M * s[1] + s[0];

        // Update the XBG subgenerator (xoroshiro128v1_0)
        uint64_t q0 = s[2];
        uint64_t q1 = s[3];
        q1 ^= q0;
        q0 = std::rotl(q0, R1);
        q0 ^= q1 ^ (q1 << S1);
        q1 = std::rotl(q1, R2);
        s[2] = q0;
        s[3] = q1;

        return z;
    }
};
