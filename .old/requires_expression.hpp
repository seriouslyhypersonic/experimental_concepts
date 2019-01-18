/*
 * Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * Copyright Duncan P. N. Exon Smith
 * URL: https://gist.github.com/dexonsmith
 */

#ifndef REQUIRES_EXPRESSION_H
#define REQUIRES_EXPRESSION_H

/**
 * @file requires_expresasion.hpp
 *
 * Defines a requires_expression function that can be used to emulate C++20
 * requires. The function argument is a generic lambda. The function is then
 * called for the template arguments and returns true if valid; false otherwise.
 */

/* --- Requires Function --- */

#include <utility>

namespace detail {

/// Check if any type in the list is void
template<class... Ts>
struct any_voids: std::disjunction<std::is_void<Ts>...> {};

template <class... Ts>
constexpr bool any_voids_v = any_voids<Ts...>::value;

} // namespace detail

namespace detail {
/** Selected iff F(Args...) is valid; returns true.
 *
 * Uses the same technique as \c hana::is_valid:
 *   http://boostorg.github.io/hana/index.html#tutorial-introspection-is_valid
 *
 * Which is similar to std::experimental::is_detected:
 * http://en.cppreference.com/w/cpp/experimental/is_detected
 */
template <class F, class... Args,
          class = decltype(std::declval<F&&>()(std::declval<Args&&>()...))>
constexpr bool requires_impl(int) {
    return true;
}

/// Selected iff F(Args...) is invalid; returns false.
template <class F, class... Args>
constexpr bool requires_impl(...) {
    return false;
}

} // namespace detail

/// Return true iff F(Args...) is valid.
template <class... Args, class F>
constexpr bool reqexpr(F&&) {
    // Only evaluate if there aren't any voids.  void cannot be passed as an
    // argument, so requires_impl would give a compile error instead of returning
    // false.
    if constexpr (!detail::any_voids_v<Args...>) {
        return detail::requires_impl<F&&, Args&&...>(int{});
    }

    return false;
}

#endif //REQUIRES_EXPRESSION_H
