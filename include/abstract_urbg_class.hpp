// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: OSL-3.0

/// Abstract Uniform Random Bit Generator class
/**
* \file
* \author Steven Ward
* \sa https://en.cppreference.com/w/cpp/named_req/UniformRandomBitGenerator
* \sa https://en.cppreference.com/w/cpp/named_req/RandomNumberEngine
* \sa https://eel.is/c++draft/rand.req.urng
* \sa https://eel.is/c++draft/rand.req.eng
* Note: This does not meet the requirements of a random number engine.
* The default ctor does not create an engine with the same initial state as all
* other default-constructed engines of the same type.
*/

#pragma once

#include "fill_rand.hpp"

#include <array>
#include <concepts>
#include <cstdint>
#include <cstring>
#include <limits>

/// Abstract Uniform Random Bit Generator class
/**
* Some random number engines have criteria for their initial state.
* For example, the state must not be 0, or a particular element must be odd.
* In the derived class, be sure to override the constructors to prepare the
* initial state accordingly.
*/
template <typename S, std::unsigned_integral R>
struct AbstractURBG
{
public:

    using state_type = S;
    using result_type = R;

    // https://eel.is/c++draft/rand.req.eng#3.1
    static_assert(sizeof(state_type) % sizeof(result_type) == 0);

    using seed_bytes_type = std::array<uint8_t, sizeof(state_type)>;

    static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }

    static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }

protected:

    state_type s{};

public:

    // ctors

    AbstractURBG() { fill_rand(s); }

    explicit AbstractURBG(const state_type& new_s) : s(new_s) { }

    explicit AbstractURBG(const seed_bytes_type& bytes)
    {
        (void)std::memcpy(&s, std::data(bytes), sizeof(state_type));
    }

    AbstractURBG(const AbstractURBG&) = default;

    AbstractURBG& operator=(const AbstractURBG&) = default;

    /// dtor
    virtual ~AbstractURBG()
    {
        // zeroize the state
        (void)std::memset(&s, 0, sizeof(state_type));
    }

    virtual result_type next() = 0; // XXX: must override this

    result_type operator()() { return next(); }
};
