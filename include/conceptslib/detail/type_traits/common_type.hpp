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

#ifndef DETAIL_COMMON_TYPE_H
#define DETAIL_COMMON_TYPE_H

#include <type_traits>

#include <conceptslib/detail/type_traits/type_traits.hpp>
#include <conceptslib/detail/type_traits/common_reference.hpp>

namespace traits
{
template <class...>
struct common_type;

/// Helper typedef to access \c common_type member \c type
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

using traits::clref_t;
template<class T, class U>
struct binary_common_type<
    T, U, std::enable_if_t<same_decayed_v<T, U> &&
                           !is_detected_v<ternary_return_t, T, U> &&
                           is_detected_v<cond_res_t, clref_t<T> ,clref_t<U>>>>
{
    using type = std::decay_t<cond_res_t<clref_t<T>, clref_t<U>>>;
};

} // namespace detail

template<>
struct common_type<>
{ };

/**
 * @metafunction Determines the common type of T0
 * @details Determines the type T0 be implicitly converted to. If such type
 * exists, the member \c type names that type. Otherwise, there is no member
 * \c type.
 * @note The rules for determining the common_type can be found at
 * https://en.cppreference.com/w/cpp/types/common_type
 * @attention This metafunction mimics the additional functionality of the
 * future C++20 \c std::common_type.
 * @attention Specializations for this implementation of \c common_type
 * should be done inside the library's namespace
 */
template<class T0>
struct common_type<T0>: common_type<T0, T0>
{ };

/**
 * @metafunction Determines the common type among T1 and T2
 * @details Determines the type T1 and T2 can be implicitly converted to. If
 * such type exists, the member \c type names that type. Otherwise, there is no
 * member \c type.
 * @note The rules for determining the common_type can be found at
 * https://en.cppreference.com/w/cpp/types/common_type
 * @attention This metafunction mimics the additional functionality of the
 * future C++20 \c std::common_type.
 * @attention Specializations for this implementation of \c common_type
 * should be done inside the library's namespace
 */
template<class T1, class T2>
struct common_type<T1, T2>: detail::binary_common_type<T1, T2>
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

/**
 * @metafunction Determines the common type among all types Ts...
 * @details Determines the type all Ts... can be implicitly converted to. If
 * such type exists, the member \c type names that type. Otherwise, there is no
 * member \c type.
 * @note The rules for determining the common_type can be found at
 * https://en.cppreference.com/w/cpp/types/common_type
 * @attention This metafunction mimics the additional functionality of the
 * future C++20 \c std::common_type.
 * @attention Specializations for this implementation of \c common_type
 * should be done inside the library's namespace
 */
template<class T1, class T2, class... Rest>
struct common_type<T1, T2, Rest...>:
    detail::multiple_common_type<void, T1, T2, Rest...>
{ };

} // namespace traits

#endif //DETAIL_COMMON_TYPE_H
