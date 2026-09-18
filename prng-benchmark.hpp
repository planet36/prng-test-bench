// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// Helpers shared by the PRNG benchmark programs
/**
* \file
* \author Steven Ward
*/

#pragma once

#include "parse_int.hpp"
#include "seed_seq.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <err.h>
#include <exception>
#include <random>
#include <thread>

/// A PRNG declared with \c DEF_URBG_SUBCLASS
/**
* Only these have \c seed_bytes_type.  The \c std engines do not.
*/
template <typename T>
concept my_urbg = std::uniform_random_bit_generator<T> && requires {
    typename T::seed_bytes_type;
};

/// Construct a randomly seeded \c std engine
template <typename URBG>
requires std::uniform_random_bit_generator<URBG> && (!my_urbg<URBG>)
URBG
make_random_seeded()
{
    return random_device_seeded<URBG>();
}

/// Construct a randomly seeded PRNG of mine
/**
* The default constructor fills the state from \c arc4random_buf.
*/
template <my_urbg URBG>
URBG
make_random_seeded()
{
    return URBG{};
}

/// Get the number of threads to run each benchmark on
/**
* The count is read from the environment variable \c NUM_THREADS, where 0 means one thread
* per hardware thread.  When \c NUM_THREADS is not set, the count is 1.
*
* The program exits if \c NUM_THREADS is not an integer in the interval from 0 to the number
* of hardware threads.
*/
inline int
get_num_threads()
{
    constexpr int min_threads = 1;
    const auto hw_threads = static_cast<int>(std::thread::hardware_concurrency());
    const auto max_threads = std::max(min_threads, hw_threads);

    int num_threads = min_threads;

    try
    {
        // parse_env_int takes these args: name, min, max, default
        num_threads = parse_env_int("NUM_THREADS", 0, max_threads, min_threads);
    }
    catch (const std::exception& ex)
    {
        (void)std::fflush(stdout);
        errx(EXIT_FAILURE, "%s", ex.what());
    }

    if (num_threads == 0)
    {
        num_threads = max_threads;
    }

    return num_threads;
}
