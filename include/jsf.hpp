// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// JSF (Jenkins' Small Fast) PRNG by Bob Jenkins
/**
\file
\author Steven Ward
\sa https://burtleburtle.net/bob/rand/smallprng.html
*/

#pragma once

#include "def_urbg_class.hpp"

#include <array>
#include <bit>
#include <cstdint>

// 2-rotate version
DEF_URBG_CLASS(jsf32_2, SINGLE_ARG(std::array<uint32_t, 4>), uint32_t)
{
    static constexpr unsigned int R1 = 27;
    static constexpr unsigned int R2 = 17;

    ++s[0]; // (SDW)
    const auto e = s[0] - std::rotl(s[1], R1);
    s[0] = s[1] ^ std::rotl(s[2], R2);
    s[1] = s[2] + s[3];
    s[2] = s[3] + e;
    s[3] = e + s[0];
    return s[3];
}

// 3-rotate version
DEF_URBG_CLASS(jsf32_3, SINGLE_ARG(std::array<uint32_t, 4>), uint32_t)
{
    static constexpr unsigned int R1 = 23;
    static constexpr unsigned int R2 = 16;
    static constexpr unsigned int R3 = 11;

    ++s[0]; // (SDW)
    const auto e = s[0] - std::rotl(s[1], R1);
    s[0] = s[1] ^ std::rotl(s[2], R2);
    s[1] = s[2] + std::rotl(s[3], R3);
    s[2] = s[3] + e;
    s[3] = e + s[0];
    return s[3];
}

DEF_URBG_CLASS(jsf64, SINGLE_ARG(std::array<uint64_t, 4>), uint64_t)
{
    static constexpr unsigned int R1 = 7;
    static constexpr unsigned int R2 = 13;
    static constexpr unsigned int R3 = 37;

    ++s[0]; // (SDW)
    const auto e = s[0] - std::rotl(s[1], R1);
    s[0] = s[1] ^ std::rotl(s[2], R2);
    s[1] = s[2] + std::rotl(s[3], R3);
    s[2] = s[3] + e;
    s[3] = e + s[0];
    return s[3];
}
