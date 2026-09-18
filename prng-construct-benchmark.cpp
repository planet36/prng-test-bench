// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

#include "prng-benchmark.hpp"
#include "prng.hpp"

#include <benchmark/benchmark.h> // https://github.com/google/benchmark
#include <random>
#include <type_traits>

/// Benchmark construction of a randomly seeded PRNG
template <typename URBG>
requires std::uniform_random_bit_generator<std::remove_cvref_t<URBG>>
void
BM_prng_construct(benchmark::State& BM_state)
{
    // Perform setup here

    URBG gen = make_random_seeded<URBG>();

    for (auto _ : BM_state) // NOLINT(clang-analyzer-deadcode.DeadStores)
    {
        // This code gets timed

        gen = make_random_seeded<URBG>();
    }

    // This is to prevent the compiler from eliding the work above.
    benchmark::DoNotOptimize(gen);
}

// NOLINTNEXTLINE(bugprone-exception-escape)
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    using namespace std::literals;

    // Copied from benchmark.h
    benchmark::MaybeReenterWithoutASLR(argc, argv);
    benchmark::Initialize(&argc, argv);

    if (benchmark::ReportUnrecognizedArguments(argc, argv))
    {
        return 1;
    }

    const int num_threads = get_num_threads();

    // {{{ speed

#define REGISTER_BENCHMARK_PRNG_CONSTRUCT(NAME) \
{ auto* benchmark = benchmark::RegisterBenchmark(#NAME, BM_prng_construct<NAME>); \
if (num_threads > 1) benchmark->Threads(num_threads); }

    // <random>
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(std::default_random_engine)
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(std::knuth_b              )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(std::minstd_rand          )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(std::minstd_rand0         )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(std::mt19937              )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(std::mt19937_64           )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(std::philox4x32           )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(std::philox4x64           )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(std::ranlux24             )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(std::ranlux24_base        )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(std::ranlux48             )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(std::ranlux48_base        )

    // mine
#if defined(__AES__)
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(aes_compress_ctr2_128 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(aes_ctr_128           )
#endif
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(biski64               )
#if defined(__PCLMUL__)
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(clmulrand             )
#endif
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(degski32              )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(degski64              )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(ettinger_mixer        )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(gjrand                )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(jsf32_2               )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(jsf32_3               )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(jsf64                 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(klimov_shamir_32      )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(lcg32                 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(lcg64                 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(lea64                 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(lehmer64              )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(lxm                   )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(mcg128                )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(moremur               )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(msws32                )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(msws64                )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(mumx_ctr2             )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(mumx_mumx_rrxx_1      )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(mumx_mumx_x1          )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(mumx_mumx_x2          )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(murmurhash3           )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(murmurhash3_32        )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(mx3                   )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(nasam                 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(pcg32                 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(pcg32_fast            )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(pcg64                 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(pcg64dxsm             )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(romu_duo              )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(romu_duo_jr           )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(romu_quad             )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(romu_quad32           )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(romu_trio             )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(romu_trio32           )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(rrma2xsm2xs           )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(rrmxmx                )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(rrxmrrxmsx_0          )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(seiran                )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(sfc32                 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(sfc64                 )
#if defined(__SHA__)
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(sha1_ctr_128          )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(sha256_ctr_128        )
#endif
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(shioi                 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(splitmix32            )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(splitmix64            )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(splitxix33            )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(squares32             )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(squares64             )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(staffordMix13         )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(stc_crand32           )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(stc_crand64           )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(ttwanghash64          )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(wyrand                )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(xoroshiro64starstar   )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(xoroshiro128plusplus  )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(xoroshiro128starstar  )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(xoroshiro1024plusplus )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(xoroshiro1024starstar )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(xoroshiro128aox       )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(xoshiro128plusplus    )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(xoshiro128starstar    )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(xoshiro256plusplus    )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(xoshiro256starstar    )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(xoshiro512plusplus    )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(xoshiro512starstar    )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(xsm32                 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(xsm64                 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(xxh32_avalanche       )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(xxh64_avalanche       )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(xxh3_avalanche        )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT(xxh3_rrmxmx           )

    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();

    // }}}

    return 0;
}
