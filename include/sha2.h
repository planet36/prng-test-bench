// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// SHA-2 initial hash values (H0) and constants
/**
* \file
* \author Steven Ward
* \sa https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
* \sa https://en.wikipedia.org/wiki/SHA-2
*/

#pragma once

#if defined(__cplusplus)
#include <array>
#include <cstdint>
#else
#if __STDC_VERSION__ < 202311L
#error "C23 or later required"
#endif
#include <stdint.h>
#endif

/**
* ### 5.3.2 SHA-224
*
* > For SHA-224, the initial hash value, H(0), shall consist of the following
* > eight 32-bit words, in hex:
*
* >  These words were obtained by taking the second thirty-two bits of the
* >  fractional parts of the square roots of the ninth through sixteenth prime
* >  numbers.
*/
alignas(64)
#if defined(__cplusplus)
inline constexpr std::array<std::uint32_t, 8> SHA_224_H0{
#else
constexpr uint32_t SHA_224_H0[8] = {
#endif
    0xc1059ed8,
    0x367cd507,
    0x3070dd17,
    0xf70e5939,
    0xffc00b31,
    0x68581511,
    0x64f98fa7,
    0xbefa4fa4,
};

/**
* ### 5.3.3 SHA-256
*
* > For SHA-256, the initial hash value, H(0), shall consist of the following
* > eight 32-bit words, in hex:
*
* > These words were obtained by taking the first thirty-two bits of the
* > fractional parts of the square roots of the first eight prime numbers.
*/
alignas(64)
#if defined(__cplusplus)
inline constexpr std::array<std::uint32_t, 8> SHA_256_H0{
#else
constexpr uint32_t SHA_256_H0[8] = {
#endif
    0x6a09e667,
    0xbb67ae85,
    0x3c6ef372,
    0xa54ff53a,
    0x510e527f,
    0x9b05688c,
    0x1f83d9ab,
    0x5be0cd19,
};

/**
* ### 5.3.4 SHA-384
*
* > For SHA-384, the initial hash value, H(0), shall consist of the following
* > eight 64-bit words, in hex:
*
* > These words were obtained by taking the first sixty-four bits of the
* > fractional parts of the square roots of the ninth through sixteenth prime
* > numbers.
*/
alignas(64)
#if defined(__cplusplus)
inline constexpr std::array<std::uint64_t, 8> SHA_384_H0{
#else
constexpr uint64_t SHA_384_H0[8] = {
#endif
    UINT64_C(0xcbbb9d5dc1059ed8),
    UINT64_C(0x629a292a367cd507),
    UINT64_C(0x9159015a3070dd17),
    UINT64_C(0x152fecd8f70e5939),
    UINT64_C(0x67332667ffc00b31),
    UINT64_C(0x8eb44a8768581511),
    UINT64_C(0xdb0c2e0d64f98fa7),
    UINT64_C(0x47b5481dbefa4fa4),
};

/**
* ### 5.3.5 SHA-512
*
* > For SHA-512, the initial hash value, H(0), shall consist of the following
* > eight 64-bit words, in hex:
*
* > These words were obtained by taking the first sixty-four bits of the
* > fractional parts of the square roots of the first eight prime numbers.
*/
alignas(64)
#if defined(__cplusplus)
inline constexpr std::array<std::uint64_t, 8> SHA_512_H0{
#else
constexpr uint64_t SHA_512_H0[8] = {
#endif
    UINT64_C(0x6a09e667f3bcc908),
    UINT64_C(0xbb67ae8584caa73b),
    UINT64_C(0x3c6ef372fe94f82b),
    UINT64_C(0xa54ff53a5f1d36f1),
    UINT64_C(0x510e527fade682d1),
    UINT64_C(0x9b05688c2b3e6c1f),
    UINT64_C(0x1f83d9abfb41bd6b),
    UINT64_C(0x5be0cd19137e2179),
};

/**
* ### 4.2.2 SHA-224 and SHA-256 Constants
*
* > These words represent the first thirty-two bits of the fractional parts of
* > the cube roots of the first sixty-four prime numbers. In hex, these constant
* > words are (from left to right)
*/
alignas(64)
#if defined(__cplusplus)
inline constexpr std::array<std::uint32_t, 64> SHA_256_C{
#else
constexpr uint32_t SHA_256_C[64] = {
#endif
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

/**
* ### 4.2.3 SHA-384, SHA-512, SHA-512/224 and SHA-512/256 Constants
*
* > These words represent the first sixty-four bits of the fractional parts of
* > the cube roots of the first eighty prime numbers. In hex, these constant
* > words are (from left to right)
*/
alignas(64)
#if defined(__cplusplus)
inline constexpr std::array<std::uint64_t, 80> SHA_512_C{
#else
constexpr uint64_t SHA_512_C[80] = {
#endif
    UINT64_C(0x428a2f98d728ae22), UINT64_C(0x7137449123ef65cd),
    UINT64_C(0xb5c0fbcfec4d3b2f), UINT64_C(0xe9b5dba58189dbbc),
    UINT64_C(0x3956c25bf348b538), UINT64_C(0x59f111f1b605d019),
    UINT64_C(0x923f82a4af194f9b), UINT64_C(0xab1c5ed5da6d8118),
    UINT64_C(0xd807aa98a3030242), UINT64_C(0x12835b0145706fbe),
    UINT64_C(0x243185be4ee4b28c), UINT64_C(0x550c7dc3d5ffb4e2),
    UINT64_C(0x72be5d74f27b896f), UINT64_C(0x80deb1fe3b1696b1),
    UINT64_C(0x9bdc06a725c71235), UINT64_C(0xc19bf174cf692694),
    UINT64_C(0xe49b69c19ef14ad2), UINT64_C(0xefbe4786384f25e3),
    UINT64_C(0x0fc19dc68b8cd5b5), UINT64_C(0x240ca1cc77ac9c65),
    UINT64_C(0x2de92c6f592b0275), UINT64_C(0x4a7484aa6ea6e483),
    UINT64_C(0x5cb0a9dcbd41fbd4), UINT64_C(0x76f988da831153b5),
    UINT64_C(0x983e5152ee66dfab), UINT64_C(0xa831c66d2db43210),
    UINT64_C(0xb00327c898fb213f), UINT64_C(0xbf597fc7beef0ee4),
    UINT64_C(0xc6e00bf33da88fc2), UINT64_C(0xd5a79147930aa725),
    UINT64_C(0x06ca6351e003826f), UINT64_C(0x142929670a0e6e70),
    UINT64_C(0x27b70a8546d22ffc), UINT64_C(0x2e1b21385c26c926),
    UINT64_C(0x4d2c6dfc5ac42aed), UINT64_C(0x53380d139d95b3df),
    UINT64_C(0x650a73548baf63de), UINT64_C(0x766a0abb3c77b2a8),
    UINT64_C(0x81c2c92e47edaee6), UINT64_C(0x92722c851482353b),
    UINT64_C(0xa2bfe8a14cf10364), UINT64_C(0xa81a664bbc423001),
    UINT64_C(0xc24b8b70d0f89791), UINT64_C(0xc76c51a30654be30),
    UINT64_C(0xd192e819d6ef5218), UINT64_C(0xd69906245565a910),
    UINT64_C(0xf40e35855771202a), UINT64_C(0x106aa07032bbd1b8),
    UINT64_C(0x19a4c116b8d2d0c8), UINT64_C(0x1e376c085141ab53),
    UINT64_C(0x2748774cdf8eeb99), UINT64_C(0x34b0bcb5e19b48a8),
    UINT64_C(0x391c0cb3c5c95a63), UINT64_C(0x4ed8aa4ae3418acb),
    UINT64_C(0x5b9cca4f7763e373), UINT64_C(0x682e6ff3d6b2b8a3),
    UINT64_C(0x748f82ee5defb2fc), UINT64_C(0x78a5636f43172f60),
    UINT64_C(0x84c87814a1f0ab72), UINT64_C(0x8cc702081a6439ec),
    UINT64_C(0x90befffa23631e28), UINT64_C(0xa4506cebde82bde9),
    UINT64_C(0xbef9a3f7b2c67915), UINT64_C(0xc67178f2e372532b),
    UINT64_C(0xca273eceea26619c), UINT64_C(0xd186b8c721c0c207),
    UINT64_C(0xeada7dd6cde0eb1e), UINT64_C(0xf57d4f7fee6ed178),
    UINT64_C(0x06f067aa72176fba), UINT64_C(0x0a637dc5a2c898a6),
    UINT64_C(0x113f9804bef90dae), UINT64_C(0x1b710b35131c471b),
    UINT64_C(0x28db77f523047d84), UINT64_C(0x32caab7b40c72493),
    UINT64_C(0x3c9ebe0a15c9bebc), UINT64_C(0x431d67c49c100d4c),
    UINT64_C(0x4cc5d4becb3e42b6), UINT64_C(0x597f299cfc657e2a),
    UINT64_C(0x5fcb6fab3ad6faec), UINT64_C(0x6c44198c4a475817),
};
