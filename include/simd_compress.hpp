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

#include <array>
#include <cstddef>

#if defined(__x86_64__) && defined(__AES__)

#include <immintrin.h>

using uint8x16_t = __m128i;

#elif defined(__aarch64__) && defined(__ARM_FEATURE_AES)

#include <arm_neon.h>

#else

#error "Architecture not supported"

#endif

/// Compress (via 2 rounds of AES encryption) 2 128-bit SIMD registers into 1,
/// non-symmetrically and non-linearly
/**
* \li diffusion rate of \a a = 50.2%
* \li diffusion rate of \a b = 12.7%
*/
[[nodiscard]] static inline uint8x16_t
simd_compress_aes_enc_r2(const uint8x16_t a, const uint8x16_t b) noexcept
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
[[nodiscard]] static inline uint8x16_t
simd_compress_aes_enc_r3(const uint8x16_t a, const uint8x16_t b) noexcept
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
[[nodiscard]] static inline uint8x16_t
simd_compress_aes_enc_r4(const uint8x16_t a, const uint8x16_t b) noexcept
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

#if defined(__x86_64__) && defined(__VAES__)

/// Compress (via 2 rounds of AES encryption) 2 256-bit SIMD registers into 1,
/// non-symmetrically and non-linearly
[[nodiscard]] static inline __m256i
simd_compress_aes_enc_r2(const __m256i a, const __m256i b) noexcept
{
    return _mm256_aesenc_epi128(
                _mm256_aesenc_epi128(a, b),
                a);
}

/// Compress (via 3 rounds of AES encryption) 2 256-bit SIMD registers into 1,
/// non-symmetrically and non-linearly
[[nodiscard]] static inline __m256i
simd_compress_aes_enc_r3(const __m256i a, const __m256i b) noexcept
{
    return _mm256_aesenc_epi128(
                _mm256_aesenc_epi128(
                    _mm256_aesenc_epi128(b, a),
                    b),
                a);
}

/// Compress (via 4 rounds of AES encryption) 2 256-bit SIMD registers into 1,
/// non-symmetrically and non-linearly
[[nodiscard]] static inline __m256i
simd_compress_aes_enc_r4(const __m256i a, const __m256i b) noexcept
{
    return _mm256_aesenc_epi128(
                _mm256_aesenc_epi128(
                    _mm256_aesenc_epi128(
                        _mm256_aesenc_epi128(a, b),
                        a),
                    b),
                a);
}

#endif

#if defined(__x86_64__) && defined(__VAES__)

/// Perform \c simd_compress_aes_enc_r2 on corresponding elements of \a arr_1 and \a arr_2
/**
* \pre \a arr_2 points to \a N elements
*/
template <size_t N>
requires (N > 0) && ((N % 2) == 0) // N must be positive and even
static void
simd_compress_aes_enc_r2_arr(std::array<uint8x16_t, N>& arr_1, const uint8x16_t* arr_2) noexcept
{
    for (unsigned int i = 0; i < N; i += 2)
    {
        // Cast adjacent pairs of elements to __m256i.
        __m256i v_1 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&arr_1[i]));
        __m256i v_2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&arr_2[i]));

        v_1 = simd_compress_aes_enc_r2(v_1, v_2);

        _mm256_storeu_si256(reinterpret_cast<__m256i*>(&arr_1[i]), v_1);
    }
}

/// Perform \c simd_compress_aes_enc_r3 on corresponding elements of \a arr_1 and \a arr_2
/**
* \pre \a arr_2 points to \a N elements
*/
template <size_t N>
requires (N > 0) && ((N % 2) == 0) // N must be positive and even
static void
simd_compress_aes_enc_r3_arr(std::array<uint8x16_t, N>& arr_1, const uint8x16_t* arr_2) noexcept
{
    for (unsigned int i = 0; i < N; i += 2)
    {
        // Cast adjacent pairs of elements to __m256i.
        __m256i v_1 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&arr_1[i]));
        __m256i v_2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&arr_2[i]));

        v_1 = simd_compress_aes_enc_r3(v_1, v_2);

        _mm256_storeu_si256(reinterpret_cast<__m256i*>(&arr_1[i]), v_1);
    }
}

/// Perform \c simd_compress_aes_enc_r4 on corresponding elements of \a arr_1 and \a arr_2
/**
* \pre \a arr_2 points to \a N elements
*/
template <size_t N>
requires (N > 0) && ((N % 2) == 0) // N must be positive and even
static void
simd_compress_aes_enc_r4_arr(std::array<uint8x16_t, N>& arr_1, const uint8x16_t* arr_2) noexcept
{
    for (unsigned int i = 0; i < N; i += 2)
    {
        // Cast adjacent pairs of elements to __m256i.
        __m256i v_1 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&arr_1[i]));
        __m256i v_2 = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(&arr_2[i]));

        v_1 = simd_compress_aes_enc_r4(v_1, v_2);

        _mm256_storeu_si256(reinterpret_cast<__m256i*>(&arr_1[i]), v_1);
    }
}

#endif

/// Perform \c simd_compress_aes_enc_r2 on corresponding elements of \a arr_1 and \a arr_2
/**
* \pre \a arr_2 points to \a N elements
*/
template <size_t N>
static void
simd_compress_aes_enc_r2_arr(std::array<uint8x16_t, N>& arr_1, const uint8x16_t* arr_2) noexcept
{
    for (unsigned int i = 0; i < N; ++i)
    {
        arr_1[i] = simd_compress_aes_enc_r2(arr_1[i], arr_2[i]);
    }
}

/// Perform \c simd_compress_aes_enc_r3 on corresponding elements of \a arr_1 and \a arr_2
/**
* \pre \a arr_2 points to \a N elements
*/
template <size_t N>
static void
simd_compress_aes_enc_r3_arr(std::array<uint8x16_t, N>& arr_1, const uint8x16_t* arr_2) noexcept
{
    for (unsigned int i = 0; i < N; ++i)
    {
        arr_1[i] = simd_compress_aes_enc_r3(arr_1[i], arr_2[i]);
    }
}

/// Perform \c simd_compress_aes_enc_r4 on corresponding elements of \a arr_1 and \a arr_2
/**
* \pre \a arr_2 points to \a N elements
*/
template <size_t N>
static void
simd_compress_aes_enc_r4_arr(std::array<uint8x16_t, N>& arr_1, const uint8x16_t* arr_2) noexcept
{
    for (unsigned int i = 0; i < N; ++i)
    {
        arr_1[i] = simd_compress_aes_enc_r4(arr_1[i], arr_2[i]);
    }
}
