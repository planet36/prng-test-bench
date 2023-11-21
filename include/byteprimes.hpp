// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// numbers where each byte is prime
/**
\file
\author Steven Ward
\sa https://github.com/dragontamer/AESRand/blob/master/AESRand/AESRand/AESRand.cpp#L12
These were inspired by the byte sequence used in AESRand.

1 isn't prime, but every number must be odd.

The last number is left padded with "00".

Fish shell commands to generate the numbers:
\verbatim
begin
echo "01"
openssl prime $(seq 255) | grep -F 'is prime' | cut -d ' ' -f 1 |
sed -e 's/^.$/0&/' ; echo "00"
end |
tac | xargs -n 8 | tac |
sed -e 's/^/0x/' -e 's/ //g' -e 's/$/,/' |
tr '[:upper:]' '[:lower:]'
\endverbatim

Bash shell commands to generate the numbers:
\verbatim
# (bash)
{
echo "01"
openssl prime $(seq 255) | grep -F 'is prime' | cut -d ' ' -f 1 |
sed -e 's/^.$/0&/' ; echo "00"
} |
tac | xargs -n 8 | tac |
sed -e 's/^/0x/' -e 's/ //g' -e 's/$/,/' |
tr '[:upper:]' '[:lower:]'
\endverbatim

The values are are coprime with 2**64.
https://www.wolframalpha.com/input?i=CoprimeQ%5B0x110d0b0705030201%2C+2%5E64%5D
https://www.wolframalpha.com/input?i=CoprimeQ%5B0x2f2b29251f1d1713%2C+2%5E64%5D
https://www.wolframalpha.com/input?i=CoprimeQ%5B0x534f4947433d3b35%2C+2%5E64%5D
https://www.wolframalpha.com/input?i=CoprimeQ%5B0x7f716d6b67656159%2C+2%5E64%5D
https://www.wolframalpha.com/input?i=CoprimeQ%5B0xa7a39d97958b8983%2C+2%5E64%5D
https://www.wolframalpha.com/input?i=CoprimeQ%5B0xd3c7c5c1bfb5b3ad%2C+2%5E64%5D
https://www.wolframalpha.com/input?i=CoprimeQ%5B0x00fbf1efe9e5e3df%2C+2%5E64%5D
*/

#pragma once

#include <array>
#include <cstdint>

namespace
{
inline constexpr std::array<uint64_t, 7> byteprimes = {
	0x110d0b0705030201, // not prime (popcount = 17)
	0x2f2b29251f1d1713, // not prime (popcount = 31)
	0x534f4947433d3b35, // not prime (popcount = 33)
	0x7f716d6b67656159, // not prime (popcount = 37)
	0xa7a39d97958b8983, // not prime (popcount = 33)
	0xd3c7c5c1bfb5b3ad, // not prime (popcount = 39)
	0x00fbf1efe9e5e3df, // not prime (popcount = 41)
};
static_assert((byteprimes[0] & 1) != 0, "must be odd");
static_assert((byteprimes[1] & 1) != 0, "must be odd");
static_assert((byteprimes[2] & 1) != 0, "must be odd");
static_assert((byteprimes[3] & 1) != 0, "must be odd");
static_assert((byteprimes[4] & 1) != 0, "must be odd");
static_assert((byteprimes[5] & 1) != 0, "must be odd");
static_assert((byteprimes[6] & 1) != 0, "must be odd");
}
