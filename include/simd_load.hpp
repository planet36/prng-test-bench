// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// Load data into a SIMD variable
/**
* \file
* \author Steven Ward
*/

#pragma once

#include "simd_types.hpp"

#if defined(DEBUG)
#include <cassert>
#endif
#include <cstddef>
#include <cstring>
#include <span>

/// Load 16 bytes from \a src into a \c uint8x16_t
/**
* \pre the size of \a src is at least 16
*/
[[nodiscard]] static inline uint8x16_t
simd_load16(const std::span<const std::byte> src) noexcept
{
#if defined(DEBUG)
    assert(std::size(src) >= sizeof(uint8x16_t));
#endif

    uint8x16_t dst{};
    (void)std::memcpy(&dst, std::data(src), sizeof(dst));
    return dst;
}

/// Load 16 bytes from \a src into a \c uint8x16_t
/**
* \pre \a src points to at least 16 bytes of data
*/
[[nodiscard]] static inline uint8x16_t
simd_load16(const void* src) noexcept
{
    return simd_load16(std::span{static_cast<const std::byte*>(src), sizeof(uint8x16_t)});
}
