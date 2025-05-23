// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// seeds and seed sequences used by the test suite
/**
\file
\author Steven Ward
*/

#pragma once

#include <array>
#include <cstdint>
#include <random>

/**
Python snippet to generate the array
\code{.py}
from itertools import batched

for n in (4, 8, 12, 16, 24, 32, 64, 128):
    w = 256//n
    print(f"inline constexpr std::array<uint8_t, {n}> seed_bytes_pattern_{n}{{")
    #bytes = [w//2 + w*i for i in range(n)]
    bytes = n * [0xaa]
    for bytes_group in batched(bytes, 8):
        print('    ' + ', '.join(f'0x{x:02X}' for x in bytes_group) + ',')
    print("};\n")
\endcode
*/

inline constexpr std::array<uint8_t, 4> seed_bytes_pattern_4{
    0xAA, 0xAA, 0xAA, 0xAA,
};

inline constexpr std::array<uint8_t, 8> seed_bytes_pattern_8{
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
};

inline constexpr std::array<uint8_t, 12> seed_bytes_pattern_12{
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA,
};

inline constexpr std::array<uint8_t, 16> seed_bytes_pattern_16{
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
};

inline constexpr std::array<uint8_t, 24> seed_bytes_pattern_24{
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
};

inline constexpr std::array<uint8_t, 32> seed_bytes_pattern_32{
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
};

inline constexpr std::array<uint8_t, 64> seed_bytes_pattern_64{
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
};

inline constexpr std::array<uint8_t, 128> seed_bytes_pattern_128{
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
};

template <std::uniform_random_bit_generator URBG>
constexpr auto
get_seed_bytes_zero()
{
    constexpr size_t num_seed_bytes = sizeof(typename URBG::seed_bytes_type);
    return std::array<uint8_t, num_seed_bytes>{};
}

template <std::uniform_random_bit_generator URBG>
constexpr auto
get_seed_bytes_pattern()
{
    constexpr size_t num_seed_bytes = sizeof(typename URBG::seed_bytes_type);
    if constexpr (num_seed_bytes == 4) { return seed_bytes_pattern_4; }
    else if constexpr (num_seed_bytes == 8) { return seed_bytes_pattern_8; }
    else if constexpr (num_seed_bytes == 12) { return seed_bytes_pattern_12; }
    else if constexpr (num_seed_bytes == 16) { return seed_bytes_pattern_16; }
    else if constexpr (num_seed_bytes == 24) { return seed_bytes_pattern_24; }
    else if constexpr (num_seed_bytes == 32) { return seed_bytes_pattern_32; }
    else if constexpr (num_seed_bytes == 64) { return seed_bytes_pattern_64; }
    else if constexpr (num_seed_bytes == 128) { return seed_bytes_pattern_128; }
}
