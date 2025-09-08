// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Golden ratio scaled to unsigned integers
/**
* \file
* \author Steven Ward
*/

#pragma once

#include <stdint.h>

/// GoldenRatio scaled to uint16_t
/**
* https://www.wolframalpha.com/input?i=IntegerString%5BFloor%5B%282**16%29*Frac%5BGoldenRatio%5D%5D%2C16%5D
*/
#define GOLDEN_RATIO_16 UINT16_C(0x9e37) // not prime (popcount = 10)

/// GoldenRatio scaled to uint32_t
/**
* https://www.wolframalpha.com/input?i=IntegerString%5BFloor%5B%282**32%29*Frac%5BGoldenRatio%5D%5D%2C16%5D
*/
#define GOLDEN_RATIO_32 UINT32_C(0x9e3779b9) // not prime (popcount = 20)

/// GoldenRatio scaled to uint64_t
/**
* https://www.wolframalpha.com/input?i=IntegerString%5BFloor%5B%282**64%29*Frac%5BGoldenRatio%5D%5D%2C16%5D
*/
#define GOLDEN_RATIO_64 UINT64_C(0x9e3779b97f4a7c15) // not prime (popcount = 38)
