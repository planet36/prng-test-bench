// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Alias templates for arrays of SIMD types
/**
\file
\author Steven Ward
*/

#pragma once

#include <array>
#include <cstddef>
#include <immintrin.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"

template <size_t N>
using arr_m128i = std::array<alignas(sizeof(__m128i)) __m128i, N>;

template <size_t N>
using arr_m256i = std::array<alignas(sizeof(__m256i)) __m256i, N>;

template <size_t N>
using arr_m512i = std::array<alignas(sizeof(__m512i)) __m512i, N>;

#pragma GCC diagnostic pop
