// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// seeds and seed sequences used by the test suite
/**
\file
\author Steven Ward
*/

#pragma once

#include "seed_seq.hpp"

#include <array>
#include <cstdint>
#include <random>

inline constexpr uint32_t seed_zero_32{};
inline constexpr uint64_t seed_zero_64{};

fill_seed_seq seeder_zero;

/*

bin(0xacac3535)
'0b10101100101011000011010100110101'

bin(0xcaca5353)
'0b11001010110010100101001101010011'

bin(0xacac3535caca5353)
'0b1010110010101100001101010011010111001010110010100101001101010011'

bin(0xcaca5353acac3535)
'0b1100101011001010010100110101001110101100101011000011010100110101'

bin(0xAAAAAAAAAAAAAAAA)
'0b1010101010101010101010101010101010101010101010101010101010101010'

*/

//inline constexpr uint32_t seed_pattern_32{0xcaca5353}; // popcount = 16
// This value matches the one in seed_bytes_pattern_4
//inline constexpr uint32_t seed_pattern_32{0xE0A06020};
inline constexpr uint32_t seed_pattern_32{0xAAAAAAAA};
//inline constexpr uint64_t seed_pattern_64{0xcaca5353acac3535}; // popcount = 32
// This value matches the one in seed_bytes_pattern_8
//inline constexpr uint64_t seed_pattern_64{0xF0D0B09070503010};
inline constexpr uint64_t seed_pattern_64{0xAAAAAAAAAAAAAAAA};

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

template <std::uniform_random_bit_generator G>
constexpr auto
get_seed_bytes_zero()
{
	constexpr size_t num_seed_bytes = sizeof(typename G::seed_bytes_type);
	return std::array<uint8_t, num_seed_bytes>{};
}

template <std::uniform_random_bit_generator G>
constexpr auto
get_seed_bytes_pattern()
{
	constexpr size_t num_seed_bytes = sizeof(typename G::seed_bytes_type);
	if constexpr (num_seed_bytes == 4) { return seed_bytes_pattern_4; }
	else if constexpr (num_seed_bytes == 8) { return seed_bytes_pattern_8; }
	else if constexpr (num_seed_bytes == 12) { return seed_bytes_pattern_12; }
	else if constexpr (num_seed_bytes == 16) { return seed_bytes_pattern_16; }
	else if constexpr (num_seed_bytes == 24) { return seed_bytes_pattern_24; }
	else if constexpr (num_seed_bytes == 32) { return seed_bytes_pattern_32; }
	else if constexpr (num_seed_bytes == 64) { return seed_bytes_pattern_64; }
	else if constexpr (num_seed_bytes == 128) { return seed_bytes_pattern_128; }
}

fill_seed_seq<seed_pattern_32> seeder_pattern;
