// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

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
#include <memory>

/// Abstract Uniform Random Bit Generator class
/**
* \tparam S the state type
* \tparam R the result type
*
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

    explicit AbstractURBG(const state_type& new_s) : s(new_s) {}

    explicit AbstractURBG(const seed_bytes_type& bytes)
    {
        (void)std::memcpy(std::addressof(s), std::data(bytes), sizeof(state_type));
    }

    AbstractURBG(const AbstractURBG&) = default;

    AbstractURBG& operator=(const AbstractURBG&) = default;

    /// dtor
    virtual ~AbstractURBG()
    {
        // zeroize the state
        // https://en.cppreference.com/w/c/string/byte/memset
        // https://www.gnu.org/software//gnulib/manual/html_node/memset_005fexplicit.html
        // https://sourceware.org/glibc/manual/latest/html_node/Erasing-Sensitive-Data.html
#if defined(memset_explicit)
        (void)memset_explicit(std::addressof(s), 0, sizeof(state_type));
#elif defined(explicit_bzero)
        explicit_bzero(std::addressof(s), sizeof(state_type));
#else
        (void)std::memset(std::addressof(s), 0, sizeof(state_type));
#endif
    }

    virtual result_type next() = 0; // XXX: must override this

    result_type operator()() { return next(); }
};

// https://stackoverflow.com/a/13842612
#define SINGLE_ARG(...) __VA_ARGS__
// Use SINGLE_ARG when a macro arg has a comma.

#define DEF_URBG_SUBCLASS(CLASS_NAME, STATE_TYPE, RESULT_TYPE)                              \
    struct CLASS_NAME final : public AbstractURBG<STATE_TYPE, RESULT_TYPE>                  \
    {                                                                                       \
    protected:                                                                              \
        void init(); /* must implement this */                                              \
                                                                                            \
    public:                                                                                 \
        CLASS_NAME() { init(); }                                                            \
        explicit CLASS_NAME(const state_type& new_s) : AbstractURBG(new_s) { init(); }      \
        explicit CLASS_NAME(const seed_bytes_type& bytes) : AbstractURBG(bytes) { init(); } \
        result_type next() override; /* must implement this */                              \
    };                                                                                      \
    static_assert(std::uniform_random_bit_generator<CLASS_NAME>);
