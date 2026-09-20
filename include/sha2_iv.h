// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// Initial hash values (H0) of SHA-2 functions
/**
* \file
* \author Steven Ward
* \sa https://csrc.nist.gov/pubs/fips/180-4/upd1/final
* \sa https://en.wikipedia.org/wiki/SHA-2
*/

#pragma once

#include <stdint.h>

// NOLINTBEGIN(cppcoreguidelines-macro-usage)

// FIPS PUB 180-4
// FEDERAL INFORMATION PROCESSING STANDARDS PUBLICATION
// Secure Hash Standard (SHS)
// https://csrc.nist.gov/pubs/fips/180-4/upd1/final

// 5.3.2 SHA-224
// For SHA-224, the initial hash value, H(0), shall consist of the following
// eight 32-bit words, in hex:

#define SHA_224_H0_0 UINT32_C(0xc1059ed8)
#define SHA_224_H0_1 UINT32_C(0x367cd507)
#define SHA_224_H0_2 UINT32_C(0x3070dd17)
#define SHA_224_H0_3 UINT32_C(0xf70e5939)
#define SHA_224_H0_4 UINT32_C(0xffc00b31)
#define SHA_224_H0_5 UINT32_C(0x68581511)
#define SHA_224_H0_6 UINT32_C(0x64f98fa7)
#define SHA_224_H0_7 UINT32_C(0xbefa4fa4)

// These words were obtained by taking the second thirty-two bits of the
// fractional parts of the square roots of the ninth through sixteenth prime
// numbers.

// 5.3.3 SHA-256
// For SHA-256, the initial hash value, H(0), shall consist of the following
// eight 32-bit words, in hex:

#define SHA_256_H0_0 UINT32_C(0x6a09e667)
#define SHA_256_H0_1 UINT32_C(0xbb67ae85)
#define SHA_256_H0_2 UINT32_C(0x3c6ef372)
#define SHA_256_H0_3 UINT32_C(0xa54ff53a)
#define SHA_256_H0_4 UINT32_C(0x510e527f)
#define SHA_256_H0_5 UINT32_C(0x9b05688c)
#define SHA_256_H0_6 UINT32_C(0x1f83d9ab)
#define SHA_256_H0_7 UINT32_C(0x5be0cd19)

// These words were obtained by taking the first thirty-two bits of the
// fractional parts of the square roots of the first eight prime numbers.

// 5.3.4 SHA-384
// For SHA-384, the initial hash value, H(0), shall consist of the following
// eight 64-bit words, in hex:

#define SHA_384_H0_0 UINT64_C(0xcbbb9d5dc1059ed8)
#define SHA_384_H0_1 UINT64_C(0x629a292a367cd507)
#define SHA_384_H0_2 UINT64_C(0x9159015a3070dd17)
#define SHA_384_H0_3 UINT64_C(0x152fecd8f70e5939)
#define SHA_384_H0_4 UINT64_C(0x67332667ffc00b31)
#define SHA_384_H0_5 UINT64_C(0x8eb44a8768581511)
#define SHA_384_H0_6 UINT64_C(0xdb0c2e0d64f98fa7)
#define SHA_384_H0_7 UINT64_C(0x47b5481dbefa4fa4)

// These words were obtained by taking the first sixty-four bits of the
// fractional parts of the square roots of the ninth through sixteenth prime
// numbers.

// 5.3.5 SHA-512
// For SHA-512, the initial hash value, H(0), shall consist of the following
// eight 64-bit words, in hex:

#define SHA_512_H0_0 UINT64_C(0x6a09e667f3bcc908)
#define SHA_512_H0_1 UINT64_C(0xbb67ae8584caa73b)
#define SHA_512_H0_2 UINT64_C(0x3c6ef372fe94f82b)
#define SHA_512_H0_3 UINT64_C(0xa54ff53a5f1d36f1)
#define SHA_512_H0_4 UINT64_C(0x510e527fade682d1)
#define SHA_512_H0_5 UINT64_C(0x9b05688c2b3e6c1f)
#define SHA_512_H0_6 UINT64_C(0x1f83d9abfb41bd6b)
#define SHA_512_H0_7 UINT64_C(0x5be0cd19137e2179)

// These words were obtained by taking the first sixty-four bits of the
// fractional parts of the square roots of the first eight prime numbers.

// NOLINTEND(cppcoreguidelines-macro-usage)
