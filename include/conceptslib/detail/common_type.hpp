/*
 * Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * Acknowledgments: implementation by Tristan Brindle with minimal changes to
 *                  reflect the C++17 interface for the detection idiom.
 *                  - Copyright  : Tristan Brindle
 *                  - Project URL: https://github.com/tcbrindle/NanoRange
 *                  - License    : BSL-1.0
 */

#ifndef IMPL_DETAIL_COMMON_TYPE_H
#define IMPL_DETAIL_COMMON_TYPE_H

#include <type_traits>

#include <conceptslib/detail/type_traits.hpp>
#include <conceptslib/detail/common_reference.hpp>

namespace traits
{

template <class...>
struct common_type;

template<class... Ts>
using common_type_t = typename common_type<Ts...>::type;

namespace detail
{

template<class T, class U>
constexpr bool same_decayed_v =
    std::is_same_v<T, std::decay_t<T>> &&
    std::is_same_v<U, std::decay_t<U>>;

template<class T, class U>
using ternary_return_t =
    std::decay_t<decltype(std::declval<bool>() ? std::declval<T>()
                                               : std::declval<U>() )>;

template<class, class, class = void>
struct binary_common_type { };

template<class T, class U>
struct binary_common_type<T, U
                         ,std::enable_if_t<!same_decayed_v<T, U>>>
    : common_type<std::decay_t<T>, std::decay_t<U>>
{ };

template<class T, class U>
struct binary_common_type<
    T, U ,std::enable_if_t<same_decayed_v<T, U> &&
                           is_detected_v<ternary_return_t, T, U>>>
{
    using type = ternary_return_t<T, U>;
};

using traits::cref_t;
template<class T, class U>
struct binary_common_type<
    T, U, std::enable_if_t<same_decayed_v<T, U> &&
                           !is_detected_v<ternary_return_t, T, U> &&
                           is_detected_v<cond_res_t, cref_t<T> ,cref_t<U>>>>
{
    using type = std::decay_t<cond_res_t<cref_t<T>, cref_t<U>>>;
};

} // namespace detail

template<>
struct common_type<>
{ };

template<class T>
struct common_type<T>: common_type<T, T>
{ };

template<class T, class U>
struct common_type<T, U>: detail::binary_common_type<T, U>
{ };

namespace detail
{

template<class Void, typename...>
struct multiple_common_type
{ };

template<class T1, class T2, class... Rest>
struct multiple_common_type<std::void_t<common_type<T1, T2>>, T1, T2, Rest...>
    : common_type<common_type_t<T1, T2>, Rest...>
{ };

} // namespace detail

template<class T1, class T2, class... Rest>
struct common_type<T1, T2, Rest...>:
    detail::multiple_common_type<void, T1, T2, Rest...>
{ };

} // namespace traits

#endif //IMPL_DETAIL_COMMON_TYPE_H
