// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// PRNGs from https://github.com/tommyettinger/sarong
/**
* \file
* \author Steven Ward
* \sa https://github.com/tommyettinger/sarong/tree/master/src/main/java/sarong
*/

#pragma once

#include "abstract_urbg_class.hpp"
#include "xxhprimes.hpp"

#include <array>
#include <bit>
#include <cstdint>

// https://github.com/tommyettinger/sarong/blob/master/src/main/java/sarong/BellRNG.java
DEF_URBG_SUBCLASS(bell, SINGLE_ARG(std::array<uint64_t, 2>), uint64_t)

/// prepare the initial state
void bell::init()
{
}

bell::result_type bell::next()
{
    // XXX: s[1] is never influenced by s[0]
    s[1] = ((s[1] >> 1) ^ (-(s[1] & 1) & 0xd800000000000000));
    s[0] += s[1] + 0x9e3779b97f4a7c15;
    const auto x = (s[0] ^ (s[0] >> 30)) * 0x369dea0f31a53f85;
    return x ^ (x >> 28);
}

// https://github.com/tommyettinger/sarong/blob/master/src/main/java/sarong/BrightRNG.java
DEF_URBG_SUBCLASS(bright, uint64_t, uint64_t)

/// prepare the initial state
void bright::init()
{
}

bright::result_type bright::next()
{
    s += 0xeb44accab455d165;
    const auto x = (s ^ (s >> 29) ^ (s >> 43) ^ (s << 7) ^ (s << 53)) * 0xdb4f0b9175ae2165;
    return x ^ (x >> 26);
}

// https://github.com/tommyettinger/sarong/blob/master/src/main/java/sarong/DirkRNG.java
DEF_URBG_SUBCLASS(dirk, uint64_t, uint64_t)

/// prepare the initial state
void dirk::init()
{
}

dirk::result_type dirk::next()
{
    auto x = (++s ^ 0x9e3779b97f4a7c15) * 0xd1b54a32d192ed03;
    x = (x ^ (x >> 28) ^ 0xdb4f0b9175ae2165) * 0xe60e2b722b53aeeb;
    return x ^ std::rotl(x, 19) ^ std::rotr(x, 18);
}

// https://github.com/tommyettinger/sarong/blob/master/src/main/java/sarong/LinnormRNG.java
DEF_URBG_SUBCLASS(linnorm, uint64_t, uint64_t)

/// prepare the initial state
void linnorm::init()
{
}

linnorm::result_type linnorm::next()
{
    s = s * 0x369dea0f31a53f85 + 1;
    const auto x = (s ^ (s >> 23) ^ (s >> 47)) * 0xaef17502108ef2d9;
    return x ^ (x >> 25);
}

// https://github.com/tommyettinger/sarong/blob/master/src/main/java/sarong/MizuchiRNG.java
DEF_URBG_SUBCLASS(mizuchi, uint64_t, uint64_t)

/// prepare the initial state
void mizuchi::init()
{
}

mizuchi::result_type mizuchi::next()
{
    static constexpr uint64_t stream = 1;
    static_assert(stream & 1, "must be odd");

    // stream is an odd number
    s = s * 0x41c64e6d + stream;
    const auto x = (s ^ (s >> 23) ^ (s >> 47)) * 0xaef17502108ef2d9;
    return x ^ (x >> 25);
}

// https://github.com/tommyettinger/sarong/blob/master/src/main/java/sarong/Mover64RNG.java
DEF_URBG_SUBCLASS(mover_64, SINGLE_ARG(std::array<uint64_t, 2>), uint64_t)

/// prepare the initial state
void mover_64::init()
{
}

mover_64::result_type mover_64::next()
{
    s[0] += xxh_prime64[0]; // (SDW)
    s[1] += xxh_prime64[1]; // (SDW)
    s[0] = std::rotl(s[0] * 0x41c64e6b, 28);
    s[1] = std::rotl(s[1] * 0x9e3779b9, 37);
    return s[0] ^ s[1];
}

// https://github.com/tommyettinger/sarong/blob/master/src/main/java/sarong/MoverCounter64RNG.java
DEF_URBG_SUBCLASS(mover_counter_64, SINGLE_ARG(std::array<uint64_t, 2>), uint64_t)

/// prepare the initial state
void mover_counter_64::init()
{
}

mover_counter_64::result_type mover_counter_64::next()
{
    s[1] += 0x9e3779b97f4a7af6; // (SDW: XXX: adding an even number?!)
    s[0] = std::rotl(s[0], 21) * s[1];
    return s[0] * 0x41c64e6b;
}

// https://github.com/tommyettinger/sarong/blob/master/src/main/java/sarong/OrbitRNG.java
DEF_URBG_SUBCLASS(orbit, SINGLE_ARG(std::array<uint64_t, 2>), uint64_t)

/// prepare the initial state
void orbit::init()
{
}

orbit::result_type orbit::next()
{
    s[0] += 0xc6bc279692b5c323;
    s[1] += 0x9e3779b97f4a7c15;
    const auto x = (s[0] ^ (s[0] >> 27)) * (s[1] | 1U);
    if (s[0] == 0)
        s[1] -= 0x9e3779b97f4a7c15;
    return x ^ (x >> 27);
}

// https://github.com/tommyettinger/sarong/blob/master/src/main/java/sarong/PelicanRNG.java
DEF_URBG_SUBCLASS(pelican, uint64_t, uint64_t)

/// prepare the initial state
void pelican::init()
{
}

pelican::result_type pelican::next()
{
    auto x = s++;
    x = (x ^ std::rotr(x, 23) ^ std::rotl(x, 17) ^ 0xd1b54a32d192ed03) * 0xaef17502108ef2d9;
    x = (x ^ (x >> 43) ^ (x >> 31) ^ (x >> 23)) * 0xdb4f0b9175ae2165;
    return x ^ (x >> 28);
}

// https://github.com/tommyettinger/sarong/blob/master/src/main/java/sarong/PulleyRNG.java
DEF_URBG_SUBCLASS(pulley, uint64_t, uint64_t)

/// prepare the initial state
void pulley::init()
{
}

pulley::result_type pulley::next()
{
    auto x = s++;
    x = (x ^ std::rotr(x, 23) ^ std::rotl(x, 17)) * 0x369dea0f31a53f85;
    x = (x ^ (x >> 25) ^ (x >> 37)) * 0xdb4f0b9175ae2165;
    return x ^ (x >> 28);
}

// https://github.com/tommyettinger/sarong/blob/master/src/main/java/sarong/QuixoticRNG.java
DEF_URBG_SUBCLASS(quixotic, uint64_t, uint64_t)

/// prepare the initial state
void quixotic::init()
{
}

quixotic::result_type quixotic::next()
{
    s = (s ^ 0x6c8e9cf570932bd5) * 0x41c64e6b;
    const auto x = (s ^ (s >> 27)) + 0xaef17502108ef2d9;
    return x ^ (x >> 25);
}

// https://github.com/tommyettinger/sarong/blob/master/src/main/java/sarong/SeaSlater64RNG.java
DEF_URBG_SUBCLASS(sea_slater_64, SINGLE_ARG(std::array<uint64_t, 2>), uint64_t)

/// prepare the initial state
void sea_slater_64::init()
{
}

sea_slater_64::result_type sea_slater_64::next()
{
    s[0] += xxh_prime64[0]; // (SDW)
    s[1] += xxh_prime64[1]; // (SDW)
    const uint64_t old_s0 = s[0];
    const uint64_t old_s1 = s[1] ^ old_s0;
    const uint64_t x = (old_s0 << 7) - std::rotl(old_s0, 5);
    s[0] = std::rotl(old_s0, 24) ^ old_s1 ^ (old_s1 << 16);
    s[1] = std::rotl(old_s1, 37);
    return (x << 12) - std::rotl(x, 9);
}

// https://github.com/tommyettinger/sarong/blob/master/src/main/java/sarong/TangleRNG.java
DEF_URBG_SUBCLASS(tangle, SINGLE_ARG(std::array<uint64_t, 2>), uint64_t)

/// prepare the initial state
void tangle::init()
{
}

tangle::result_type tangle::next()
{
    s[0] += 0xc6bc279692b5c323;
    s[1] += 0x9e3779b97f4a7c16; // (SDW: XXX: adding an even number?!)
    const auto x = (s[0] ^ (s[0] >> 31)) * s[1];
    return x ^ (x >> 26) ^ (x >> 6);
}

// https://github.com/tommyettinger/sarong/blob/master/src/main/java/sarong/ThrustAltRNG.java
DEF_URBG_SUBCLASS(thrust_alt, uint64_t, uint64_t)

/// prepare the initial state
void thrust_alt::init()
{
}

thrust_alt::result_type thrust_alt::next()
{
    s += 0x6c8e9cf570932bd5;
    const auto x = (s ^ (s >> 25)) * (s | 0xa529);
    return x ^ (x >> 23);
}

// https://github.com/tommyettinger/sarong/blob/master/src/main/java/sarong/ToppingRNG.java
DEF_URBG_SUBCLASS(topping, uint64_t, uint64_t)

/// prepare the initial state
void topping::init()
{
}

topping::result_type topping::next()
{
    auto x = s++;
    x = (x ^ std::rotl(x, 23) ^ std::rotr(x, 17)) * 0xeb44accab455d165;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
}
