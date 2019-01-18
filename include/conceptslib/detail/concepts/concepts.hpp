/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * Acknowledgments: Implementation strategy as suggested by Isabella Muerte
 *                  - GitHub: https://github.com/slurps-mad-rips
 *
 *                  Emulation of requires expression similar to that of Tristan
 *                  Brindle
 *                  - GitHub: https://github.com/tcbrindle/NanoRange
 */

#ifndef DETAIL_CONCEPTS_H
#define DETAIL_CONCEPTS_H

#define CONCEPT constexpr bool
#define REQUIREMENT struct
#define REQUIRES requires_fn

#include <experimental/type_traits>
#include <type_traits>

#include <conceptslib/type_traits.hpp>

namespace concepts
{
using traits::is_detected_v;
using traits::detected_t;

using traits::is_detected_exact_v;
using traits::is_detected_convertible_v;

/* --- Requires expression emulation --- */
namespace detail
{
template<class... Ts>
void valid_expr(Ts&&...);

// Generate an instantiatable return type if Exp result is true
template<bool Exp>
auto valid_if() -> std::enable_if_t<Exp, int>;

// Check if the Req's requires_fn function can be called with Args...
template<class Req, class... Args
        ,typename = decltype(&Req:: template requires_fn<Args...>)>
void test_requires(Req&);

template<class Req, class... Args>
using test_requires_t =
    decltype(test_requires<Req, Args...>(std::declval<Req&>()));

} // namespace detail

/**
 * Check if a requirement Req has a valid \c requires_ member function
 * @tparam Req The requirement with a \c requires_member function
 * @tparam Args The parameter types of the \c requires_ member function
 * @details Req is requirement class with a requires_ member function
 * declaration (does not need implementation) where a trailing return type may
 * be used to test the validity of a sequence of requirements.
 */
template<class Req, class... Args>
const bool requires_ = is_detected_v<detail::test_requires_t, Req, Args...>;

/* Special names to use with Concepts */
// Most likely unecessary
template<bool... bools>
constexpr bool require = std::conjunction<std::bool_constant<bools>...>::value;

template<bool... bools>
constexpr bool either = std::disjunction<std::bool_constant<bools>...>::value;

template<bool... bools>
constexpr bool disallow = !require<bools...>;

template<template <class...> class Op, class... Args>
constexpr bool exists = is_detected_v<Op, Args...>;

template<class To, template <class...> class Op, class... Args>
constexpr bool converts_to = is_detected_convertible_v<To, Op, Args...>;

template <class Exact, template <class...> class Op, class... Args>
constexpr bool identical_to = is_detected_exact_v<Exact, Op, Args...>;

/* Operator detectors */

namespace ops
{
using std::declval;

template<class T, class U>
using equal_to = decltype(declval<T>() == declval<U>());

template<class T, class U>
using not_equal_to = decltype(declval<T>() != declval<U>());

template<class T, class U>
using less = decltype(declval<T> < declval<U>());

template<class T, class U>
using less_equal = decltype(declval<T> <= declval<U>());

template<class T, class U>
using greater = decltype(declval<T> > declval<U>());

template<class T, class U>
using greater_equal = decltype(declval<T> >= declval<U>());

template<class T>
using negation = decltype(!declval<T>());

template<class T, class U = T>
using logical_conjunction = decltype(declval<T>() && declval<U>());

template<class T, class U = T>
using logical_disjuntion = decltype(declval<T>() || declval<U>());

template<class T> using derreference = decltype(*declval<T>());
template<class T> using arrow = decltype(declval<T>().operator->());

template<class T> using postfix_increment = decltype(declval<T>()++);
template<class T> using prefix_increment = decltype(++declval<T>());

} // namespace ops

/* Aliases detectors */

namespace aliases
{
template<class T> using value_type = typename T::value_type;
template<class T> using size_tye = typename T::size_type;
template<class T> using allocator_type = typename T::allocator_type;
}

/* Concepts */

//using std::declval;

/* --- Standard library concepts (tries to model the concepts library ) --- */

/* Core language concepts */
/**
 * @concept Specifies that two types can be swapped with each other [Incomplete]
 * @details Specifies that expressions of the type and value category encoded
 * by T and U are swappable with each other.
 * @note Standard library concept: core language concept
 * @todo CommonReference
 */
//template<class T, class U>
//CONCEPT SwappableWith = require<
//std::is_swappable_with_v<T, T>,
//std::is_swappable_with_v<U, U>,
//std::is_swappable_with_v<T, U>,
//std::is_swappable_with_v<U, T>
//>;
} // namespace concepts


#endif //DETAIL_CONCEPTS_H
