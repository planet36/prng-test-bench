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
using uint8x16_t = __m128i;

#if defined(__AVX__)
using uint8x16x2_t = __m256i;
#endif

#elif defined(__aarch64__) && defined(__ARM_FEATURE_AES)
#include <arm_neon.h>
#else
#error "Architecture not supported"
#endif

/// Compress (via 2 rounds of AES encryption) 2 128-bit SIMD registers into 1,
/// non-symmetrically and non-linearly
/**
* \li diffusion rate of \a a = 50.3%
* \li diffusion rate of \a b = 12.7%
*/
static inline uint8x16_t
compress_aesenc2(const uint8x16_t a, const uint8x16_t b)
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
compress_aesenc3(const uint8x16_t a, const uint8x16_t b)
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
compress_aesenc4(const uint8x16_t a, const uint8x16_t b)
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

/// Compress (via 2 rounds of AES encryption) 2 256-bit SIMD registers into 1,
/// non-symmetrically and non-linearly
static inline uint8x16x2_t
compress_aesenc2(const uint8x16x2_t a, const uint8x16x2_t b)
{
#if defined(__x86_64__) && defined(__VAES__)
    return _mm256_aesenc_epi128(
                _mm256_aesenc_epi128(a, b),
                a);
#elif defined(__x86_64__) && defined(__AES__)
    return _mm256_setr_m128i(
            compress_aesenc2(
                _mm256_extracti128_si256(a, 0),
                _mm256_extracti128_si256(b, 0)),
            compress_aesenc2(
                _mm256_extracti128_si256(a, 1),
                _mm256_extracti128_si256(b, 1))
            );
#elif defined(__aarch64__) && defined(__ARM_FEATURE_AES)
    return { compress_aesenc2(a.val[0], b.val[0]),
        compress_aesenc2(a.val[1], b.val[1]) };
#endif
}

/// Compress (via 3 rounds of AES encryption) 2 256-bit SIMD registers into 1,
/// non-symmetrically and non-linearly
static inline uint8x16x2_t
compress_aesenc3(const uint8x16x2_t a, const uint8x16x2_t b)
{
#if defined(__x86_64__) && defined(__VAES__)
    return _mm256_aesenc_epi128(
                _mm256_aesenc_epi128(
                    _mm256_aesenc_epi128(b, a),
                    b),
                a);
#elif defined(__x86_64__) && defined(__AES__)
    return _mm256_setr_m128i(
            compress_aesenc3(
                _mm256_extracti128_si256(a, 0),
                _mm256_extracti128_si256(b, 0)),
            compress_aesenc3(
                _mm256_extracti128_si256(a, 1),
                _mm256_extracti128_si256(b, 1))
            );
#elif defined(__aarch64__) && defined(__ARM_FEATURE_AES)
    return { compress_aesenc3(a.val[0], b.val[0]),
        compress_aesenc3(a.val[1], b.val[1]) };
#endif
}

/// Compress (via 4 rounds of AES encryption) 2 256-bit SIMD registers into 1,
/// non-symmetrically and non-linearly
static inline uint8x16x2_t
compress_aesenc4(const uint8x16x2_t a, const uint8x16x2_t b)
{
#if defined(__x86_64__) && defined(__VAES__)
    return _mm256_aesenc_epi128(
                _mm256_aesenc_epi128(
                    _mm256_aesenc_epi128(
                        _mm256_aesenc_epi128(a, b),
                        a),
                    b),
                a);
#elif defined(__x86_64__) && defined(__AES__)
    return _mm256_setr_m128i(
            compress_aesenc4(
                _mm256_extracti128_si256(a, 0),
                _mm256_extracti128_si256(b, 0)),
            compress_aesenc4(
                _mm256_extracti128_si256(a, 1),
                _mm256_extracti128_si256(b, 1))
            );
#elif defined(__aarch64__) && defined(__ARM_FEATURE_AES)
    return { compress_aesenc4(a.val[0], b.val[0]),
        compress_aesenc4(a.val[1], b.val[1]) };
#endif
}
