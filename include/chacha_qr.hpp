// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// ChaCha quarter-round function
/**
* \file
* \author Steven Ward
* \sa https://cr.yp.to/chacha/chacha-20080128.pdf
* \sa https://datatracker.ietf.org/doc/html/rfc8439#section-2.1
* \sa https://en.wikipedia.org/wiki/Salsa20#ChaCha_variant
*/

#pragma once

#include <bit>
#include <cstdint>

constexpr void
chacha_qr(uint32_t& a, uint32_t& b, uint32_t& c, uint32_t& d)
{
    a += b;
    d = std::rotl(d ^ a, 16);
    c += d;
    b = std::rotl(b ^ c, 12);
    a += b;
    d = std::rotl(d ^ a, 8);
    c += d;
    b = std::rotl(b ^ c, 7);
}
