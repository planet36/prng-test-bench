// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

#include "prng.hpp"
#include "seeds.hpp"

#include <algorithm>
#include <benchmark/benchmark.h>
#include <cstdlib>
#include <fmt/ranges.h>
#include <random>
#include <ranges>
#include <string>
#include <thread>
#include <vector>

const auto BM_prng = [](benchmark::State& state, std::uniform_random_bit_generator auto gen)
{
	using result_type = typename decltype(gen)::result_type;

	for (auto _ : state)
	{
		benchmark::DoNotOptimize(gen());
	}

	state.SetBytesProcessed(state.iterations() * sizeof(result_type));
};

int main(int argc, char** argv)
{
	// copied from /usr/include/benchmark/benchmark.h
	benchmark::Initialize(&argc, argv);

	// PRNG names may be given in argv
	//if (benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;

	// {{{ determine num_threads

	constexpr unsigned int min_threads = 1;
	const unsigned int max_threads = (std::thread::hardware_concurrency() != 0) ? std::thread::hardware_concurrency() : min_threads;
	// https://en.wikipedia.org/wiki/Elvis_operator
	//const unsigned int max_threads = std::thread::hardware_concurrency() ?: min_threads;

	unsigned int num_threads;

	try
	{
		num_threads = static_cast<unsigned int>(std::stoi(std::getenv("NUM_THREADS")));
	}
	catch (...)
	{
		num_threads = max_threads;
	}

	num_threads = std::clamp(num_threads, min_threads, max_threads);

	/*
	if (num_threads > min_threads)
		// Don't use all the cores
		--num_threads;
	*/

	// }}}

	std::vector<std::string> prng_names;

	if (argc > 1)
	{
		// use given functions
		for (int i = 1; i < argc; ++i)
		{
			prng_names.emplace_back(argv[i]);
		}

		// validate prng_names
		for (const auto& prng_name : prng_names)
		{
			// verify the given function names are valid
			if (!prng_name_to_info.contains(prng_name))
			{
				fmt::println(stderr, "Error: \"{}\" is not a valid function name.", prng_name);

				fmt::println(stderr, "Valid function names are:");
				const auto keys = std::views::keys(prng_name_to_info);
				fmt::println(stderr, "  {}", fmt::join(keys, "\n  "));

				return EXIT_FAILURE;
			}
		}
	}
	else
	{
		// use all functions
		for (const auto& [prng_name, ignore] : prng_name_to_info)
		{
			prng_names.push_back(prng_name);
		}
	}

// conditionally register benchmark given PRNG
#define CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(NAME) \
	if (prng_name == #NAME) { \
	if (num_threads == 1) benchmark::RegisterBenchmark(#NAME, BM_prng, NAME{}); \
	else benchmark::RegisterBenchmark(#NAME, BM_prng, NAME{})->Threads(num_threads); \
}

	for (const auto& prng_name : prng_names)
	{
		// <random>
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(std::default_random_engine)
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(std::knuth_b              )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(std::minstd_rand          )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(std::minstd_rand0         )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(std::mt19937              )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(std::mt19937_64           )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(std::ranlux24             )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(std::ranlux24_base        )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(std::ranlux48             )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(std::ranlux48_base        )

		// mine
#if defined(__AES__)
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(aes128_prng_dec_dm_n1 )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(aes128_prng_dec_dm_n2 )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(aes128_prng_dec_dm_n3 )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(aes128_prng_dec_dm_n4 )

		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(aes128_prng_dec_mix_n1)
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(aes128_prng_dec_mix_n2)
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(aes128_prng_dec_mix_n3)
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(aes128_prng_dec_mix_n4)

		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(aes128_prng_enc_dm_n1 )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(aes128_prng_enc_dm_n2 )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(aes128_prng_enc_dm_n3 )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(aes128_prng_enc_dm_n4 )

		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(aes128_prng_enc_mix_n1)
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(aes128_prng_enc_mix_n2)
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(aes128_prng_enc_mix_n3)
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(aes128_prng_enc_mix_n4)

		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(aesdeclastrand       )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(aesdecrand           )

		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(aesenclastrand       )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(aesencrand           )

		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(aesimcrand           )
#endif
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(bell                 )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(bright               )
#if defined(__PCLMUL__)
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(clmulrand            )
#endif
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(degski32             )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(degski64             )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(dirk                 )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(ettinger_mixer       )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(gjrand               )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(jsf32_2              )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(jsf32_3              )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(jsf64                )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(klimov_shamir_32     )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(lcg32                )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(lcg64                )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(lea64                )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(lehmer64             )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(linnorm              )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(mcg128               )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(mizuchi              )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(moremur              )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(mover_64             )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(mover_counter_64     )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(MRG32k3a             )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(mumx_mumx_rrxx_1     )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(mumx_mumx_x1         )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(mumx_mumx_x2         )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(murmurhash3          )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(murmurhash3_32       )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(mx3                  )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(nasam                )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(orbit                )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(pcg32                )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(pcg32_fast           )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(pcg64                )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(pcg64dxsm            )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(pelican              )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(pulley               )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(quixotic             )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(romu_duo             )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(romu_duo_jr          )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(romu_quad            )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(romu_quad32          )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(romu_trio            )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(romu_trio32          )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(rrma2xsm2xs          )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(rrmxmx               )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(rrxmrrxmsx_0         )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(sea_slater_64        )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(seiran               )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(sfc32                )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(sfc64                )
#if defined(__SHA__)
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(sha1msg1rand         )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(sha1msg2rand         )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(sha256msg1rand       )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(sha256msg2rand       )
#endif
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(shioi                )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(splitmix32           )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(splitmix64           )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(splitxix33           )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(squares32            )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(squares64            )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(stc64                )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(tangle               )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(thrust_alt           )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(topping              )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(ttwanghash64         )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(wyrand               )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(xoroshiro1024plusplus)
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(xoroshiro1024starstar)
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(xoroshiro128plusplus )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(xoroshiro128starstar )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(xoroshiro64starstar  )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(xoshiro128plusplus   )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(xoshiro128starstar   )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(xoshiro256plusplus   )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(xoshiro256starstar   )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(xoshiro512plusplus   )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(xoshiro512starstar   )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(xsm32                )
		CONDITIONAL_REG_BENCHMARK_DEFAULT_CTOR(xsm64                )
	}

	benchmark::RunSpecifiedBenchmarks();

	benchmark::Shutdown();

	return 0;
}
