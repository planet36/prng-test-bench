// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

#pragma once

#include <immintrin.h>

union union_128
{
	__m128i xmm;
	__int128_t i128;
	__uint128_t u128;
};
