// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

#include "parse_int.hpp"
#include "prng.hpp"
#include "seed_seq.hpp"

#include <algorithm>
#include <benchmark/benchmark.h> // https://github.com/google/benchmark
#include <cstdlib>
#include <err.h>
#include <exception>
#include <thread>

/// Benchmark the generation of next values from standard PRNGs
template <typename URBG>
requires std::uniform_random_bit_generator<std::remove_cvref_t<URBG>>
void
BM_std_urbg_next(benchmark::State& BM_state)
{
    using result_type = std::remove_cvref_t<URBG>::result_type;

    // Perform setup here

    URBG gen = random_device_seeded<URBG>();

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

/// Benchmark the generation of next values from my PRNGs
template <typename URBG>
requires std::uniform_random_bit_generator<std::remove_cvref_t<URBG>>
void
BM_my_urbg_next(benchmark::State& BM_state)
{
    using result_type = std::remove_cvref_t<URBG>::result_type;

    // Perform setup here

    URBG gen{};

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
        return 1;

    // {{{ determine num_threads

    constexpr int min_threads = 1;
    const auto hw_threads = static_cast<int>(std::thread::hardware_concurrency());
    const auto max_threads = std::max(min_threads, hw_threads);

    // NUM_THREADS=0 means max_threads
    int num_threads = min_threads;

    try
    {
        num_threads = parse_env_int("NUM_THREADS", 0, max_threads, min_threads);
    }
    catch (const std::exception& ex)
    {
        (void)std::fflush(stdout);
        errx(EXIT_FAILURE, "%s", ex.what());
    }

    if (num_threads == 0)
        num_threads = max_threads;

    // }}}

    // {{{ speed

#define REGISTER_BENCHMARK_STD_NEXT(NAME) \
{ auto* benchmark = benchmark::RegisterBenchmark(#NAME, BM_std_urbg_next<NAME>); \
if (num_threads > 1) benchmark->Threads(num_threads); }

#define REGISTER_BENCHMARK_MY_NEXT(NAME) \
{ auto* benchmark = benchmark::RegisterBenchmark(#NAME, BM_my_urbg_next<NAME>); \
if (num_threads > 1) benchmark->Threads(num_threads); }

    // <random>
    REGISTER_BENCHMARK_STD_NEXT(std::default_random_engine)
    REGISTER_BENCHMARK_STD_NEXT(std::knuth_b              )
    REGISTER_BENCHMARK_STD_NEXT(std::minstd_rand          )
    REGISTER_BENCHMARK_STD_NEXT(std::minstd_rand0         )
    REGISTER_BENCHMARK_STD_NEXT(std::mt19937              )
    REGISTER_BENCHMARK_STD_NEXT(std::mt19937_64           )
    REGISTER_BENCHMARK_STD_NEXT(std::philox4x32           )
    REGISTER_BENCHMARK_STD_NEXT(std::philox4x64           )
    REGISTER_BENCHMARK_STD_NEXT(std::ranlux24             )
    REGISTER_BENCHMARK_STD_NEXT(std::ranlux24_base        )
    REGISTER_BENCHMARK_STD_NEXT(std::ranlux48             )
    REGISTER_BENCHMARK_STD_NEXT(std::ranlux48_base        )

    // mine
#if defined(__AES__)
    REGISTER_BENCHMARK_MY_NEXT(aes_compress_ctr2_128 )
    REGISTER_BENCHMARK_MY_NEXT(aes_ctr_128           )
#endif
    REGISTER_BENCHMARK_MY_NEXT(biski64               )
#if defined(__PCLMUL__)
    REGISTER_BENCHMARK_MY_NEXT(clmulrand             )
#endif
    REGISTER_BENCHMARK_MY_NEXT(degski32              )
    REGISTER_BENCHMARK_MY_NEXT(degski64              )
    REGISTER_BENCHMARK_MY_NEXT(ettinger_mixer        )
    REGISTER_BENCHMARK_MY_NEXT(gjrand                )
    REGISTER_BENCHMARK_MY_NEXT(jsf32_2               )
    REGISTER_BENCHMARK_MY_NEXT(jsf32_3               )
    REGISTER_BENCHMARK_MY_NEXT(jsf64                 )
    REGISTER_BENCHMARK_MY_NEXT(klimov_shamir_32      )
    REGISTER_BENCHMARK_MY_NEXT(lcg32                 )
    REGISTER_BENCHMARK_MY_NEXT(lcg64                 )
    REGISTER_BENCHMARK_MY_NEXT(lea64                 )
    REGISTER_BENCHMARK_MY_NEXT(lehmer64              )
    REGISTER_BENCHMARK_MY_NEXT(lxm                   )
    REGISTER_BENCHMARK_MY_NEXT(mcg128                )
    REGISTER_BENCHMARK_MY_NEXT(moremur               )
    REGISTER_BENCHMARK_MY_NEXT(msws32                )
    REGISTER_BENCHMARK_MY_NEXT(msws64                )
    REGISTER_BENCHMARK_MY_NEXT(mumx_ctr2             )
    REGISTER_BENCHMARK_MY_NEXT(mumx_mumx_rrxx_1      )
    REGISTER_BENCHMARK_MY_NEXT(mumx_mumx_x1          )
    REGISTER_BENCHMARK_MY_NEXT(mumx_mumx_x2          )
    REGISTER_BENCHMARK_MY_NEXT(murmurhash3           )
    REGISTER_BENCHMARK_MY_NEXT(murmurhash3_32        )
    REGISTER_BENCHMARK_MY_NEXT(mx3                   )
    REGISTER_BENCHMARK_MY_NEXT(nasam                 )
    REGISTER_BENCHMARK_MY_NEXT(pcg32                 )
    REGISTER_BENCHMARK_MY_NEXT(pcg32_fast            )
    REGISTER_BENCHMARK_MY_NEXT(pcg64                 )
    REGISTER_BENCHMARK_MY_NEXT(pcg64dxsm             )
    REGISTER_BENCHMARK_MY_NEXT(romu_duo              )
    REGISTER_BENCHMARK_MY_NEXT(romu_duo_jr           )
    REGISTER_BENCHMARK_MY_NEXT(romu_quad             )
    REGISTER_BENCHMARK_MY_NEXT(romu_quad32           )
    REGISTER_BENCHMARK_MY_NEXT(romu_trio             )
    REGISTER_BENCHMARK_MY_NEXT(romu_trio32           )
    REGISTER_BENCHMARK_MY_NEXT(rrma2xsm2xs           )
    REGISTER_BENCHMARK_MY_NEXT(rrmxmx                )
    REGISTER_BENCHMARK_MY_NEXT(rrxmrrxmsx_0          )
    REGISTER_BENCHMARK_MY_NEXT(seiran                )
    REGISTER_BENCHMARK_MY_NEXT(sfc32                 )
    REGISTER_BENCHMARK_MY_NEXT(sfc64                 )
#if defined(__SHA__)
    REGISTER_BENCHMARK_MY_NEXT(sha1_ctr_128          )
    REGISTER_BENCHMARK_MY_NEXT(sha256_ctr_128        )
#endif
    REGISTER_BENCHMARK_MY_NEXT(shioi                 )
    REGISTER_BENCHMARK_MY_NEXT(splitmix32            )
    REGISTER_BENCHMARK_MY_NEXT(splitmix64            )
    REGISTER_BENCHMARK_MY_NEXT(splitxix33            )
    REGISTER_BENCHMARK_MY_NEXT(squares32             )
    REGISTER_BENCHMARK_MY_NEXT(squares64             )
    REGISTER_BENCHMARK_MY_NEXT(staffordMix13         )
    REGISTER_BENCHMARK_MY_NEXT(stc_crand32           )
    REGISTER_BENCHMARK_MY_NEXT(stc_crand64           )
    REGISTER_BENCHMARK_MY_NEXT(ttwanghash64          )
    REGISTER_BENCHMARK_MY_NEXT(wyrand                )
    REGISTER_BENCHMARK_MY_NEXT(xoroshiro64starstar   )
    REGISTER_BENCHMARK_MY_NEXT(xoroshiro128plusplus  )
    REGISTER_BENCHMARK_MY_NEXT(xoroshiro128starstar  )
    REGISTER_BENCHMARK_MY_NEXT(xoroshiro1024plusplus )
    REGISTER_BENCHMARK_MY_NEXT(xoroshiro1024starstar )
    REGISTER_BENCHMARK_MY_NEXT(xoroshiro128aox       )
    REGISTER_BENCHMARK_MY_NEXT(xoshiro128plusplus    )
    REGISTER_BENCHMARK_MY_NEXT(xoshiro128starstar    )
    REGISTER_BENCHMARK_MY_NEXT(xoshiro256plusplus    )
    REGISTER_BENCHMARK_MY_NEXT(xoshiro256starstar    )
    REGISTER_BENCHMARK_MY_NEXT(xoshiro512plusplus    )
    REGISTER_BENCHMARK_MY_NEXT(xoshiro512starstar    )
    REGISTER_BENCHMARK_MY_NEXT(xsm32                 )
    REGISTER_BENCHMARK_MY_NEXT(xsm64                 )
    REGISTER_BENCHMARK_MY_NEXT(xxh32_avalanche       )
    REGISTER_BENCHMARK_MY_NEXT(xxh64_avalanche       )
    REGISTER_BENCHMARK_MY_NEXT(xxh3_avalanche        )
    REGISTER_BENCHMARK_MY_NEXT(xxh3_rrmxmx           )

    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();

    // }}}

    return 0;
}
