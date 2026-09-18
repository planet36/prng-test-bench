// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/*

g++ -std=c++26 -O3 -march=native -I include prng-dump.cpp -o prng-dump

clear ; ./prng-dump -i | column --table --table-right 2,4,5

"How to Test with PractRand"
https://www.pcg-random.org/posts/how-to-test-with-practrand.html

*/

#include "parse_int.hpp"
#include "prng.hpp"
#include "seed_seq.hpp"
#include "seeds.hpp"

#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <err.h>
#include <exception>
#include <print>
#include <random>
#include <string>
#include <string_view>
#include <type_traits>
#include <unistd.h>

inline constexpr std::string_view program_author = "Steven Ward";
inline constexpr std::string_view program_version = "2026-09-18";
inline constexpr std::string_view program_license = "MPL-2.0";

// Globals

inline constexpr unsigned long long bytes_per_gigabyte = 1000ULL * 1000ULL * 1000ULL;
inline constexpr unsigned long long bytes_per_gibibyte = 1024ULL * 1024ULL * 1024ULL;

inline constexpr uint32_t seed_pattern_32{UINT32_C(0x01010101) * seed_pattern_byte};

inline constexpr std::string_view default_prng_name{"std::default_random_engine"};

inline constexpr unsigned long long max_limit_gibibytes = 1024ULL * 1024ULL; // 1 pebibyte (PiB)

bool verbose = false;
unsigned long long limit_bytes = 0;

/// How the PRNG is seeded
enum class seed_type
{
    default_ctor,
    pattern,
    random,
    zero,
};

seed_type seed = seed_type::default_ctor;

/// Print a tab-separated line of information about each PRNG
void
print_all_prng_info()
{
    for (const auto& [prng_name, info] : prng_name_to_info)
    {
        // XXX: Do not change the order of the fields.
        std::println("{}\t{}\t{}\t{}\t{}",
                prng_name,
                info.result_size_bits,
                info.result_min,
                info.result_max,
                info.state_size_bytes);
    }
}

/// Write all \a count bytes of \a buf to \a fd
/**
* Retry after a partial write or \c EINTR.  Exit on any other error.
*/
void
write_all(const int fd, const void* buf, size_t count)
{
    const auto* p = static_cast<const char*>(buf);

    while (count > 0)
    {
        const ssize_t num_bytes_written = ::write(fd, p, count);

        if (num_bytes_written < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }

            err(EXIT_FAILURE, "write");
        }

        p += num_bytes_written;
        count -= static_cast<size_t>(num_bytes_written);
    }
}

/// Write the output of \a gen to stdout in blocks of 32 KiB
/**
* Stop after \c limit_bytes bytes, or never if \c limit_bytes is 0.
*/
template <typename URBG>
requires std::uniform_random_bit_generator<std::remove_cvref_t<URBG>>
void
prng_dump(URBG&& gen)
{
    using result_type = typename std::remove_cvref_t<URBG>::result_type;

    // /proc/sys/fs/pipe-max-size = 1048576
    // fcntl(STDOUT_FILENO, F_GETPIPE_SZ) = 65536
    // BUFSIZ = 8192
    // PractRand uses a buffer of size 32768 bytes for reading from stdin.
    constexpr size_t buf_size_bytes = 32768;
    constexpr size_t buf_num_elems = buf_size_bytes / sizeof(result_type);
    static_assert(buf_size_bytes % sizeof(result_type) == 0);
    static_assert(bytes_per_gibibyte % buf_size_bytes == 0);

    result_type buf[buf_num_elems] = {0};

    const size_t num_writes = limit_bytes / buf_size_bytes;

    for (size_t j = 0; (limit_bytes == 0) || (j < num_writes); ++j)
    {
        for (size_t i = 0; i < buf_num_elems; ++i)
        {
            buf[i] = gen();
        }

        write_all(STDOUT_FILENO, buf, sizeof(buf));
    }
}

/// Print the version information
void
print_version()
{
    std::println("{} {}", program_invocation_short_name, program_version);
    std::println("License: {}", program_license);
    std::println("Written by {}", program_author);
}

/// Print the help message
void
print_usage()
{
    std::println("Usage: {} [OPTION]... [PRNG]", program_invocation_short_name);
    std::println("Dump random output to stdout.");
    std::println("The default PRNG is {}.", default_prng_name);
    std::println("");

    std::println("OPTIONS");
    std::println("");

    std::println("-V");
    std::println("    Print the version information, then exit.");
    std::println("");

    std::println("-h");
    std::println("    Print this message, then exit.");
    std::println("");

    std::println("-v");
    std::println("    Print diagnostics.");
    std::println("");

    std::println("-i");
    std::println("    Print information about the available PRNGs, then exit.");
    std::println("");

    std::println("-l  MAX");
    std::println("    Limit the output to no more than MAX gibibytes.");
    std::println("    If MAX is 0, the output is unlimited.");
    std::println("    (maximum: {})", max_limit_gibibytes);
    std::println("");

    std::println("-s  SEED_TYPE");
    std::println("    Specify the type of seed to be used.");
    std::println(R"(    If not given, "default" is used.)");
    std::println("    SEED_TYPE must be one of the following values:");
    std::println(R"(      "d", "def", "default",  (The PRNG is default constructed.  A std engine gets its fixed default seed, and any other PRNG is seeded with random values.))");
    std::println(R"(      "p", "pat", "pattern",  (The PRNG is seeded with bytes of value 0x{:02X}.))", seed_pattern_byte);
    std::println(R"(      "r", "rand", "random",  (The PRNG is seeded with random values.  For a PRNG not in std, this is the same as "default".))");
    std::println(R"(      "z", "zero",            (The PRNG is seeded with bytes of value 0x00.))");
    std::println("");
}

/// Process the command line options
// NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays)
void process_options(int argc, char* argv[])
try
{
    using namespace std::literals;

    static constexpr std::string_view short_options = "+Vhvil:s:";
    int c = 0;
    while ((c = getopt(argc, argv, short_options.data())) != -1)
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
            {
                // the value of "-l" is gibibytes (GiB)
                const unsigned long long limit_gibibytes = parse_option_int(optarg, 0,
                        max_limit_gibibytes, "-l");

                // convert GiB to B
                limit_bytes = limit_gibibytes * bytes_per_gibibyte;
            }
            break;

        case 's':
            if ((optarg == "d"sv) || (optarg == "def"sv) || (optarg == "default"sv))
            {
                seed = seed_type::default_ctor;
            }
            else if ((optarg == "p"sv) || (optarg == "pat"sv) || (optarg == "pattern"sv))
            {
                seed = seed_type::pattern;
            }
            else if ((optarg == "r"sv) || (optarg == "rand"sv) || (optarg == "random"sv))
            {
                seed = seed_type::random;
            }
            else if ((optarg == "z"sv) || (optarg == "zero"sv))
            {
                seed = seed_type::zero;
            }
            else
            {
                errx(EXIT_FAILURE, "Invalid option value: \"%s\"", optarg);
            }
            break;

        default:
            std::exit(EXIT_FAILURE);
        }
    }
}
catch (const std::exception& ex)
{
    (void)std::fflush(stdout);
    errx(EXIT_FAILURE, "%s", ex.what());
}

int
main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    process_options(argc, argv);

    std::string prng_name{default_prng_name};

    if (argc - optind == 1)
    {
        prng_name = argv[optind];
    }
    else if (argc - optind > 1)
    {
        errx(EXIT_FAILURE, "Too many arguments");
    }

    if (verbose)
    {
        std::println(stderr, "# limit_bytes={}", limit_bytes);
        std::println(stderr, "# prng_name={}", prng_name);
    }

    if (!prng_name_to_info.contains(prng_name))
    {
        errx(EXIT_FAILURE, "Unknown PRNG: \"%s\"", prng_name.c_str());
    }

#define CONDITIONAL_DUMP_STD(NAME) \
if (prng_name == #NAME) { \
    fill_seed_seq<seed_pattern_32> seeder_pattern; \
    fill_seed_seq<0> seeder_zero; \
    switch (seed) { \
    case seed_type::default_ctor: prng_dump(NAME{}); break; \
    case seed_type::pattern: prng_dump(NAME(seeder_pattern)); break; \
    case seed_type::random: prng_dump(random_device_seeded<NAME>()); break; \
    case seed_type::zero: prng_dump(NAME(seeder_zero)); break; \
    } \
    return 0; \
}

#define CONDITIONAL_DUMP_MINE(NAME) \
if (prng_name == #NAME) { \
    switch (seed) { \
    case seed_type::default_ctor: prng_dump(NAME{}); break; \
    case seed_type::pattern: prng_dump(NAME{get_seed_bytes_pattern<NAME>()}); break; \
    case seed_type::random: prng_dump(NAME{}); break; \
    case seed_type::zero: prng_dump(NAME{get_seed_bytes_zero<NAME>()}); break; \
    } \
    return 0; \
}

    // <random>
    CONDITIONAL_DUMP_STD(std::default_random_engine) // NOLINT(bugprone-random-generator-seed,cert-msc32-c,cert-msc51-cpp)
    CONDITIONAL_DUMP_STD(std::knuth_b              ) // NOLINT(bugprone-random-generator-seed,cert-msc32-c,cert-msc51-cpp)
    CONDITIONAL_DUMP_STD(std::minstd_rand          ) // NOLINT(bugprone-random-generator-seed,cert-msc32-c,cert-msc51-cpp)
    CONDITIONAL_DUMP_STD(std::minstd_rand0         ) // NOLINT(bugprone-random-generator-seed,cert-msc32-c,cert-msc51-cpp)
    CONDITIONAL_DUMP_STD(std::mt19937              ) // NOLINT(bugprone-random-generator-seed,cert-msc32-c,cert-msc51-cpp)
    CONDITIONAL_DUMP_STD(std::mt19937_64           ) // NOLINT(bugprone-random-generator-seed,cert-msc32-c,cert-msc51-cpp)
    CONDITIONAL_DUMP_STD(std::philox4x32           ) // NOLINT(bugprone-random-generator-seed,cert-msc32-c,cert-msc51-cpp)
    CONDITIONAL_DUMP_STD(std::philox4x64           ) // NOLINT(bugprone-random-generator-seed,cert-msc32-c,cert-msc51-cpp)
    CONDITIONAL_DUMP_STD(std::ranlux24             ) // NOLINT(bugprone-random-generator-seed,cert-msc32-c,cert-msc51-cpp)
    CONDITIONAL_DUMP_STD(std::ranlux24_base        ) // NOLINT(bugprone-random-generator-seed,cert-msc32-c,cert-msc51-cpp)
    CONDITIONAL_DUMP_STD(std::ranlux48             ) // NOLINT(bugprone-random-generator-seed,cert-msc32-c,cert-msc51-cpp)
    CONDITIONAL_DUMP_STD(std::ranlux48_base        ) // NOLINT(bugprone-random-generator-seed,cert-msc32-c,cert-msc51-cpp)

    // mine
#if defined(__AES__)
    CONDITIONAL_DUMP_MINE(aes_compress_ctr2_128 )
    CONDITIONAL_DUMP_MINE(aes_ctr_128           )
#endif
    CONDITIONAL_DUMP_MINE(biski64               )
#if defined(__PCLMUL__)
    CONDITIONAL_DUMP_MINE(clmulrand             )
#endif
    CONDITIONAL_DUMP_MINE(degski32              )
    CONDITIONAL_DUMP_MINE(degski64              )
    CONDITIONAL_DUMP_MINE(ettinger_mixer        )
    CONDITIONAL_DUMP_MINE(gjrand                )
    CONDITIONAL_DUMP_MINE(jsf32_2               )
    CONDITIONAL_DUMP_MINE(jsf32_3               )
    CONDITIONAL_DUMP_MINE(jsf64                 )
    CONDITIONAL_DUMP_MINE(klimov_shamir_32      )
    CONDITIONAL_DUMP_MINE(lcg32                 )
    CONDITIONAL_DUMP_MINE(lcg64                 )
    CONDITIONAL_DUMP_MINE(lea64                 )
    CONDITIONAL_DUMP_MINE(lehmer64              )
    CONDITIONAL_DUMP_MINE(lxm                   )
    CONDITIONAL_DUMP_MINE(mcg128                )
    CONDITIONAL_DUMP_MINE(moremur               )
    CONDITIONAL_DUMP_MINE(msws32                )
    CONDITIONAL_DUMP_MINE(msws64                )
    CONDITIONAL_DUMP_MINE(mumx_ctr2             )
    CONDITIONAL_DUMP_MINE(mumx_mumx_rrxx_1      )
    CONDITIONAL_DUMP_MINE(mumx_mumx_x1          )
    CONDITIONAL_DUMP_MINE(mumx_mumx_x2          )
    CONDITIONAL_DUMP_MINE(murmurhash3           )
    CONDITIONAL_DUMP_MINE(murmurhash3_32        )
    CONDITIONAL_DUMP_MINE(mx3                   )
    CONDITIONAL_DUMP_MINE(nasam                 )
    CONDITIONAL_DUMP_MINE(pcg32                 )
    CONDITIONAL_DUMP_MINE(pcg32_fast            )
    CONDITIONAL_DUMP_MINE(pcg64                 )
    CONDITIONAL_DUMP_MINE(pcg64dxsm             )
    CONDITIONAL_DUMP_MINE(romu_duo              )
    CONDITIONAL_DUMP_MINE(romu_duo_jr           )
    CONDITIONAL_DUMP_MINE(romu_quad             )
    CONDITIONAL_DUMP_MINE(romu_quad32           )
    CONDITIONAL_DUMP_MINE(romu_trio             )
    CONDITIONAL_DUMP_MINE(romu_trio32           )
    CONDITIONAL_DUMP_MINE(rrma2xsm2xs           )
    CONDITIONAL_DUMP_MINE(rrmxmx                )
    CONDITIONAL_DUMP_MINE(rrxmrrxmsx_0          )
    CONDITIONAL_DUMP_MINE(seiran                )
    CONDITIONAL_DUMP_MINE(sfc32                 )
    CONDITIONAL_DUMP_MINE(sfc64                 )
#if defined(__SHA__)
    CONDITIONAL_DUMP_MINE(sha1_ctr_128          )
    CONDITIONAL_DUMP_MINE(sha256_ctr_128        )
#endif
    CONDITIONAL_DUMP_MINE(shioi                 )
    CONDITIONAL_DUMP_MINE(splitmix32            )
    CONDITIONAL_DUMP_MINE(splitmix64            )
    CONDITIONAL_DUMP_MINE(splitxix33            )
    CONDITIONAL_DUMP_MINE(squares32             )
    CONDITIONAL_DUMP_MINE(squares64             )
    CONDITIONAL_DUMP_MINE(staffordMix13         )
    CONDITIONAL_DUMP_MINE(stc_crand32           )
    CONDITIONAL_DUMP_MINE(stc_crand64           )
    CONDITIONAL_DUMP_MINE(ttwanghash64          )
    CONDITIONAL_DUMP_MINE(wyrand                )
    CONDITIONAL_DUMP_MINE(xoroshiro64starstar   )
    CONDITIONAL_DUMP_MINE(xoroshiro128plusplus  )
    CONDITIONAL_DUMP_MINE(xoroshiro128starstar  )
    CONDITIONAL_DUMP_MINE(xoroshiro1024plusplus )
    CONDITIONAL_DUMP_MINE(xoroshiro1024starstar )
    CONDITIONAL_DUMP_MINE(xoroshiro128aox       )
    CONDITIONAL_DUMP_MINE(xoshiro128plusplus    )
    CONDITIONAL_DUMP_MINE(xoshiro128starstar    )
    CONDITIONAL_DUMP_MINE(xoshiro256plusplus    )
    CONDITIONAL_DUMP_MINE(xoshiro256starstar    )
    CONDITIONAL_DUMP_MINE(xoshiro512plusplus    )
    CONDITIONAL_DUMP_MINE(xoshiro512starstar    )
    CONDITIONAL_DUMP_MINE(xsm32                 )
    CONDITIONAL_DUMP_MINE(xsm64                 )
    CONDITIONAL_DUMP_MINE(xxh32_avalanche       )
    CONDITIONAL_DUMP_MINE(xxh64_avalanche       )
    CONDITIONAL_DUMP_MINE(xxh3_avalanche        )
    CONDITIONAL_DUMP_MINE(xxh3_rrmxmx           )

    return 0;
}

/*

Output:

aes_compress_ctr2_128       128  0  340282366920938463463374607431768211455    32
aes_ctr_128                 128  0  340282366920938463463374607431768211455    32
biski64                      64  0                     18446744073709551615    24
clmulrand                    64  0                     18446744073709551615    16
degski32                     32  0                               4294967295     4
degski64                     64  0                     18446744073709551615     8
ettinger_mixer               64  0                     18446744073709551615     8
gjrand                       64  0                     18446744073709551615    32
jsf32_2                      32  0                               4294967295    16
jsf32_3                      32  0                               4294967295    16
jsf64                        64  0                     18446744073709551615    32
klimov_shamir_32             32  0                               4294967295     8
lcg32                        32  0                               4294967295    16
lcg64                        64  0                     18446744073709551615    16
lea64                        64  0                     18446744073709551615     8
lehmer64                     64  0                     18446744073709551615    16
lxm                          64  0                     18446744073709551615    32
mcg128                       64  0                     18446744073709551615    16
moremur                      64  0                     18446744073709551615     8
msws32                       32  0                               4294967295    16
msws64                       64  0                     18446744073709551615    32
mumx_ctr2                    64  0                     18446744073709551615    16
mumx_mumx_rrxx_1             64  0                     18446744073709551615     8
mumx_mumx_x1                 64  0                     18446744073709551615     8
mumx_mumx_x2                 64  0                     18446744073709551615     8
murmurhash3                  64  0                     18446744073709551615     8
murmurhash3_32               32  0                               4294967295     4
mx3                          64  0                     18446744073709551615     8
nasam                        64  0                     18446744073709551615     8
pcg32                        32  0                               4294967295     8
pcg32_fast                   32  0                               4294967295     8
pcg64                        64  0                     18446744073709551615    16
pcg64dxsm                    64  0                     18446744073709551615    16
romu_duo                     64  0                     18446744073709551615    16
romu_duo_jr                  64  0                     18446744073709551615    16
romu_quad                    64  0                     18446744073709551615    32
romu_quad32                  32  0                               4294967295    16
romu_trio                    64  0                     18446744073709551615    24
romu_trio32                  32  0                               4294967295    12
rrma2xsm2xs                  64  0                     18446744073709551615     8
rrmxmx                       64  0                     18446744073709551615     8
rrxmrrxmsx_0                 64  0                     18446744073709551615     8
seiran                       64  0                     18446744073709551615    16
sfc32                        32  0                               4294967295    16
sfc64                        64  0                     18446744073709551615    32
sha1_ctr_128                128  0  340282366920938463463374607431768211455    16
sha256_ctr_128              128  0  340282366920938463463374607431768211455    16
shioi                        64  0                     18446744073709551615    16
splitmix32                   32  0                               4294967295     8
splitmix64                   64  0                     18446744073709551615     8
splitxix33                   64  0                     18446744073709551615     8
squares32                    32  0                               4294967295     8
squares64                    64  0                     18446744073709551615     8
staffordMix13                64  0                     18446744073709551615     8
stc_crand32                  32  0                               4294967295    16
stc_crand64                  64  0                     18446744073709551615    32
std::default_random_engine   64  1                               2147483646     8
std::knuth_b                 64  1                               2147483646  2064
std::minstd_rand             64  1                               2147483646     8
std::minstd_rand0            64  1                               2147483646     8
std::mt19937                 64  0                               4294967295  5000
std::mt19937_64              64  0                     18446744073709551615  2504
std::philox4x32              64  0                               4294967295    88
std::philox4x64              64  0                     18446744073709551615    88
std::ranlux24                64  0                                 16777215   216
std::ranlux24_base           64  0                                 16777215   208
std::ranlux48                64  0                          281474976710655   120
std::ranlux48_base           64  0                          281474976710655   112
ttwanghash64                 64  0                     18446744073709551615     8
wyrand                       64  0                     18446744073709551615     8
xoroshiro1024plusplus        64  0                     18446744073709551615   128
xoroshiro1024starstar        64  0                     18446744073709551615   128
xoroshiro128aox              64  0                     18446744073709551615    16
xoroshiro128plusplus         64  0                     18446744073709551615    16
xoroshiro128starstar         64  0                     18446744073709551615    16
xoroshiro64starstar          32  0                               4294967295     8
xoshiro128plusplus           32  0                               4294967295    16
xoshiro128starstar           32  0                               4294967295    16
xoshiro256plusplus           64  0                     18446744073709551615    32
xoshiro256starstar           64  0                     18446744073709551615    32
xoshiro512plusplus           64  0                     18446744073709551615    64
xoshiro512starstar           64  0                     18446744073709551615    64
xsm32                        32  0                               4294967295     8
xsm64                        64  0                     18446744073709551615    16
xxh32_avalanche              32  0                               4294967295     4
xxh3_avalanche               64  0                     18446744073709551615     8
xxh3_rrmxmx                  64  0                     18446744073709551615     8
xxh64_avalanche              64  0                     18446744073709551615     8

*/
