// SPDX-FileCopyrightText: David Blackman
// SPDX-FileCopyrightText: Sebastiano Vigna (vigna@acm.org)
// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: CC0-1.0

/// xoroshiro PRNG
/**
* \file
* \author Steven Ward
* \sa https://prng.di.unimi.it/
* \sa https://vigna.di.unimi.it/ftp/papers/ScrambledLinear.pdf
*/

#pragma once

#include "abstract_urbg_class.hpp"

#include <array>
#include <bit>
#include <cstdint>

/** This is xoroshiro64** 1.0, our 32-bit all-purpose, rock-solid, small-state
 * generator. It is extremely fast and it passes all tests we are aware of, but
 * its state space is not large enough for any parallel application.
 *
 * For generating just single-precision (i.e., 32-bit) floating-point numbers,
 * xoroshiro64* is even faster.
 *
 * The state must be seeded so that it is not everywhere zero.
 */
DEF_URBG_SUBCLASS(xoroshiro64starstar, SINGLE_ARG(std::array<uint32_t, 2>), uint32_t)

/// prepare the initial state
void
xoroshiro64starstar::init()
{
    if (s == state_type{})
    {
        for (int i = 0; i < std::ssize(s); ++i)
        {
            s[i] = i + 1;
        }
    }
}

xoroshiro64starstar::result_type
xoroshiro64starstar::next()
{
    const auto s0 = s[0];
    auto s1 = s[1];
    const auto result = std::rotl(s0 * 0x9E3779BB, 5) * 5;

    s1 ^= s0;
    s[0] = std::rotl(s0, 26) ^ s1 ^ (s1 << 9); // a, b
    s[1] = std::rotl(s1, 13); // c

    return result;
}

/** This is xoroshiro128++ 1.0, one of our all-purpose, rock-solid, small-state
 * generators. It is extremely (sub-ns) fast and it passes all tests we are
 * aware of, but its state space is large enough only for mild parallelism.
 *
 * For generating just floating-point numbers, xoroshiro128+ is even faster
 * (but it has a very mild bias, see notes in the comments).
 *
 * The state must be seeded so that it is not everywhere zero. If you have a
 * 64-bit seed, we suggest to seed a splitmix64 generator and use its output to
 * fill s.
 */
DEF_URBG_SUBCLASS(xoroshiro128plusplus, SINGLE_ARG(std::array<uint64_t, 2>), uint64_t)

/// prepare the initial state
/**
* The warm-up discards the zero-seed outputs that still have mostly zero bits (found
* empirically).
*/
void
xoroshiro128plusplus::init()
{
    if (s == state_type{})
    {
        for (int i = 0; i < std::ssize(s); ++i)
        {
            s[i] = i + 1;
        }
    }

    constexpr int num_warmup_discards = 3;
    for (int i = 0; i < num_warmup_discards; ++i)
    {
        (void)next();
    }
}

xoroshiro128plusplus::result_type
xoroshiro128plusplus::next()
{
    const auto s0 = s[0];
    auto s1 = s[1];
    const auto result = std::rotl(s0 + s1, 17) + s0;

    s1 ^= s0;
    s[0] = std::rotl(s0, 49) ^ s1 ^ (s1 << 21); // a, b
    s[1] = std::rotl(s1, 28); // c

    return result;
}

/** This is xoroshiro128** 1.0, one of our all-purpose, rock-solid, small-state
 * generators. It is extremely (sub-ns) fast and it passes all tests we are
 * aware of, but its state space is large enough only for mild parallelism.
 *
 * For generating just floating-point numbers, xoroshiro128+ is even faster
 * (but it has a very mild bias, see notes in the comments).
 *
 * The state must be seeded so that it is not everywhere zero. If you have a
 * 64-bit seed, we suggest to seed a splitmix64 generator and use its output to
 * fill s.
 */
DEF_URBG_SUBCLASS(xoroshiro128starstar, SINGLE_ARG(std::array<uint64_t, 2>), uint64_t)

/// prepare the initial state
/**
* The warm-up discards the zero-seed outputs that still have mostly zero bits (found
* empirically).
*/
void
xoroshiro128starstar::init()
{
    if (s == state_type{})
    {
        for (int i = 0; i < std::ssize(s); ++i)
        {
            s[i] = i + 1;
        }
    }

    constexpr int num_warmup_discards = 4;
    for (int i = 0; i < num_warmup_discards; ++i)
    {
        (void)next();
    }
}

xoroshiro128starstar::result_type
xoroshiro128starstar::next()
{
    const auto s0 = s[0];
    auto s1 = s[1];
    const auto result = std::rotl(s0 * 5, 7) * 9;

    s1 ^= s0;
    s[0] = std::rotl(s0, 24) ^ s1 ^ (s1 << 16); // a, b
    s[1] = std::rotl(s1, 37); // c

    return result;
}

/** This is xoroshiro1024++ 1.0, one of our all-purpose, rock-solid,
 * large-state generators. It is extremely fast and it passes all tests we are
 * aware of. Its state however is too large--in general, the xoshiro256 family
 * should be preferred.
 *
 * For generating just floating-point numbers, xoroshiro1024* is even faster
 * (but it has a very mild bias, see notes in the comments).
 *
 * The state must be seeded so that it is not everywhere zero. If you have a
 * 64-bit seed, we suggest to seed a splitmix64 generator and use its output to
 * fill s.
 */
struct xoroshiro1024plusplus final : public AbstractURBG<std::array<uint64_t, 16>, uint64_t>
{
protected:
    unsigned int p{};

    inline void init();

public:
    xoroshiro1024plusplus() { init(); }
    explicit xoroshiro1024plusplus(const state_type& new_s) : AbstractURBG(new_s) { init(); }
    explicit xoroshiro1024plusplus(const seed_bytes_type& bytes) : AbstractURBG(bytes) { init(); }
    inline result_type next() override;
};
static_assert(std::uniform_random_bit_generator<xoroshiro1024plusplus>);

/// prepare the initial state
/**
* The warm-up discards the zero-seed outputs that still have mostly zero bits (found
* empirically).
*/
void
xoroshiro1024plusplus::init()
{
    if (s == state_type{})
    {
        for (int i = 0; i < std::ssize(s); ++i)
        {
            s[i] = i + 1;
        }
    }

    constexpr int num_warmup_discards = 8;
    for (int i = 0; i < num_warmup_discards; ++i)
    {
        (void)next();
    }
}

xoroshiro1024plusplus::result_type
xoroshiro1024plusplus::next()
{
    const auto q = p;
    p = (p + 1) % s.size();
    const auto s0 = s[p];
    auto s15 = s[q];
    const auto result = std::rotl(s0 + s15, 23) + s15;

    s15 ^= s0;
    s[q] = std::rotl(s0, 25) ^ s15 ^ (s15 << 27);
    s[p] = std::rotl(s15, 36);

    return result;
}

/** This is xoroshiro1024** 1.0, one of our all-purpose, rock-solid,
 * large-state generators. It is extremely fast and it passes all tests we are
 * aware of. Its state however is too large--in general, the xoshiro256 family
 * should be preferred.
 *
 * For generating just floating-point numbers, xoroshiro1024* is even faster
 * (but it has a very mild bias, see notes in the comments).
 *
 * The state must be seeded so that it is not everywhere zero. If you have a
 * 64-bit seed, we suggest to seed a splitmix64 generator and use its output to
 * fill s.
 */
struct xoroshiro1024starstar final : public AbstractURBG<std::array<uint64_t, 16>, uint64_t>
{
protected:
    unsigned int p{};

    inline void init();

public:
    xoroshiro1024starstar() { init(); }
    explicit xoroshiro1024starstar(const state_type& new_s) : AbstractURBG(new_s) { init(); }
    explicit xoroshiro1024starstar(const seed_bytes_type& bytes) : AbstractURBG(bytes) { init(); }
    inline result_type next() override;
};
static_assert(std::uniform_random_bit_generator<xoroshiro1024starstar>);

/// prepare the initial state
/**
* The warm-up discards the zero-seed outputs that still have mostly zero bits (found
* empirically).
*/
void
xoroshiro1024starstar::init()
{
    if (s == state_type{})
    {
        for (int i = 0; i < std::ssize(s); ++i)
        {
            s[i] = i + 1;
        }
    }

    constexpr int num_warmup_discards = 22;
    for (int i = 0; i < num_warmup_discards; ++i)
    {
        (void)next();
    }
}

xoroshiro1024starstar::result_type
xoroshiro1024starstar::next()
{
    const auto q = p;
    p = (p + 1) % s.size();
    const auto s0 = s[p];
    auto s15 = s[q];
    const auto result = std::rotl(s0 * 5, 7) * 9;

    s15 ^= s0;
    s[q] = std::rotl(s0, 25) ^ s15 ^ (s15 << 27);
    s[p] = std::rotl(s15, 36);

    return result;
}
