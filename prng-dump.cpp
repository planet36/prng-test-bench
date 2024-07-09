// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/*

gpp -Iinclude $OPTIMIZE_OPTIONS prng-dump.cpp -o prng-dump && d ./prng-dump

clear ; ./prng-dump -i | column --table --table-right 4,5

# From `make test-long`, take the fastest generators that are good (seed zero).
# The best generators are: (sorted by speed)
aesdecrand        38246
aesencrand        38011
clmulrand         21230
wyrand            15709
romu_duo_jr       15246

./prng-dump -i | grep -E -w 'aesdecrand|aesencrand|clmulrand|wyrand|romu_duo_jr' |
column --table --table-right 4,5

aesdecrand   64  0  18446744073709551615  16
aesencrand   64  0  18446744073709551615  16
clmulrand    64  0  18446744073709551615  16
romu_duo_jr  64  0  18446744073709551615  16
wyrand       64  0  18446744073709551615   8

"How to Test with PractRand"
https://www.pcg-random.org/posts/how-to-test-with-practrand.html

*/

#include "prng.hpp"
#include "seeds.hpp"

#include <cassert>
#include <climits>
#include <cstdint>
#include <random>
#include <string>
#include <utility>

#include <fmt/core.h>

#define nl (void)putchar('\n')

const char* program_author = "Steven Ward";
const char* program_version = "2023-11-14";
const char* program_license = "OSL-3.0";

// Globals

constexpr unsigned long long bytes_per_gigabyte = 1'000ULL * 1'000ULL * 1'000ULL;
constexpr unsigned long long bytes_per_gibibyte = 1'024ULL * 1'024ULL * 1'024ULL;

const std::string default_prng_name = "default_random_engine";

bool verbose = false;
unsigned long long limit_bytes = 0;
bool use_default_ctor = false;
bool use_pattern_seed = false;
bool use_random_seed = false;
bool use_zero_seed = false;

void
print_all_prng_info()
{
	for (const auto& [prng_name, info] : prng_name_to_info)
	{
		fmt::println("{}\t{}\t{}\t{}\t{}",
				prng_name,
				info.result_type_size_bits,
				info.result_min,
				info.result_max,
				info.prng_size_bytes
				);
	}
}

template <std::uniform_random_bit_generator Generator>
void
prng_dump(Generator&& rng)
{
	using result_type = typename Generator::result_type;

	// /proc/sys/fs/pipe-max-size = 1048576
	// fcntl(fileno(stdout), F_GETPIPE_SZ) = 65536
	// BUFSIZ = 8192
	// PractRand uses a buffer of size 4096 bytes for reading from stdin.
	constexpr size_t buf_size_bytes = 4096;
	constexpr size_t buf_num_elems = buf_size_bytes / sizeof(result_type);
	static_assert(buf_size_bytes % sizeof(result_type) == 0);

	result_type buf[buf_num_elems] = {0};

	if (limit_bytes == 0)
	{
		while (true)
		{
			for (size_t i = 0; i < buf_num_elems; ++i) { buf[i] = rng(); }

			(void)std::fwrite(&buf[0], sizeof(buf[0]), buf_num_elems, stdout);
		}
	}
	else // limit_bytes > 0
	{
		const size_t num_writes = limit_bytes / buf_size_bytes;
		assert(limit_bytes % buf_size_bytes == 0);

		for (size_t j = 0; j < num_writes; ++j)
		{
			for (size_t i = 0; i < buf_num_elems; ++i) { buf[i] = rng(); }

			(void)std::fwrite(&buf[0], sizeof(buf[0]), buf_num_elems, stdout);
		}
	}
}

/// Print the version information.
void
print_version()
{
	fmt::println("{} {}", program_invocation_short_name, program_version);
	fmt::println("License: {}", program_license);
	fmt::println("Written by {}", program_author);
}

/// Print the suggestion message.
void
print_suggestion()
{
	fmt::println(stderr, "Try '{} -h' for more information.",
	             program_invocation_short_name);
}

/// Print the message if verbose is enabled.
/**
\param s the string to print
*/
void
print_verbose(const std::string& s)
{
	if (verbose && !s.empty())
	{
		(void)std::fputs(s.c_str(), stderr);
		(void)std::fputc('\n', stderr);
	}
}

/// Print the warning message.
/**
\param s the string to print
*/
void
print_warning(const std::string& s)
{
	if (!s.empty())
	{
		(void)std::fputs("Warning: ", stderr);
		(void)std::fputs(s.c_str(), stderr);
		(void)std::fputc('\n', stderr);
	}
}

/// Print the error message.
/**
\param s the string to print
*/
[[noreturn]] void
print_error(const std::string& s)
{
	if (!s.empty())
	{
		(void)std::fputs("Error: ", stderr);
		(void)std::fputs(s.c_str(), stderr);
		(void)std::fputc('\n', stderr);
	}

	print_suggestion();

	std::exit(EXIT_FAILURE);
}

/// Print the help message.
void
print_usage()
{
	fmt::println("Usage: {} [OPTIONS] PRNG", program_invocation_short_name);
	fmt::println("Dump random output to stdout.");
	fmt::println("The default PRNG is {}", default_prng_name);
	nl;

	fmt::println("OPTIONS");
	nl;

	fmt::println("-V");
	fmt::println("    Print the version information, then exit.");
	nl;

	fmt::println("-h");
	fmt::println("    Print this message, then exit.");
	nl;

	fmt::println("-v");
	fmt::println("    Print diagnostics.");
	nl;

	fmt::println("-i");
	fmt::println("    Print information about the available PRNGs, then exit.");
	nl;

	fmt::println("-l  MAX");
	fmt::println("    Limit the output to no more than MAX gibibytes.");
	nl;

	fmt::println("-s  SEED_TYPE");
	fmt::println("    Specify the type of seed to be used.");
	fmt::println("    SEED_TYPE must be one of the following values:");
	fmt::println("      \"d\", \"def\", \"default\",  (The PRNG is default constructed.)");
	fmt::println("      \"p\", \"pat\", \"pattern\",  (The PRNG is seeded with this repeating pattern of bytes: {:#0{}x}.)", seed_pattern_32, sizeof(seed_pattern_32) * 2 + 2);
	fmt::println("      \"r\", \"rand\", \"random\",  (The PRNG is seeded with random bytes.)");
	fmt::println("      \"z\", \"zero\"             (The PRNG is seeded with this repeating pattern of bytes: {:#0{}x}.)", seed_zero_32, sizeof(seed_zero_32) * 2 + 2);
	nl;
}

/// Process the command line options.
/**
\param argc the arg count
\param argv the arg vector
*/
void
process_options(int argc, char* argv[])
{
	using namespace std::literals::string_literals;

	const char* short_options = "+Vhvil:s:";
	int c;
	while ((c = getopt(argc, argv, short_options)) != -1)
	{
		switch (c)
		{
		case 'h':
			print_usage();
			std::exit(EXIT_SUCCESS);
			break;

		case 'V':
			print_version();
			std::exit(EXIT_SUCCESS);
			break;

		case 'v':
			verbose = true;
			break;

		case 'i':
			print_all_prng_info();
			std::exit(EXIT_SUCCESS);
			break;

		case 'l':
			try
			{
				limit_bytes = std::stoull(optarg);
			}
			catch (const std::invalid_argument& ex)
			{
				print_error(fmt::format("invalid_argument: {}", ex.what()));
			}
			catch (const std::out_of_range& ex)
			{
				print_error(fmt::format("out_of_range: {}", ex.what()));
			}

			//if (__builtin_umulll_overflow(limit_bytes, bytes_per_gibibyte, &limit_bytes))
			if (limit_bytes > std::numeric_limits<decltype(limit_bytes)>::max() / bytes_per_gibibyte)
			{
				print_error(fmt::format("Arithmetic overflow: {} * {}", optarg, bytes_per_gibibyte));
			}
			limit_bytes *= bytes_per_gibibyte;
			break;

		case 's':
			if ((optarg == "d"s) || (optarg == "def"s) || (optarg == "default"s))
			{
				use_default_ctor = true;
				use_pattern_seed = false;
				use_random_seed = false;
				use_zero_seed = false;
			}
			else if ((optarg == "p"s) || (optarg == "pat"s) || (optarg == "pattern"s))
			{
				use_default_ctor = false;
				use_pattern_seed = true;
				use_random_seed = false;
				use_zero_seed = false;
			}
			else if ((optarg == "r"s) || (optarg == "rand"s) || (optarg == "random"s))
			{
				use_default_ctor = false;
				use_pattern_seed = false;
				use_random_seed = true;
				use_zero_seed = false;
			}
			else if ((optarg == "z"s) || (optarg == "zero"s))
			{
				use_default_ctor = false;
				use_pattern_seed = false;
				use_random_seed = false;
				use_zero_seed = true;
			}
			else
			{
				print_error(fmt::format("Invalid option value: '{}'", optarg));
			}
			break;

		default:
			std::exit(EXIT_FAILURE);
		}
	}
}

/*
std::chrono::time_point<std::chrono::steady_clock> start_time;

void print_elapsed_time()
{
	const auto end_time = std::chrono::steady_clock::now();
	const auto elapsed_time = end_time - start_time;
	const auto elapsed_time_s = std::chrono::duration<double>(elapsed_time).count();
	print_verbose(fmt::format("# time elapsed = {:.3f} s", elapsed_time_s));
}
*/

int
main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	process_options(argc, argv);

	std::string prng_name = default_prng_name;

	for (int i = optind; i < argc; ++i)
	{
		prng_name = argv[i];
	}

	print_verbose(fmt::format("# limit_bytes={}", limit_bytes));
	print_verbose(fmt::format("# prng_name={}", prng_name));

	if (!prng_name_to_info.contains(prng_name))
	{
		print_error(fmt::format("Unknown PRNG: {}", prng_name));
	}

	/*
	start_time = std::chrono::steady_clock::now();

	assert(atexit(print_elapsed_time) == 0);
	*/

	/*
	{
		const time_t now_time_t = time(nullptr);
		const tm now_time_tm = *localtime(&now_time_t);
		print_verbose(fmt::format("# begin {:%FT%T%z}", now_time_tm));
	}
	*/

#define CONDITIONAL_DUMP_STD(NAME) \
if (prng_name == #NAME) { \
	if (use_default_ctor) prng_dump(NAME{}); \
	else if (use_pattern_seed) prng_dump(NAME{seeder_pattern}); \
	else if (use_random_seed) prng_dump(random_device_seeded<NAME>()); \
	else if (use_zero_seed) prng_dump(NAME{seeder_zero}); \
	else  std::unreachable(); \
	return 0; \
}

#define CONDITIONAL_DUMP_MINE(NAME) \
if (prng_name == #NAME) { \
	if (use_default_ctor) prng_dump(NAME{}); \
	else if (use_pattern_seed) prng_dump(NAME{get_seed_bytes_pattern<NAME>()}); \
	else if (use_random_seed) prng_dump(NAME{}); \
	else if (use_zero_seed) prng_dump(NAME{get_seed_bytes_zero<NAME>()}); \
	else  std::unreachable(); \
	return 0; \
}

	// <random>
	CONDITIONAL_DUMP_STD(std::default_random_engine)
	CONDITIONAL_DUMP_STD(std::knuth_b              )
	CONDITIONAL_DUMP_STD(std::minstd_rand          )
	CONDITIONAL_DUMP_STD(std::minstd_rand0         )
	CONDITIONAL_DUMP_STD(std::mt19937              )
	CONDITIONAL_DUMP_STD(std::mt19937_64           )
	CONDITIONAL_DUMP_STD(std::ranlux24             )
	CONDITIONAL_DUMP_STD(std::ranlux24_base        )
	CONDITIONAL_DUMP_STD(std::ranlux48             )
	CONDITIONAL_DUMP_STD(std::ranlux48_base        )

	// mine
#if defined(__AES__)
	CONDITIONAL_DUMP_MINE(aesdeclastrand       )
	CONDITIONAL_DUMP_MINE(aesdecrand           )
	CONDITIONAL_DUMP_MINE(aesenclastrand       )
	CONDITIONAL_DUMP_MINE(aesencrand           )
	CONDITIONAL_DUMP_MINE(aesimcrand           )
#endif
	CONDITIONAL_DUMP_MINE(bell                 )
	CONDITIONAL_DUMP_MINE(bright               )
#if defined(__PCLMUL__)
	CONDITIONAL_DUMP_MINE(clmulrand            )
#endif
	CONDITIONAL_DUMP_MINE(degski32             )
	CONDITIONAL_DUMP_MINE(degski64             )
	CONDITIONAL_DUMP_MINE(dirk                 )
	CONDITIONAL_DUMP_MINE(ettinger_mixer       )
	CONDITIONAL_DUMP_MINE(gjrand               )
	CONDITIONAL_DUMP_MINE(jsf32_2              )
	CONDITIONAL_DUMP_MINE(jsf32_3              )
	CONDITIONAL_DUMP_MINE(jsf64                )
	CONDITIONAL_DUMP_MINE(klimov_shamir_32     )
	CONDITIONAL_DUMP_MINE(lcg32                )
	CONDITIONAL_DUMP_MINE(lcg64                )
	CONDITIONAL_DUMP_MINE(lea64                )
	CONDITIONAL_DUMP_MINE(lehmer64             )
	CONDITIONAL_DUMP_MINE(linnorm              )
	CONDITIONAL_DUMP_MINE(mcg128               )
	CONDITIONAL_DUMP_MINE(mizuchi              )
	CONDITIONAL_DUMP_MINE(moremur              )
	CONDITIONAL_DUMP_MINE(mover_64             )
	CONDITIONAL_DUMP_MINE(mover_counter_64     )
	CONDITIONAL_DUMP_MINE(MRG32k3a             )
	CONDITIONAL_DUMP_MINE(mumx_mumx_rrxx_1     )
	CONDITIONAL_DUMP_MINE(mumx_mumx_x1         )
	CONDITIONAL_DUMP_MINE(mumx_mumx_x2         )
	CONDITIONAL_DUMP_MINE(murmurhash3          )
	CONDITIONAL_DUMP_MINE(murmurhash3_32       )
	CONDITIONAL_DUMP_MINE(mx3                  )
	CONDITIONAL_DUMP_MINE(nasam                )
	CONDITIONAL_DUMP_MINE(orbit                )
	CONDITIONAL_DUMP_MINE(pcg32                )
	CONDITIONAL_DUMP_MINE(pcg32_fast           )
	CONDITIONAL_DUMP_MINE(pcg64                )
	CONDITIONAL_DUMP_MINE(pcg64dxsm            )
	CONDITIONAL_DUMP_MINE(pelican              )
	CONDITIONAL_DUMP_MINE(pulley               )
	CONDITIONAL_DUMP_MINE(quixotic             )
	CONDITIONAL_DUMP_MINE(romu_duo             )
	CONDITIONAL_DUMP_MINE(romu_duo_jr          )
	CONDITIONAL_DUMP_MINE(romu_quad            )
	CONDITIONAL_DUMP_MINE(romu_quad32          )
	CONDITIONAL_DUMP_MINE(romu_trio            )
	CONDITIONAL_DUMP_MINE(romu_trio32          )
	CONDITIONAL_DUMP_MINE(rrma2xsm2xs          )
	CONDITIONAL_DUMP_MINE(rrmxmx               )
	CONDITIONAL_DUMP_MINE(rrxmrrxmsx_0         )
	CONDITIONAL_DUMP_MINE(sea_slater_64        )
	CONDITIONAL_DUMP_MINE(seiran               )
	CONDITIONAL_DUMP_MINE(sfc32                )
	CONDITIONAL_DUMP_MINE(sfc64                )
#if defined(__SHA__)
	CONDITIONAL_DUMP_MINE(sha1msg1rand         )
	CONDITIONAL_DUMP_MINE(sha1msg2rand         )
	CONDITIONAL_DUMP_MINE(sha256msg1rand       )
	CONDITIONAL_DUMP_MINE(sha256msg2rand       )
#endif
	CONDITIONAL_DUMP_MINE(shioi                )
	CONDITIONAL_DUMP_MINE(splitmix32           )
	CONDITIONAL_DUMP_MINE(splitmix64           )
	CONDITIONAL_DUMP_MINE(splitxix33           )
	CONDITIONAL_DUMP_MINE(squares32            )
	CONDITIONAL_DUMP_MINE(squares64            )
	CONDITIONAL_DUMP_MINE(stc64                )
	CONDITIONAL_DUMP_MINE(tangle               )
	CONDITIONAL_DUMP_MINE(thrust_alt           )
	CONDITIONAL_DUMP_MINE(topping              )
	CONDITIONAL_DUMP_MINE(ttwanghash64         )
	CONDITIONAL_DUMP_MINE(wyrand               )
	CONDITIONAL_DUMP_MINE(xoroshiro1024plusplus)
	CONDITIONAL_DUMP_MINE(xoroshiro1024starstar)
	CONDITIONAL_DUMP_MINE(xoroshiro128plusplus )
	CONDITIONAL_DUMP_MINE(xoroshiro128starstar )
	CONDITIONAL_DUMP_MINE(xoroshiro64starstar  )
	CONDITIONAL_DUMP_MINE(xoshiro128plusplus   )
	CONDITIONAL_DUMP_MINE(xoshiro128starstar   )
	CONDITIONAL_DUMP_MINE(xoshiro256plusplus   )
	CONDITIONAL_DUMP_MINE(xoshiro256starstar   )
	CONDITIONAL_DUMP_MINE(xoshiro512plusplus   )
	CONDITIONAL_DUMP_MINE(xoshiro512starstar   )
	CONDITIONAL_DUMP_MINE(xsm32                )
	CONDITIONAL_DUMP_MINE(xsm64                )

	/*
	{
		const time_t now_time_t = time(nullptr);
		const tm now_time_tm = *localtime(&now_time_t);
		print_verbose(fmt::format("# end {:%FT%T%z}", now_time_tm));
	}
	*/

	return 0;
}

/*

Output:

MRG32k3a                    64  0  18446744073709551615     8
aesdeclastrand              64  0  18446744073709551615    16
aesdecrand                  64  0  18446744073709551615    16
aesenclastrand              64  0  18446744073709551615    16
aesencrand                  64  0  18446744073709551615    16
aesimcrand                  64  0  18446744073709551615    16
bell                        64  0  18446744073709551615    16
bright                      64  0  18446744073709551615     8
clmulrand                   64  0  18446744073709551615    16
degski32                    32  0            4294967295     4
degski64                    64  0  18446744073709551615     8
dirk                        64  0  18446744073709551615     8
ettinger_mixer              64  0  18446744073709551615     8
gjrand                      64  0  18446744073709551615    32
jsf32_2                     32  0            4294967295    16
jsf32_3                     32  0            4294967295    16
jsf64                       64  0  18446744073709551615    32
klimov_shamir_32            32  0            4294967295     8
lcg32                       32  0            4294967295    16
lcg64                       64  0  18446744073709551615    16
lea64                       64  0  18446744073709551615     8
lehmer64                    64  0  18446744073709551615    16
linnorm                     64  0  18446744073709551615     8
mcg128                      64  0  18446744073709551615    16
mizuchi                     64  0  18446744073709551615     8
moremur                     64  0  18446744073709551615     8
mover_64                    64  0  18446744073709551615    16
mover_counter_64            64  0  18446744073709551615    16
mumx_mumx_rrxx_1            64  0  18446744073709551615     8
mumx_mumx_x1                64  0  18446744073709551615     8
mumx_mumx_x2                64  0  18446744073709551615     8
murmurhash3                 64  0  18446744073709551615     8
murmurhash3_32              32  0            4294967295     4
mx3                         64  0  18446744073709551615     8
nasam                       64  0  18446744073709551615     8
orbit                       64  0  18446744073709551615    16
pcg32                       32  0            4294967295     8
pcg32_fast                  32  0            4294967295     8
pcg64                       64  0  18446744073709551615    16
pcg64dxsm                   64  0  18446744073709551615    16
pelican                     64  0  18446744073709551615     8
pulley                      64  0  18446744073709551615     8
quixotic                    64  0  18446744073709551615     8
romu_duo                    64  0  18446744073709551615    16
romu_duo_jr                 64  0  18446744073709551615    16
romu_quad                   64  0  18446744073709551615    32
romu_quad32                 32  0            4294967295    16
romu_trio                   64  0  18446744073709551615    24
romu_trio32                 32  0            4294967295    12
rrma2xsm2xs                 64  0  18446744073709551615     8
rrmxmx                      64  0  18446744073709551615     8
rrxmrrxmsx_0                64  0  18446744073709551615     8
sea_slater_64               64  0  18446744073709551615    16
seiran                      64  0  18446744073709551615    16
sfc32                       32  0            4294967295    16
sfc64                       64  0  18446744073709551615    32
shioi                       64  0  18446744073709551615    16
splitmix32                  32  0            4294967295     8
splitmix64                  64  0  18446744073709551615     8
splitxix33                  64  0  18446744073709551615     8
squares32                   32  0            4294967295     8
squares64                   64  0  18446744073709551615     8
stc64                       64  0  18446744073709551615    32
std::default_random_engine  64  1            2147483646     8
std::knuth_b                64  1            2147483646  2064
std::minstd_rand            64  1            2147483646     8
std::minstd_rand0           64  1            2147483646     8
std::mt19937                64  0            4294967295  5000
std::mt19937_64             64  0  18446744073709551615  2504
std::ranlux24               64  0              16777215   216
std::ranlux24_base          64  0              16777215   208
std::ranlux48               64  0       281474976710655   120
std::ranlux48_base          64  0       281474976710655   112
tangle                      64  0  18446744073709551615    16
thrust_alt                  64  0  18446744073709551615     8
topping                     64  0  18446744073709551615     8
ttwanghash64                64  0  18446744073709551615     8
wyrand                      64  0  18446744073709551615     8
xoroshiro1024plusplus       64  0  18446744073709551615   136
xoroshiro1024starstar       64  0  18446744073709551615   136
xoroshiro128plusplus        64  0  18446744073709551615    16
xoroshiro128starstar        64  0  18446744073709551615    16
xoroshiro64starstar         32  0            4294967295     8
xoshiro128plusplus          32  0            4294967295    16
xoshiro128starstar          32  0            4294967295    16
xoshiro256plusplus          64  0  18446744073709551615    32
xoshiro256starstar          64  0  18446744073709551615    32
xoshiro512plusplus          64  0  18446744073709551615    64
xoshiro512starstar          64  0  18446744073709551615    64
xsm32                       32  0            4294967295     8
xsm64                       64  0  18446744073709551615    16

*/
