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

/// Benchmark construction of the standard PRNGs
template <typename URBG>
requires std::uniform_random_bit_generator<std::remove_cvref_t<URBG>>
void
BM_std_urbg_construct(benchmark::State& BM_state)
{
    // Perform setup here

    URBG gen{};

    for (auto _ : BM_state) // NOLINT(clang-analyzer-deadcode.DeadStores)
    {
        // This code gets timed

        gen = random_device_seeded<URBG>();
    }

    // This is to prevent the compiler from eliding the work above.
    benchmark::DoNotOptimize(gen);
}

/// Benchmark construction of the my PRNGs
template <typename URBG>
requires std::uniform_random_bit_generator<std::remove_cvref_t<URBG>>
void
BM_my_urbg_construct(benchmark::State& BM_state)
{
    // Perform setup here

    URBG gen{};

    for (auto _ : BM_state) // NOLINT(clang-analyzer-deadcode.DeadStores)
    {
        // This code gets timed

        gen = URBG{};
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

#define REGISTER_BENCHMARK_STD_CONSTRUCT(NAME) \
{ auto* benchmark = benchmark::RegisterBenchmark(#NAME, BM_std_urbg_construct<NAME>); \
if (num_threads > 1) benchmark->Threads(num_threads); }


#define REGISTER_BENCHMARK_MY_CONSTRUCT(NAME) \
{ auto* benchmark = benchmark::RegisterBenchmark(#NAME, BM_my_urbg_construct<NAME>); \
if (num_threads > 1) benchmark->Threads(num_threads); }

    // <random>
    REGISTER_BENCHMARK_STD_CONSTRUCT(std::default_random_engine)
    REGISTER_BENCHMARK_STD_CONSTRUCT(std::knuth_b              )
    REGISTER_BENCHMARK_STD_CONSTRUCT(std::minstd_rand          )
    REGISTER_BENCHMARK_STD_CONSTRUCT(std::minstd_rand0         )
    REGISTER_BENCHMARK_STD_CONSTRUCT(std::mt19937              )
    REGISTER_BENCHMARK_STD_CONSTRUCT(std::mt19937_64           )
    REGISTER_BENCHMARK_STD_CONSTRUCT(std::philox4x32           )
    REGISTER_BENCHMARK_STD_CONSTRUCT(std::philox4x64           )
    REGISTER_BENCHMARK_STD_CONSTRUCT(std::ranlux24             )
    REGISTER_BENCHMARK_STD_CONSTRUCT(std::ranlux24_base        )
    REGISTER_BENCHMARK_STD_CONSTRUCT(std::ranlux48             )
    REGISTER_BENCHMARK_STD_CONSTRUCT(std::ranlux48_base        )

    // mine
#if defined(__AES__)
    REGISTER_BENCHMARK_MY_CONSTRUCT(aes_compress_ctr2_128 )
    REGISTER_BENCHMARK_MY_CONSTRUCT(aes_ctr_128           )
#endif
    REGISTER_BENCHMARK_MY_CONSTRUCT(biski64               )
#if defined(__PCLMUL__)
    REGISTER_BENCHMARK_MY_CONSTRUCT(clmulrand             )
#endif
    REGISTER_BENCHMARK_MY_CONSTRUCT(degski32              )
    REGISTER_BENCHMARK_MY_CONSTRUCT(degski64              )
    REGISTER_BENCHMARK_MY_CONSTRUCT(ettinger_mixer        )
    REGISTER_BENCHMARK_MY_CONSTRUCT(gjrand                )
    REGISTER_BENCHMARK_MY_CONSTRUCT(jsf32_2               )
    REGISTER_BENCHMARK_MY_CONSTRUCT(jsf32_3               )
    REGISTER_BENCHMARK_MY_CONSTRUCT(jsf64                 )
    REGISTER_BENCHMARK_MY_CONSTRUCT(klimov_shamir_32      )
    REGISTER_BENCHMARK_MY_CONSTRUCT(lcg32                 )
    REGISTER_BENCHMARK_MY_CONSTRUCT(lcg64                 )
    REGISTER_BENCHMARK_MY_CONSTRUCT(lea64                 )
    REGISTER_BENCHMARK_MY_CONSTRUCT(lehmer64              )
    REGISTER_BENCHMARK_MY_CONSTRUCT(lxm                   )
    REGISTER_BENCHMARK_MY_CONSTRUCT(mcg128                )
    REGISTER_BENCHMARK_MY_CONSTRUCT(moremur               )
    REGISTER_BENCHMARK_MY_CONSTRUCT(msws32                )
    REGISTER_BENCHMARK_MY_CONSTRUCT(msws64                )
    REGISTER_BENCHMARK_MY_CONSTRUCT(mumx_ctr2             )
    REGISTER_BENCHMARK_MY_CONSTRUCT(mumx_mumx_rrxx_1      )
    REGISTER_BENCHMARK_MY_CONSTRUCT(mumx_mumx_x1          )
    REGISTER_BENCHMARK_MY_CONSTRUCT(mumx_mumx_x2          )
    REGISTER_BENCHMARK_MY_CONSTRUCT(murmurhash3           )
    REGISTER_BENCHMARK_MY_CONSTRUCT(murmurhash3_32        )
    REGISTER_BENCHMARK_MY_CONSTRUCT(mx3                   )
    REGISTER_BENCHMARK_MY_CONSTRUCT(nasam                 )
    REGISTER_BENCHMARK_MY_CONSTRUCT(pcg32                 )
    REGISTER_BENCHMARK_MY_CONSTRUCT(pcg32_fast            )
    REGISTER_BENCHMARK_MY_CONSTRUCT(pcg64                 )
    REGISTER_BENCHMARK_MY_CONSTRUCT(pcg64dxsm             )
    REGISTER_BENCHMARK_MY_CONSTRUCT(romu_duo              )
    REGISTER_BENCHMARK_MY_CONSTRUCT(romu_duo_jr           )
    REGISTER_BENCHMARK_MY_CONSTRUCT(romu_quad             )
    REGISTER_BENCHMARK_MY_CONSTRUCT(romu_quad32           )
    REGISTER_BENCHMARK_MY_CONSTRUCT(romu_trio             )
    REGISTER_BENCHMARK_MY_CONSTRUCT(romu_trio32           )
    REGISTER_BENCHMARK_MY_CONSTRUCT(rrma2xsm2xs           )
    REGISTER_BENCHMARK_MY_CONSTRUCT(rrmxmx                )
    REGISTER_BENCHMARK_MY_CONSTRUCT(rrxmrrxmsx_0          )
    REGISTER_BENCHMARK_MY_CONSTRUCT(seiran                )
    REGISTER_BENCHMARK_MY_CONSTRUCT(sfc32                 )
    REGISTER_BENCHMARK_MY_CONSTRUCT(sfc64                 )
#if defined(__SHA__)
    REGISTER_BENCHMARK_MY_CONSTRUCT(sha1_ctr_128          )
    REGISTER_BENCHMARK_MY_CONSTRUCT(sha256_ctr_128        )
#endif
    REGISTER_BENCHMARK_MY_CONSTRUCT(shioi                 )
    REGISTER_BENCHMARK_MY_CONSTRUCT(splitmix32            )
    REGISTER_BENCHMARK_MY_CONSTRUCT(splitmix64            )
    REGISTER_BENCHMARK_MY_CONSTRUCT(splitxix33            )
    REGISTER_BENCHMARK_MY_CONSTRUCT(squares32             )
    REGISTER_BENCHMARK_MY_CONSTRUCT(squares64             )
    REGISTER_BENCHMARK_MY_CONSTRUCT(staffordMix13         )
    REGISTER_BENCHMARK_MY_CONSTRUCT(stc_crand32           )
    REGISTER_BENCHMARK_MY_CONSTRUCT(stc_crand64           )
    REGISTER_BENCHMARK_MY_CONSTRUCT(ttwanghash64          )
    REGISTER_BENCHMARK_MY_CONSTRUCT(wyrand                )
    REGISTER_BENCHMARK_MY_CONSTRUCT(xoroshiro64starstar   )
    REGISTER_BENCHMARK_MY_CONSTRUCT(xoroshiro128plusplus  )
    REGISTER_BENCHMARK_MY_CONSTRUCT(xoroshiro128starstar  )
    REGISTER_BENCHMARK_MY_CONSTRUCT(xoroshiro1024plusplus )
    REGISTER_BENCHMARK_MY_CONSTRUCT(xoroshiro1024starstar )
    REGISTER_BENCHMARK_MY_CONSTRUCT(xoroshiro128aox       )
    REGISTER_BENCHMARK_MY_CONSTRUCT(xoshiro128plusplus    )
    REGISTER_BENCHMARK_MY_CONSTRUCT(xoshiro128starstar    )
    REGISTER_BENCHMARK_MY_CONSTRUCT(xoshiro256plusplus    )
    REGISTER_BENCHMARK_MY_CONSTRUCT(xoshiro256starstar    )
    REGISTER_BENCHMARK_MY_CONSTRUCT(xoshiro512plusplus    )
    REGISTER_BENCHMARK_MY_CONSTRUCT(xoshiro512starstar    )
    REGISTER_BENCHMARK_MY_CONSTRUCT(xsm32                 )
    REGISTER_BENCHMARK_MY_CONSTRUCT(xsm64                 )
    REGISTER_BENCHMARK_MY_CONSTRUCT(xxh32_avalanche       )
    REGISTER_BENCHMARK_MY_CONSTRUCT(xxh64_avalanche       )
    REGISTER_BENCHMARK_MY_CONSTRUCT(xxh3_avalanche        )
    REGISTER_BENCHMARK_MY_CONSTRUCT(xxh3_rrmxmx           )

    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();

    // }}}

    return 0;
}
