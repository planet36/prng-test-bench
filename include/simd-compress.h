// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// One-way compression functions for 2 SIMD registers
/**
* \file
* \author Steven Ward
*
* \sa https://blog.michaelbrase.com/2018/05/08/emulating-x86-aes-intrinsics-on-armv8-a/
* \sa https://blog.michaelbrase.com/2018/06/04/optimizing-x86-aes-intrinsics-on-armv8-a/
*
* ## AES instruction comparison
* \verbatim
|--------|---------------|-------------|--------|
| Round  | AES Step      | Intel       | ARM    |
|--------|---------------|-------------|--------|
| 0      | AddRoundKey   | XOR         | AESE   |
|--------|---------------|-------------|        |
| 1      | SubBytes      | AESENC      |        |
|        |               |             |        |
|        | ShiftRows     |             |        |
|        |               |             |--------|
|        | MixColumns    |             | AESMC  |
|        |               |             |--------|
|        | AddRoundKey   |             | AESE   |
|--------|---------------|-------------|        |
| 2      | SubBytes      | AESENC      |        |
|        |               |             |        |
|        | ShiftRows     |             |        |
|        |               |             |--------|
|        | MixColumns    |             | AESMC  |
|        |               |             |--------|
|        | AddRoundKey   |             | AESE   |
|--------|---------------|-------------|        |
| 3      | SubBytes      | AESENC      |        |
|        |               |             |        |
|        | ShiftRows     |             |        |
|        |               |             |--------|
|        | MixColumns    |             | AESMC  |
|        |               |             |--------|
|        | AddRoundKey   |             | AESE   |
|--------|---------------|-------------|        |
| last   | SubBytes      | AESENCLAST  |        |
|        |               |             |        |
|        | ShiftRows     |             |        |
|        |               |             |--------|
|        | AddRoundKey   |             | XOR    |
| -------|---------------|-------------|--------|
\endverbatim
*/

#pragma once

#if defined(__x86_64__) && defined(__AES__)
#include <immintrin.h>
typedef __m128i uint8x16_t;
#elif defined(__aarch64__) && defined(__ARM_FEATURE_AES)
#include <arm_neon.h>
#else
#error "Architecture not supported"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/// Compress (via 1 round of AES encryption) 2 128-bit SIMD registers into 1,
/// non-symmetrically and non-linearly
/**
* \li diffusion rate of \a a = 0.8%
* \li diffusion rate of \a b = 12.6%
*/
static inline uint8x16_t
compress_aesenc1_128(const uint8x16_t a, const uint8x16_t b)
{
#if defined(__x86_64__) && defined(__AES__)
    return _mm_aesenc_si128(b, a);
#elif defined(__aarch64__) && defined(__ARM_FEATURE_AES)
    const uint8x16_t zero = vdupq_n_u8(0);
    return vaesmcq_u8(vaeseq_u8(b, zero)) ^ a;
#endif
}

/// Compress (via 2 rounds of AES encryption) 2 128-bit SIMD registers into 1,
/// non-symmetrically and non-linearly
/**
* \li diffusion rate of \a a = 50.3%
* \li diffusion rate of \a b = 12.7%
*/
static inline uint8x16_t
compress_aesenc2_128(const uint8x16_t a, const uint8x16_t b)
{
#if defined(__x86_64__) && defined(__AES__)
    return _mm_aesenc_si128(
                _mm_aesenc_si128(a, b),
                a);
#elif defined(__aarch64__) && defined(__ARM_FEATURE_AES)
    const uint8x16_t zero = vdupq_n_u8(0);
    return vaesmcq_u8(vaeseq_u8(
                    vaesmcq_u8(vaeseq_u8(a, zero)),
                    b)) ^ a;
#endif
}

/// Compress (via 3 rounds of AES encryption) 2 128-bit SIMD registers into 1,
/// non-symmetrically and non-linearly
/**
* \li diffusion rate of \a a = 50.2%
* \li diffusion rate of \a b = 50.0%
*/
static inline uint8x16_t
compress_aesenc3_128(const uint8x16_t a, const uint8x16_t b)
{
#if defined(__x86_64__) && defined(__AES__)
    return _mm_aesenc_si128(
                _mm_aesenc_si128(
                    _mm_aesenc_si128(b, a),
                    b),
                a);
#elif defined(__aarch64__) && defined(__ARM_FEATURE_AES)
    const uint8x16_t zero = vdupq_n_u8(0);
    return vaesmcq_u8(vaeseq_u8(
                    vaesmcq_u8(vaeseq_u8(
                            vaesmcq_u8(vaeseq_u8(b, zero)),
                            a)),
                    b)) ^ a;
#endif
}

/// Compress (via 4 rounds of AES encryption) 2 128-bit SIMD registers into 1,
/// non-symmetrically and non-linearly
/**
* \li diffusion rate of \a a = 50.0%
* \li diffusion rate of \a b = 50.0%
*/
static inline uint8x16_t
compress_aesenc4_128(const uint8x16_t a, const uint8x16_t b)
{
#if defined(__x86_64__) && defined(__AES__)
    return _mm_aesenc_si128(
                _mm_aesenc_si128(
                    _mm_aesenc_si128(
                        _mm_aesenc_si128(a, b),
                        a),
                    b),
                a);
#elif defined(__aarch64__) && defined(__ARM_FEATURE_AES)
    const uint8x16_t zero = vdupq_n_u8(0);
    return vaesmcq_u8(vaeseq_u8(
                    vaesmcq_u8(vaeseq_u8(
                            vaesmcq_u8(vaeseq_u8(
                                    vaesmcq_u8(vaeseq_u8(a, zero)),
                                    b)),
                            a)),
                    b)) ^ a;
#endif
}

#ifdef __cplusplus
} // extern "C"
#endif
