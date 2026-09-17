// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/*

g++ -std=c++26 -O2 -march=native -I include warmup-survey.cpp -o warmup-survey && ./warmup-survey

Survey the PRNGs that discard outputs in init ("warm-up").  Each PRNG starts from the state that
init has before its warm-up, and the survey reports how many outputs it takes before the
outputs look filled.

*/

#include "prng.hpp"

#include <array>
#include <bit>
#include <climits>
#include <cstdint>
#include <format>
#include <print>
#include <string>
#include <string_view>

constexpr int max_calls = 48;

// The number of calls shown in the per-call rows
constexpr int shown_calls = 16;

// An output looks filled when at least this fraction of its bits is set, or, compared with the
// output before it, at least this fraction of its bits changed.
constexpr double threshold = 0.40;

/// A PRNG whose state can be reset to all zeros, without a warm-up
template <typename G>
struct zero_start : G
{
    using state_type = typename G::state_type;

    zero_start() : G(state_type{}) {}

    /// Set the state to what init has before its warm-up when it keeps an all-zero seed
    void
    reset_state()
    {
        this->s = state_type{};
        if constexpr (requires { this->p; })
        {
            this->p = 0;
        }
    }
};

/// A PRNG whose state can be reset to 1, 2, 3, ..., without a warm-up
template <typename G>
struct iota_start : G
{
    using state_type = typename G::state_type;

    iota_start() : G(state_type{}) {}

    /// Set the state to what init has before its warm-up when it replaces an all-zero seed
    void
    reset_state()
    {
        for (size_t i = 0; i < std::size(this->s); ++i)
        {
            this->s[i] = static_cast<typename state_type::value_type>(i + 1);
        }
        if constexpr (requires { this->p; })
        {
            this->p = 0;
        }
    }
};

/// One value per call
using series = std::array<double, max_calls>;

/// The index of the first call whose value reaches the threshold, starting at \a first
[[nodiscard]] int
first_reaching(const series& values, int first)
{
    for (int k = first; k < max_calls; ++k)
    {
        if (values[k] >= threshold)
        {
            return k;
        }
    }
    return max_calls;
}

[[nodiscard]] std::string
format_row(const series& values, int first)
{
    std::string row;
    for (int k = 0; k < shown_calls; ++k)
    {
        row += (k < first) ? std::string("   --") : std::format(" {:4.2f}", values[k]);
    }
    return row;
}

template <typename Start>
void
survey(std::string_view name, std::string_view start_name)
{
    Start g;
    g.reset_state();

    const int bits = sizeof(typename Start::result_type) * CHAR_BIT;

    // popcount[k] is the fraction of bits set in output k.  changed[k] is the fraction of bits
    // that differ between output k and output k - 1, so it starts at call 1.
    series popcount{};
    series changed{};
    uint64_t previous = 0;
    for (int k = 0; k < max_calls; ++k)
    {
        const auto output = static_cast<uint64_t>(g.next());
        popcount[k] = std::popcount(output) / double(bits);
        if (k > 0)
        {
            changed[k] = std::popcount(output ^ previous) / double(bits);
        }
        previous = output;
    }

    std::println("{:<22} {:<5} {:>9} {:>8}", name, start_name, first_reaching(popcount, 0),
                 first_reaching(changed, 1));
    std::println("    set    {}", format_row(popcount, 0));
    std::println("    change {}", format_row(changed, 1));
}

int
main()
{
    std::println("Outputs to discard: the index of the first output with at least {:.0f}% of its",
                 threshold * 100);
    std::println("bits set (popcount), or with at least {:.0f}% of its bits changed from the output",
                 threshold * 100);
    std::println("before it (change).  The rows show both fractions for calls 0-{}.\n",
                 shown_calls - 1);
    std::println("{:<22} {:<5} {:>9} {:>8}", "PRNG", "start", "popcount", "change");

    survey<iota_start<xoroshiro128plusplus>>("xoroshiro128plusplus", "iota");
    survey<iota_start<xoroshiro128starstar>>("xoroshiro128starstar", "iota");
    survey<iota_start<xoroshiro1024plusplus>>("xoroshiro1024plusplus", "iota");
    survey<iota_start<xoroshiro1024starstar>>("xoroshiro1024starstar", "iota");
    survey<iota_start<xoshiro128plusplus>>("xoshiro128plusplus", "iota");
    survey<iota_start<xoshiro128starstar>>("xoshiro128starstar", "iota");
    survey<iota_start<xoshiro256plusplus>>("xoshiro256plusplus", "iota");
    survey<iota_start<xoshiro256starstar>>("xoshiro256starstar", "iota");
    survey<iota_start<xoshiro512plusplus>>("xoshiro512plusplus", "iota");
    survey<iota_start<xoshiro512starstar>>("xoshiro512starstar", "iota");
    survey<zero_start<sfc32>>("sfc32", "zero");
    survey<zero_start<sfc64>>("sfc64", "zero");
    survey<zero_start<biski64>>("biski64", "zero");
}
