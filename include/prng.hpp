// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// all header files of PRNGs to be tested, map of PRNG names to PRNG info
/**
* \file
* \author Steven Ward
*/

#pragma once

#include "MRG32k3a.hpp"
#include "aes_compress_ctr2_128.hpp"
#include "aes_ctr_128.hpp"
#include "biski64.hpp"
#include "clmulrand.hpp"
#include "degski.hpp"
#include "ettinger_mixer.hpp"
#include "gjrand.hpp"
#include "jsf.hpp"
#include "klimov_shamir.hpp"
#include "lcg.hpp"
#include "lea64.hpp"
#include "lehmer64.hpp"
#include "lxm.hpp"
#include "mcg.hpp"
#include "moremur.hpp"
#include "msws.hpp"
#include "mumx_mumx_rrxx_1.hpp"
#include "mumx_mumx_x1.hpp"
#include "mumx_mumx_x2.hpp"
#include "murmurhash.hpp"
#include "mx3.hpp"
#include "nasam.hpp"
#include "pcg.hpp"
#include "romu.hpp"
#include "rrma2xsm2xs.hpp"
#include "rrmxmx.hpp"
#include "rrxmrrxmsx_0.hpp"
#include "seiran.hpp"
#include "sfc.hpp"
#include "sha1_ctr.hpp"
#include "sha256_ctr.hpp"
#include "shioi.hpp"
#include "splitmix.hpp"
#include "splitxix33.hpp"
#include "squares.hpp"
#include "stc_crand.hpp"
#include "ttwanghash64.hpp"
#include "wyrand.hpp"
#include "xoroshiro.hpp"
#include "xoshiro.hpp"
#include "xsm.hpp"
#include "xxh.hpp"

#include <climits>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <map>
#include <random>
#include <string>

template <std::uniform_random_bit_generator URBG>
constexpr size_t
get_state_size_bytes()
{
    constexpr bool has_state_type = requires(const URBG& prng)
    {
        typename URBG::state_type;
    };

    if constexpr (has_state_type)
        return sizeof(typename URBG::state_type);
    else
        return sizeof(URBG);
}

struct prng_info_t
{
    const size_t state_size_bytes;
    const size_t result_size_bits; // This has to be bits because of how RNG_test works.
#if defined(__SIZEOF_INT128__)
    const __uint128_t result_min;
    const __uint128_t result_max;
#else
    const uintmax_t result_min;
    const uintmax_t result_max;
#endif
};

template <std::uniform_random_bit_generator URBG>
prng_info_t
create_prng_info()
{
    return prng_info_t{
        .state_size_bytes = get_state_size_bytes<URBG>(),
        .result_size_bits = sizeof(typename URBG::result_type) * CHAR_BIT,
        .result_min = URBG::min(),
        .result_max = URBG::max(),
    };
}

#define CREATE_PRNG_INFO_MAP_ENTRY(NAME) {#NAME, create_prng_info<NAME>()}

const std::map<std::string, prng_info_t> prng_name_to_info{
    // <random>
    CREATE_PRNG_INFO_MAP_ENTRY(std::default_random_engine),
    CREATE_PRNG_INFO_MAP_ENTRY(std::knuth_b              ),
    CREATE_PRNG_INFO_MAP_ENTRY(std::minstd_rand          ),
    CREATE_PRNG_INFO_MAP_ENTRY(std::minstd_rand0         ),
    CREATE_PRNG_INFO_MAP_ENTRY(std::mt19937              ),
    CREATE_PRNG_INFO_MAP_ENTRY(std::mt19937_64           ),
    CREATE_PRNG_INFO_MAP_ENTRY(std::ranlux24             ),
    CREATE_PRNG_INFO_MAP_ENTRY(std::ranlux24_base        ),
    CREATE_PRNG_INFO_MAP_ENTRY(std::ranlux48             ),
    CREATE_PRNG_INFO_MAP_ENTRY(std::ranlux48_base        ),

    // mine
#if defined(__AES__)
    CREATE_PRNG_INFO_MAP_ENTRY(aes_compress_ctr2_128 ),
    CREATE_PRNG_INFO_MAP_ENTRY(aes_ctr_128           ),
#endif
    CREATE_PRNG_INFO_MAP_ENTRY(biski64               ),
#if defined(__PCLMUL__)
    CREATE_PRNG_INFO_MAP_ENTRY(clmulrand             ),
#endif
    CREATE_PRNG_INFO_MAP_ENTRY(degski32              ),
    CREATE_PRNG_INFO_MAP_ENTRY(degski64              ),
    CREATE_PRNG_INFO_MAP_ENTRY(ettinger_mixer        ),
    CREATE_PRNG_INFO_MAP_ENTRY(gjrand                ),
    CREATE_PRNG_INFO_MAP_ENTRY(jsf32_2               ),
    CREATE_PRNG_INFO_MAP_ENTRY(jsf32_3               ),
    CREATE_PRNG_INFO_MAP_ENTRY(jsf64                 ),
    CREATE_PRNG_INFO_MAP_ENTRY(klimov_shamir_32      ),
    CREATE_PRNG_INFO_MAP_ENTRY(lcg32                 ),
    CREATE_PRNG_INFO_MAP_ENTRY(lcg64                 ),
    CREATE_PRNG_INFO_MAP_ENTRY(lea64                 ),
    CREATE_PRNG_INFO_MAP_ENTRY(lehmer64              ),
    CREATE_PRNG_INFO_MAP_ENTRY(lxm                   ),
    CREATE_PRNG_INFO_MAP_ENTRY(mcg128                ),
    CREATE_PRNG_INFO_MAP_ENTRY(moremur               ),
    CREATE_PRNG_INFO_MAP_ENTRY(MRG32k3a              ),
    CREATE_PRNG_INFO_MAP_ENTRY(msws32                ),
    CREATE_PRNG_INFO_MAP_ENTRY(msws64                ),
    CREATE_PRNG_INFO_MAP_ENTRY(mumx_mumx_rrxx_1      ),
    CREATE_PRNG_INFO_MAP_ENTRY(mumx_mumx_x1          ),
    CREATE_PRNG_INFO_MAP_ENTRY(mumx_mumx_x2          ),
    CREATE_PRNG_INFO_MAP_ENTRY(murmurhash3           ),
    CREATE_PRNG_INFO_MAP_ENTRY(murmurhash3_32        ),
    CREATE_PRNG_INFO_MAP_ENTRY(mx3                   ),
    CREATE_PRNG_INFO_MAP_ENTRY(nasam                 ),
    CREATE_PRNG_INFO_MAP_ENTRY(pcg32                 ),
    CREATE_PRNG_INFO_MAP_ENTRY(pcg32_fast            ),
    CREATE_PRNG_INFO_MAP_ENTRY(pcg64                 ),
    CREATE_PRNG_INFO_MAP_ENTRY(pcg64dxsm             ),
    CREATE_PRNG_INFO_MAP_ENTRY(romu_duo              ),
    CREATE_PRNG_INFO_MAP_ENTRY(romu_duo_jr           ),
    CREATE_PRNG_INFO_MAP_ENTRY(romu_quad             ),
    CREATE_PRNG_INFO_MAP_ENTRY(romu_quad32           ),
    CREATE_PRNG_INFO_MAP_ENTRY(romu_trio             ),
    CREATE_PRNG_INFO_MAP_ENTRY(romu_trio32           ),
    CREATE_PRNG_INFO_MAP_ENTRY(rrma2xsm2xs           ),
    CREATE_PRNG_INFO_MAP_ENTRY(rrmxmx                ),
    CREATE_PRNG_INFO_MAP_ENTRY(rrxmrrxmsx_0          ),
    CREATE_PRNG_INFO_MAP_ENTRY(seiran                ),
    CREATE_PRNG_INFO_MAP_ENTRY(sfc32                 ),
    CREATE_PRNG_INFO_MAP_ENTRY(sfc64                 ),
#if defined(__SHA__)
    CREATE_PRNG_INFO_MAP_ENTRY(sha1_ctr_64           ),
    CREATE_PRNG_INFO_MAP_ENTRY(sha1_ctr_128          ),
    CREATE_PRNG_INFO_MAP_ENTRY(sha256_ctr_64         ),
    CREATE_PRNG_INFO_MAP_ENTRY(sha256_ctr_128        ),
#endif
    CREATE_PRNG_INFO_MAP_ENTRY(shioi                 ),
    CREATE_PRNG_INFO_MAP_ENTRY(splitmix32            ),
    CREATE_PRNG_INFO_MAP_ENTRY(splitmix64            ),
    CREATE_PRNG_INFO_MAP_ENTRY(splitxix33            ),
    CREATE_PRNG_INFO_MAP_ENTRY(squares32             ),
    CREATE_PRNG_INFO_MAP_ENTRY(squares64             ),
    CREATE_PRNG_INFO_MAP_ENTRY(stc_crand32           ),
    CREATE_PRNG_INFO_MAP_ENTRY(stc_crand64           ),
    CREATE_PRNG_INFO_MAP_ENTRY(ttwanghash64          ),
    CREATE_PRNG_INFO_MAP_ENTRY(wyrand                ),
    CREATE_PRNG_INFO_MAP_ENTRY(xoroshiro64starstar   ),
    CREATE_PRNG_INFO_MAP_ENTRY(xoroshiro128plusplus  ),
    CREATE_PRNG_INFO_MAP_ENTRY(xoroshiro128starstar  ),
    CREATE_PRNG_INFO_MAP_ENTRY(xoroshiro1024plusplus ),
    CREATE_PRNG_INFO_MAP_ENTRY(xoroshiro1024starstar ),
    CREATE_PRNG_INFO_MAP_ENTRY(xoshiro128plusplus    ),
    CREATE_PRNG_INFO_MAP_ENTRY(xoshiro128starstar    ),
    CREATE_PRNG_INFO_MAP_ENTRY(xoshiro256plusplus    ),
    CREATE_PRNG_INFO_MAP_ENTRY(xoshiro256starstar    ),
    CREATE_PRNG_INFO_MAP_ENTRY(xoshiro512plusplus    ),
    CREATE_PRNG_INFO_MAP_ENTRY(xoshiro512starstar    ),
    CREATE_PRNG_INFO_MAP_ENTRY(xsm32                 ),
    CREATE_PRNG_INFO_MAP_ENTRY(xsm64                 ),
    CREATE_PRNG_INFO_MAP_ENTRY(xxh32_avalanche       ),
    CREATE_PRNG_INFO_MAP_ENTRY(xxh64_avalanche       ),
    CREATE_PRNG_INFO_MAP_ENTRY(xxh3_avalanche        ),
    CREATE_PRNG_INFO_MAP_ENTRY(xxh3_rrmxmx           ),
};
