// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

#include "get_num_threads.hpp"
#include "prng-benchmark.hpp"
#include "prng.hpp"

#include <benchmark/benchmark.h> // https://github.com/google/benchmark
#include <random>
#include <type_traits>

/// Benchmark the generation of next values from a PRNG
template <typename URBG>
requires std::uniform_random_bit_generator<std::remove_cvref_t<URBG>>
void
BM_prng_next(benchmark::State& BM_state)
{
    using result_type = std::remove_cvref_t<URBG>::result_type;

    // Perform setup here

    URBG gen = make_random_seeded<URBG>();

    volatile result_type result{};

    for (auto _ : BM_state) // NOLINT(clang-analyzer-deadcode.DeadStores)
    {
        // This code gets timed

        result = gen();
    }

    // This is to prevent the compiler from eliding the work above.
    benchmark::DoNotOptimize(result);

    // Counters are summed across threads.  kAvgThreads makes this the per-thread rate.
    BM_state.counters["bytes_per_second"] =
        benchmark::Counter(static_cast<double>(BM_state.iterations()) * sizeof(result_type),
                           benchmark::Counter::kAvgThreadsRate, benchmark::Counter::kIs1024);
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

#define REGISTER_BENCHMARK_PRNG_NEXT(NAME) \
{ auto* benchmark = benchmark::RegisterBenchmark(#NAME, BM_prng_next<NAME>); \
if (num_threads > 1) benchmark->Threads(num_threads); }

    // <random>
    REGISTER_BENCHMARK_PRNG_NEXT(std::default_random_engine)
    REGISTER_BENCHMARK_PRNG_NEXT(std::knuth_b              )
    REGISTER_BENCHMARK_PRNG_NEXT(std::minstd_rand          )
    REGISTER_BENCHMARK_PRNG_NEXT(std::minstd_rand0         )
    REGISTER_BENCHMARK_PRNG_NEXT(std::mt19937              )
    REGISTER_BENCHMARK_PRNG_NEXT(std::mt19937_64           )
    REGISTER_BENCHMARK_PRNG_NEXT(std::philox4x32           )
    REGISTER_BENCHMARK_PRNG_NEXT(std::philox4x64           )
    REGISTER_BENCHMARK_PRNG_NEXT(std::ranlux24             )
    REGISTER_BENCHMARK_PRNG_NEXT(std::ranlux24_base        )
    REGISTER_BENCHMARK_PRNG_NEXT(std::ranlux48             )
    REGISTER_BENCHMARK_PRNG_NEXT(std::ranlux48_base        )

    // mine
#if defined(__AES__)
    REGISTER_BENCHMARK_PRNG_NEXT(aes_compress_ctr2_128 )
    REGISTER_BENCHMARK_PRNG_NEXT(aes_ctr_128           )
#endif
    REGISTER_BENCHMARK_PRNG_NEXT(biski64               )
#if defined(__PCLMUL__)
    REGISTER_BENCHMARK_PRNG_NEXT(clmulrand             )
#endif
    REGISTER_BENCHMARK_PRNG_NEXT(degski32              )
    REGISTER_BENCHMARK_PRNG_NEXT(degski64              )
    REGISTER_BENCHMARK_PRNG_NEXT(ettinger_mixer        )
    REGISTER_BENCHMARK_PRNG_NEXT(gjrand                )
    REGISTER_BENCHMARK_PRNG_NEXT(jsf32_2               )
    REGISTER_BENCHMARK_PRNG_NEXT(jsf32_3               )
    REGISTER_BENCHMARK_PRNG_NEXT(jsf64                 )
    REGISTER_BENCHMARK_PRNG_NEXT(klimov_shamir_32      )
    REGISTER_BENCHMARK_PRNG_NEXT(lcg32                 )
    REGISTER_BENCHMARK_PRNG_NEXT(lcg64                 )
    REGISTER_BENCHMARK_PRNG_NEXT(lea64                 )
    REGISTER_BENCHMARK_PRNG_NEXT(lehmer64              )
    REGISTER_BENCHMARK_PRNG_NEXT(lxm                   )
    REGISTER_BENCHMARK_PRNG_NEXT(mcg128                )
    REGISTER_BENCHMARK_PRNG_NEXT(moremur               )
    REGISTER_BENCHMARK_PRNG_NEXT(msws32                )
    REGISTER_BENCHMARK_PRNG_NEXT(msws64                )
    REGISTER_BENCHMARK_PRNG_NEXT(mumx_ctr2             )
    REGISTER_BENCHMARK_PRNG_NEXT(mumx_mumx_rrxx_1      )
    REGISTER_BENCHMARK_PRNG_NEXT(mumx_mumx_x1          )
    REGISTER_BENCHMARK_PRNG_NEXT(mumx_mumx_x2          )
    REGISTER_BENCHMARK_PRNG_NEXT(murmurhash3           )
    REGISTER_BENCHMARK_PRNG_NEXT(murmurhash3_32        )
    REGISTER_BENCHMARK_PRNG_NEXT(mx3                   )
    REGISTER_BENCHMARK_PRNG_NEXT(nasam                 )
    REGISTER_BENCHMARK_PRNG_NEXT(pcg32                 )
    REGISTER_BENCHMARK_PRNG_NEXT(pcg32_fast            )
    REGISTER_BENCHMARK_PRNG_NEXT(pcg64                 )
    REGISTER_BENCHMARK_PRNG_NEXT(pcg64dxsm             )
    REGISTER_BENCHMARK_PRNG_NEXT(romu_duo              )
    REGISTER_BENCHMARK_PRNG_NEXT(romu_duo_jr           )
    REGISTER_BENCHMARK_PRNG_NEXT(romu_quad             )
    REGISTER_BENCHMARK_PRNG_NEXT(romu_quad32           )
    REGISTER_BENCHMARK_PRNG_NEXT(romu_trio             )
    REGISTER_BENCHMARK_PRNG_NEXT(romu_trio32           )
    REGISTER_BENCHMARK_PRNG_NEXT(rrma2xsm2xs           )
    REGISTER_BENCHMARK_PRNG_NEXT(rrmxmx                )
    REGISTER_BENCHMARK_PRNG_NEXT(rrxmrrxmsx_0          )
    REGISTER_BENCHMARK_PRNG_NEXT(seiran                )
    REGISTER_BENCHMARK_PRNG_NEXT(sfc32                 )
    REGISTER_BENCHMARK_PRNG_NEXT(sfc64                 )
#if defined(__SHA__)
    REGISTER_BENCHMARK_PRNG_NEXT(sha1_ctr_128          )
    REGISTER_BENCHMARK_PRNG_NEXT(sha256_ctr_128        )
#endif
    REGISTER_BENCHMARK_PRNG_NEXT(shioi                 )
    REGISTER_BENCHMARK_PRNG_NEXT(splitmix32            )
    REGISTER_BENCHMARK_PRNG_NEXT(splitmix64            )
    REGISTER_BENCHMARK_PRNG_NEXT(splitxix33            )
    REGISTER_BENCHMARK_PRNG_NEXT(squares32             )
    REGISTER_BENCHMARK_PRNG_NEXT(squares64             )
    REGISTER_BENCHMARK_PRNG_NEXT(staffordMix13         )
    REGISTER_BENCHMARK_PRNG_NEXT(stc_crand32           )
    REGISTER_BENCHMARK_PRNG_NEXT(stc_crand64           )
    REGISTER_BENCHMARK_PRNG_NEXT(ttwanghash64          )
    REGISTER_BENCHMARK_PRNG_NEXT(wyrand                )
    REGISTER_BENCHMARK_PRNG_NEXT(xoroshiro64starstar   )
    REGISTER_BENCHMARK_PRNG_NEXT(xoroshiro128plusplus  )
    REGISTER_BENCHMARK_PRNG_NEXT(xoroshiro128starstar  )
    REGISTER_BENCHMARK_PRNG_NEXT(xoroshiro1024plusplus )
    REGISTER_BENCHMARK_PRNG_NEXT(xoroshiro1024starstar )
    REGISTER_BENCHMARK_PRNG_NEXT(xoroshiro128aox       )
    REGISTER_BENCHMARK_PRNG_NEXT(xoshiro128plusplus    )
    REGISTER_BENCHMARK_PRNG_NEXT(xoshiro128starstar    )
    REGISTER_BENCHMARK_PRNG_NEXT(xoshiro256plusplus    )
    REGISTER_BENCHMARK_PRNG_NEXT(xoshiro256starstar    )
    REGISTER_BENCHMARK_PRNG_NEXT(xoshiro512plusplus    )
    REGISTER_BENCHMARK_PRNG_NEXT(xoshiro512starstar    )
    REGISTER_BENCHMARK_PRNG_NEXT(xsm32                 )
    REGISTER_BENCHMARK_PRNG_NEXT(xsm64                 )
    REGISTER_BENCHMARK_PRNG_NEXT(xxh32_avalanche       )
    REGISTER_BENCHMARK_PRNG_NEXT(xxh64_avalanche       )
    REGISTER_BENCHMARK_PRNG_NEXT(xxh3_avalanche        )
    REGISTER_BENCHMARK_PRNG_NEXT(xxh3_rrmxmx           )

    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();

    // }}}

    return 0;
}
