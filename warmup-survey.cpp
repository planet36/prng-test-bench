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
#include <cstddef>
#include <cstdint>
#include <format>
#include <print>
#include <string>
#include <string_view>
#include <vector>

constexpr int max_calls = 48;

// The number of calls shown in the per-call rows
constexpr int shown_calls = 20;

// An output looks filled when at least this fraction of its bits is set, or, compared with the
// output before it, at least this fraction of its bits changed.
constexpr double threshold = 0.40;

/// A PRNG whose state can be reset to all zeros, without a warm-up
template <typename G>
struct zero_start : G
{
    using state_type = G::state_type;

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
    using state_type = G::state_type;

    iota_start() : G(state_type{}) {}

    /// Set the state to what init has before its warm-up when it replaces an all-zero seed
    void
    reset_state()
    {
        for (size_t i = 0; i < std::size(this->s); ++i)
        {
            this->s[i] = static_cast<state_type::value_type>(i + 1);
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
[[nodiscard]] constexpr int
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

/// What the survey measured for one PRNG
struct survey_result
{
    std::string_view name;
    std::string_view start_name;

    /// popcount[k] is the fraction of bits set in output k
    series popcount{};

    /// change[k] is the fraction of bits that differ between output k and output k - 1, so it
    /// starts at call 1
    series change{};

    int popcount_discards = 0;
    int change_discards = 0;
};

template <typename Start>
[[nodiscard]] survey_result
survey(std::string_view name, std::string_view start_name)
{
    Start g;
    g.reset_state();

    const int bits = sizeof(typename Start::result_type) * CHAR_BIT;

    survey_result r{.name = name, .start_name = start_name};
    uint64_t previous = 0;
    for (int k = 0; k < max_calls; ++k)
    {
        const auto output = static_cast<uint64_t>(g.next());
        r.popcount[k] = std::popcount(output) / static_cast<double>(bits);
        if (k > 0)
        {
            r.change[k] = std::popcount(output ^ previous) / static_cast<double>(bits);
        }
        previous = output;
    }
    r.popcount_discards = first_reaching(r.popcount, 0);
    r.change_discards = first_reaching(r.change, 1);
    return r;
}

/// One row of percentages for calls from 0, with a star after the call at \a marked
[[nodiscard]] std::string
format_row(std::string_view label, const series& values, int first, int marked)
{
    std::string row = std::format("  {:<8}", label);
    for (int k = 0; k < shown_calls; ++k)
    {
        if (k < first)
        {
            row += "  - ";
        }
        else
        {
            row += std::format("{:3.0f}{}", values[k] * 100, (k == marked) ? '*' : ' ');
        }
    }
    row.erase(row.find_last_not_of(' ') + 1);
    return row;
}

void
print_summary(const std::vector<survey_result>& results)
{
    std::println("Outputs to discard after each start state.  The count is the index of the");
    std::println("first output that looks filled:");
    std::println("  popcount  at least {:.0f}% of its bits are set", threshold * 100);
    std::println("  change    at least {:.0f}% of its bits differ from the output before it",
                 threshold * 100);
    std::println("The start state is all zeros (zero) or 1, 2, 3, ... (iota).\n");

    std::println("{:<22}  {:<5}  {:>8}  {:>6}", "PRNG", "start", "popcount", "change");
    std::println("{:-<22}  {:-<5}  {:->8}  {:->6}", "", "", "", "");
    for (const auto& r : results)
    {
        std::println("{:<22}  {:<5}  {:>8}  {:>6}", r.name, r.start_name, r.popcount_discards,
                     r.change_discards);
    }
}

void
print_details(const std::vector<survey_result>& results)
{
    std::println("\nPercent of bits set (popcount) and changed from the previous output");
    std::println("(change) for calls 0-{}.  A star marks the output that gives each count.\n",
                 shown_calls - 1);

    std::string header = std::format("{:<10}", "call");
    for (int k = 0; k < shown_calls; ++k)
    {
        header += std::format("{:3} ", k);
    }
    header.pop_back();
    std::println("{}", header);

    for (const auto& r : results)
    {
        std::println("\n{} ({})", r.name, r.start_name);
        std::println("{}", format_row("popcount", r.popcount, 0, r.popcount_discards));
        std::println("{}", format_row("change", r.change, 1, r.change_discards));
    }

    std::println("\n{}", header);
}

int
main()
{
    const std::vector<survey_result> results{
        survey<iota_start<xoroshiro128plusplus>>("xoroshiro128plusplus", "iota"),
        survey<iota_start<xoroshiro128starstar>>("xoroshiro128starstar", "iota"),
        survey<iota_start<xoroshiro1024plusplus>>("xoroshiro1024plusplus", "iota"),
        survey<iota_start<xoroshiro1024starstar>>("xoroshiro1024starstar", "iota"),
        survey<iota_start<xoshiro128plusplus>>("xoshiro128plusplus", "iota"),
        survey<iota_start<xoshiro128starstar>>("xoshiro128starstar", "iota"),
        survey<iota_start<xoshiro256plusplus>>("xoshiro256plusplus", "iota"),
        survey<iota_start<xoshiro256starstar>>("xoshiro256starstar", "iota"),
        survey<iota_start<xoshiro512plusplus>>("xoshiro512plusplus", "iota"),
        survey<iota_start<xoshiro512starstar>>("xoshiro512starstar", "iota"),
        survey<zero_start<sfc32>>("sfc32", "zero"),
        survey<zero_start<sfc64>>("sfc64", "zero"),
        survey<zero_start<biski64>>("biski64", "zero"),
    };

    print_summary(results);
    print_details(results);
}
