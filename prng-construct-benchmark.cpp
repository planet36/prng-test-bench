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
BM_prng_construct_std(benchmark::State& BM_state)
{
    // Perform setup here

    URBG gen{}; // NOLINT(bugprone-random-generator-seed,cert-msc32-c,cert-msc51-cpp)

    for (auto _ : BM_state) // NOLINT(clang-analyzer-deadcode.DeadStores)
    {
        // This code gets timed

        gen = random_device_seeded<URBG>();
    }

    // This is to prevent the compiler from eliding the work above.
    benchmark::DoNotOptimize(gen);
}

/// Benchmark construction of my PRNGs
template <typename URBG>
requires std::uniform_random_bit_generator<std::remove_cvref_t<URBG>>
void
BM_prng_construct_my(benchmark::State& BM_state)
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

#define REGISTER_BENCHMARK_PRNG_CONSTRUCT_STD(NAME) \
{ auto* benchmark = benchmark::RegisterBenchmark(#NAME, BM_prng_construct_std<NAME>); \
if (num_threads > 1) benchmark->Threads(num_threads); }


#define REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(NAME) \
{ auto* benchmark = benchmark::RegisterBenchmark(#NAME, BM_prng_construct_my<NAME>); \
if (num_threads > 1) benchmark->Threads(num_threads); }

    // <random>
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_STD(std::default_random_engine)
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_STD(std::knuth_b              )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_STD(std::minstd_rand          )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_STD(std::minstd_rand0         )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_STD(std::mt19937              )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_STD(std::mt19937_64           )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_STD(std::philox4x32           )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_STD(std::philox4x64           )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_STD(std::ranlux24             )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_STD(std::ranlux24_base        )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_STD(std::ranlux48             )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_STD(std::ranlux48_base        )

    // mine
#if defined(__AES__)
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(aes_compress_ctr2_128 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(aes_ctr_128           )
#endif
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(biski64               )
#if defined(__PCLMUL__)
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(clmulrand             )
#endif
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(degski32              )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(degski64              )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(ettinger_mixer        )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(gjrand                )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(jsf32_2               )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(jsf32_3               )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(jsf64                 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(klimov_shamir_32      )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(lcg32                 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(lcg64                 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(lea64                 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(lehmer64              )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(lxm                   )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(mcg128                )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(moremur               )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(msws32                )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(msws64                )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(mumx_ctr2             )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(mumx_mumx_rrxx_1      )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(mumx_mumx_x1          )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(mumx_mumx_x2          )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(murmurhash3           )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(murmurhash3_32        )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(mx3                   )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(nasam                 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(pcg32                 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(pcg32_fast            )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(pcg64                 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(pcg64dxsm             )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(romu_duo              )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(romu_duo_jr           )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(romu_quad             )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(romu_quad32           )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(romu_trio             )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(romu_trio32           )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(rrma2xsm2xs           )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(rrmxmx                )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(rrxmrrxmsx_0          )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(seiran                )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(sfc32                 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(sfc64                 )
#if defined(__SHA__)
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(sha1_ctr_128          )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(sha256_ctr_128        )
#endif
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(shioi                 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(splitmix32            )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(splitmix64            )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(splitxix33            )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(squares32             )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(squares64             )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(staffordMix13         )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(stc_crand32           )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(stc_crand64           )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(ttwanghash64          )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(wyrand                )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(xoroshiro64starstar   )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(xoroshiro128plusplus  )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(xoroshiro128starstar  )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(xoroshiro1024plusplus )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(xoroshiro1024starstar )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(xoroshiro128aox       )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(xoshiro128plusplus    )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(xoshiro128starstar    )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(xoshiro256plusplus    )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(xoshiro256starstar    )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(xoshiro512plusplus    )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(xoshiro512starstar    )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(xsm32                 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(xsm64                 )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(xxh32_avalanche       )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(xxh64_avalanche       )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(xxh3_avalanche        )
    REGISTER_BENCHMARK_PRNG_CONSTRUCT_MY(xxh3_rrmxmx           )

    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();

    // }}}

    return 0;
}
