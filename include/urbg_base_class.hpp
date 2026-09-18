// SPDX-FileCopyrightText: Steven Ward
// SPDX-License-Identifier: MPL-2.0

/// Uniform Random Bit Generator base class
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

#include <array>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <stdlib.h> // arc4random_buf
#include <string.h> // explicit_bzero
#include <type_traits>

/// The requirements that a \c URBG_base state type must meet
/**
* \c arc4random_buf fills the state, \c std::memcpy copies seed bytes into it, and
* \c explicit_bzero erases it, and none of those is defined for a type that is not
* trivially copyable.  The state member is also value initialized where it is declared.
*/
template <typename T>
concept urbg_state = std::is_trivially_copyable_v<T> && std::default_initializable<T>;

/// Uniform Random Bit Generator base class
/**
* \tparam S the state type
* \tparam R the result type
*
* Some random number engines have criteria for their initial state.
* For example, the state must not be 0, or a particular element must be odd.
* In the derived class, implement \c init to prepare the initial state
* accordingly.  Every constructor that \c DEF_URBG_SUBCLASS defines calls it.
*
* No member of this class is virtual, so a call to \c next is dispatched statically and
* can be inlined.  A virtual \c next would also leave every instantiation of a template
* that drives a generator identical before inlining.  GCC's identical code folding then
* merges those instantiations and speculates on a single call target, which costs every
* other generator in the merged group an indirect call per output.
*/
template <urbg_state S, std::unsigned_integral R>
struct URBG_base
{
public:
    using state_type = S;
    using result_type = R;

    // https://eel.is/c++draft/rand.req.eng#3.1
    static_assert(sizeof(state_type) % sizeof(result_type) == 0);

    using seed_bytes_type = std::array<std::byte, sizeof(state_type)>;

    static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }

    static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }

protected:
    state_type s{};

public:
    // ctors

    URBG_base() { arc4random_buf(std::addressof(s), sizeof(s)); }

    explicit URBG_base(const state_type& new_s) : s(new_s) {}

    explicit URBG_base(const seed_bytes_type& bytes)
    {
        (void)std::memcpy(std::addressof(s), std::data(bytes), sizeof(state_type));
    }

    URBG_base(const URBG_base&) = default;
    URBG_base& operator=(const URBG_base&) = default;

    URBG_base(URBG_base&&) = default;
    URBG_base& operator=(URBG_base&&) = default;

protected:
    /// dtor
    /**
    * This is protected and not virtual because the class is a base for its constructors
    * and its state, not for run-time polymorphism.  Deleting a PRNG through a pointer to
    * this class would be undefined, and a protected dtor makes that a compile error.
    */
    ~URBG_base()
    {
        // zeroize the state
        // https://sourceware.org/glibc/manual/latest/html_node/Erasing-Sensitive-Data.html
        explicit_bzero(std::addressof(s), sizeof(state_type));
    }
};

// https://stackoverflow.com/a/13842612
#define SINGLE_ARG(...) __VA_ARGS__
// Use SINGLE_ARG when a macro arg has a comma.

// init and next are declared inline, so their definitions in a header can be
// included in more than one translation unit without violating the
// one-definition rule.
#define DEF_URBG_SUBCLASS(CLASS_NAME, STATE_TYPE, RESULT_TYPE)                              \
    struct CLASS_NAME : public URBG_base<STATE_TYPE, RESULT_TYPE>                           \
    {                                                                                       \
    protected:                                                                              \
        inline void init(); /* must implement this */                                       \
                                                                                            \
    public:                                                                                 \
        CLASS_NAME() { init(); }                                                            \
        explicit CLASS_NAME(const state_type& new_s) : URBG_base(new_s) { init(); }         \
        explicit CLASS_NAME(const seed_bytes_type& bytes) : URBG_base(bytes) { init(); }    \
        inline result_type next(); /* must implement this */                                \
        [[nodiscard]] result_type operator()() { return next(); }                           \
    };                                                                                      \
    static_assert(std::uniform_random_bit_generator<CLASS_NAME>);
