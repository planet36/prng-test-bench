// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/*

g++ -std=c++26 -O2 -march=native -I include warmup-survey.cpp -o warmup-survey && ./warmup-survey

Survey the PRNGs that discard outputs in init ("warm-up") and compare the counts found by eye
with counts derived from three measurements of the zero-seed sequence.

*/

// The PRNG classes are final, and deriving from them is the only way to rewind their state to
// what init had before the warm-up loop.  Keep this define out of every other program.
#pragma GCC diagnostic ignored "-Wkeyword-macro"
#define final

#include "prng.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <climits>
#include <cmath>
#include <concepts>
#include <cstdlib>
#include <format>
#include <print>
#include <string>
#include <string_view>
#include <vector>

constexpr int max_calls = 48;

// The number of consecutive calls that must meet a threshold before a measurement counts as
// settled, for the main table.
constexpr int default_window = 4;

/// One measurement per call: the value for the output produced after k discarded calls
using series = std::array<double, max_calls>;

struct survey_result
{
    std::string name;
    int warmup; // the count in init
    bool from_upstream;
    series output_popcount;
    series state_popcount;
    series avalanche;
};

template <typename G>
struct rewound : G
{
    using state_type = typename G::state_type;
    using result_type = typename G::result_type;

    rewound() : G(state_type{}) {}

    /// Replace the state, and reset any other member that the warm-up advanced
    void
    set_state(const state_type& new_s)
    {
        this->s = new_s;
        if constexpr (requires { this->p; })
            this->p = 0;
    }

    [[nodiscard]] const state_type&
    state() const
    {
        return this->s;
    }
};

template <typename T>
[[nodiscard]] int
state_bits(const T& s)
{
    return static_cast<int>(std::size(s) * sizeof(s[0]) * CHAR_BIT);
}

template <typename T>
[[nodiscard]] int
state_popcount(const T& s)
{
    int n = 0;
    for (const auto w : s)
        n += std::popcount(w);
    return n;
}

/// The state that init has before its warm-up loop when the seed is all zero
template <typename G>
[[nodiscard]] typename G::state_type
pre_warmup_state(bool zero_fix_is_iota)
{
    typename G::state_type s{};
    if (zero_fix_is_iota)
        for (size_t i = 0; i < std::size(s); ++i)
            s[i] = static_cast<typename G::state_type::value_type>(i + 1);
    return s;
}

template <typename G>
[[nodiscard]] survey_result
survey(std::string_view name, int warmup, bool zero_fix_is_iota, bool from_upstream = false)
{
    survey_result r{std::string(name), warmup, from_upstream, {}, {}, {}};

    const auto start = pre_warmup_state<G>(zero_fix_is_iota);
    const int rbits = sizeof(typename G::result_type) * CHAR_BIT;

    rewound<G> g;
    g.set_state(start);
    std::array<uint64_t, max_calls> outputs{};
    for (int k = 0; k < max_calls; ++k)
    {
        r.state_popcount[k] = state_popcount(g.state()) / double(state_bits(g.state()));
        outputs[k] = static_cast<uint64_t>(g());
        r.output_popcount[k] = std::popcount(outputs[k]) / double(rbits);
    }

    // Flip each bit of the starting state in turn, and average the fraction of output bits that
    // change at each call.
    const int sbits = state_bits(start);
    const int word_bits = sizeof(start[0]) * CHAR_BIT;
    series changed{};
    for (int bit = 0; bit < sbits; ++bit)
    {
        auto flipped = start;
        flipped[bit / word_bits] ^= static_cast<typename G::state_type::value_type>(
            typename G::state_type::value_type{1} << (bit % word_bits));
        rewound<G> h;
        h.set_state(flipped);
        for (int k = 0; k < max_calls; ++k)
            changed[k] +=
                std::popcount(outputs[k] ^ static_cast<uint64_t>(h())) / double(rbits);
    }
    for (int k = 0; k < max_calls; ++k)
        r.avalanche[k] = changed[k] / sbits;

    return r;
}

/// The first call index from which window consecutive values are all at least threshold
[[nodiscard]] int
settled(const series& values, double threshold, int window = default_window)
{
    for (int k = 0; k + window <= max_calls; ++k)
        if (std::all_of(values.begin() + k, values.begin() + k + window,
                        [=](double v) { return v >= threshold; }))
            return k;
    return max_calls;
}

struct fit
{
    double threshold;
    int total_error;
    std::vector<int> counts;
};

/// Find the threshold that best reproduces the empirical warm-up counts
[[nodiscard]] fit
best_fit(const std::vector<survey_result>& results, series survey_result::*member)
{
    fit best{0.0, 1 << 30, {}};
    for (int t = 20; t <= 50; ++t)
    {
        const double threshold = t / 100.0;
        fit f{threshold, 0, {}};
        for (const auto& r : results)
        {
            const int count = settled(r.*member, threshold);
            f.counts.push_back(count);
            if (!r.from_upstream)
                f.total_error += std::abs(count - r.warmup);
        }
        if (f.total_error < best.total_error)
            best = f;
    }
    return best;
}

int
main()
{
    const std::vector<survey_result> results{
        survey<xoroshiro128plusplus>("xoroshiro128plusplus", 2, true),
        survey<xoroshiro128starstar>("xoroshiro128starstar", 4, true),
        survey<xoroshiro1024plusplus>("xoroshiro1024plusplus", 8, true),
        survey<xoroshiro1024starstar>("xoroshiro1024starstar", 22, true),
        survey<xoshiro128plusplus>("xoshiro128plusplus", 4, true),
        survey<xoshiro128starstar>("xoshiro128starstar", 4, true),
        survey<xoshiro256plusplus>("xoshiro256plusplus", 4, true),
        survey<xoshiro256starstar>("xoshiro256starstar", 6, true),
        survey<xoshiro512plusplus>("xoshiro512plusplus", 8, true),
        survey<xoshiro512starstar>("xoshiro512starstar", 8, true),
        survey<sfc32>("sfc32", 6, false),
        survey<sfc64>("sfc64", 9, false),
        survey<biski64>("biski64", 16, false, true),
    };

    struct named_fit
    {
        std::string_view label;
        fit f;
    };
    const std::array fits{
        named_fit{"output popcount", best_fit(results, &survey_result::output_popcount)},
        named_fit{"state popcount", best_fit(results, &survey_result::state_popcount)},
        named_fit{"avalanche", best_fit(results, &survey_result::avalanche)},
    };

    std::println("Settled = first call from which {} consecutive values reach the threshold.",
                 default_window);
    std::println("Error excludes biski64, whose count comes from upstream.\n");
    for (const auto& [label, f] : fits)
        std::println("{:<16} best threshold {:.2f}  total error {}", label, f.threshold,
                     f.total_error);

    // A fixed rule for comparison: discard outputs until the first one with at least 40% of its
    // bits set.
    constexpr double simple_threshold = 0.40;
    int simple_error = 0;

    std::println("\n{:<22} {:>7}  {:>10}  {:>10}  {:>10}  {:>13}", "PRNG", "warm-up",
                 "output pop", "state pop", "avalanche", "first >= 40%");
    for (size_t i = 0; i < results.size(); ++i)
    {
        const int simple = settled(results[i].output_popcount, simple_threshold, 1);
        if (!results[i].from_upstream)
            simple_error += std::abs(simple - results[i].warmup);
        std::println("{:<22} {:>6}{}  {:>10}  {:>10}  {:>10}  {:>13}", results[i].name,
                     results[i].warmup, results[i].from_upstream ? "*" : " ",
                     fits[0].f.counts[i], fits[1].f.counts[i], fits[2].f.counts[i], simple);
    }
    std::println("total error of the first-output-with-40%-set rule: {}", simple_error);
    std::println("* from upstream");

    std::println("\nTotal error of output popcount by threshold (rows) and window (columns):");
    std::println("{:>9} {:>5} {:>5} {:>5} {:>5}", "threshold", 1, 2, 4, 8);
    for (int t = 30; t <= 50; t += 2)
    {
        std::string line;
        for (const int w : {1, 2, 4, 8})
        {
            int error = 0;
            for (const auto& r : results)
                if (!r.from_upstream)
                    error += std::abs(settled(r.output_popcount, t / 100.0, w) - r.warmup);
            line += std::format(" {:>5}", error);
        }
        std::println("{:>9.2f}{}", t / 100.0, line);
    }

    std::println("\nAvalanche at calls 0-11 (fraction of output bits changed by one state bit):");
    for (const auto& r : results)
    {
        std::string line;
        for (int k = 0; k < 12; ++k)
            line += std::format(" {:.2f}", r.avalanche[k]);
        std::println("{:<22}{}", r.name, line);
    }
}
