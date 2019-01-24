/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef DETAIL_CALLABLE_H
#define DETAIL_CALLABLE_H

#include <functional>
#include <utility>

#include <conceptslib/detail/concepts/concepts.hpp>

#ifdef __clang__
    #define SUPPORTS_STD_INVOKE (__clang_major__ > 6)
#else
    #define SUPPORTS_STD_INVOKE true
#endif

#if !SUPPORTS_STD_INVOKE
#include <conceptslib/detail/functional/invoke.hpp>
#endif

namespace concepts
{

/* --- Concept Invocable --- */

namespace detail
{
REQUIREMENT InvocableReq
{
#if SUPPORTS_STD_INVOKE
    // As spec'd by the concepts library:
    // Compiles fine with gcc and Clang 7 (but not 6)
    template<class F, class... Args>
    auto REQUIRES(F&& f, Args&&... args) -> decltype(
        std::invoke(std::forward<F>(f), std::forward<Args>(args)...)
    );
#else
    // Alternative implementation
    template<class F, class... Args>
    auto REQUIRES(F&&, Args&&... args) -> invoke_result_t<F, Args...>;
#endif
};

} // namespace detail
/**
 * @concept Specifies that a callable type F can be called with a set of
 * argument types Args... using the function template \c std::invoke.
 */
template<class F, class... Args>
CONCEPT Invocable = requires_<detail::InvocableReq, F, Args...>;

/* --- Concept RegularInvocable --- */
/**
 * @concept The RegularInvocable concept adds to the Invocable concept by
 * requiring the invoke expression to be equality preserving and not modify
 * either the function object or the arguments.
 * @note The distinction between Invocable and RegularInvocable is purely
 * semantic.
 */
template<class F, class... Args>
CONCEPT RegularInvocable = Invocable<F, Args...>;

/* --- Concept Predicate --- */
/**
 * @concept Specifies that a callable type is a Boolean predicate
 * @details Specifies that F is a predicate that accepts arguments whose types
 * and value categories are encoded by Args..., i.e., it can be invoked with
 * these arguments to produce a Boolean result.
 */
template<class F, class... Args>
CONCEPT Predicate =
    RegularInvocable<F, Args...> &&
    Boolean<traits::detected_t<std::invoke_result_t, F, Args...>>;

/* --- Concept Relation --- */
/**
 * @concept Specifies that a callable type is a binary relation
 * @details Specifies that R defines a binary relation over the set of
 * expressions whose type and value category are those encoded by either T or U.
 */
template<class R, class T, class U>
CONCEPT Relation =
    Predicate<R, T, T> && Predicate<R, U, U> &&
    Predicate<R, T, U> && Predicate<R, U, T>;

/* --- Concept StrictWeakOrder --- */
/**
 * @concept Specifies that a Relation imposes a strict weak ordering
 * @details Specifies that the Relation R imposes a strict weak ordering on its
 * arguments.
 * @note The distinction between Relation and StrictWeakOrder is purely
 * semantic.
 * @note More details at
 * https://en.cppreference.com/w/cpp/concepts/StrictWeakOrder
 */
 template<class R, class T, class U>
 CONCEPT StrictWeakOrder = Relation<R, T, U>;

} // namespace concepts

#endif //DETAIL_CALLABLE_H
