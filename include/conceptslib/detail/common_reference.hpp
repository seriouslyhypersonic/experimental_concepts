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

#ifndef DETAIL_COMMON_REFERENCE_H
#define DETAIL_COMMON_REFERENCE_H

#include <utility>
#include <type_traits>

#include <conceptslib/detail/type_traits.hpp>

namespace traits
{

template <typename ...>
struct common_type;

namespace detail
{

// Create a const lvalue reference from the underlying type of T.
template<class T>
using clref_t = std::add_lvalue_reference_t<const std::remove_reference_t<T>>;

// Create an rvalue reference from a reference type T. Otherwise, use T.
template <class T>
struct rref
{
    using type = T;
};

template<class T>
struct rref<T&>
{
    using type = std::remove_reference_t<T>&&;
};

template <class T>
using rref_t = typename rref<T>::type;

// req for builtin common between return values of T and U returning functions
template<class T, class U>
using cond_res_t = decltype(std::declval<bool>() ? std::declval<T(&)()>()()
                                                 : std::declval<U(&)()>()() );

/* --- Metafunction simple_common_reference ---*/
// Base case: failure
template<class T, class U>
struct simple_common_reference { };

// Case 1: T and U are lvalue references and the built in common exists for
//         cv12 T and cv12 U, where cv12 is the union of their cv qualifiers
//         and that type is a reference
template<class T, class U
        ,class C = detected_t<cond_res_t, copy_cv_t<T, U>&, copy_cv_t<U, T>&>>
struct lvalue_simple_common_reference
    : std::enable_if<std::is_reference_v<C>, C>
{ };

template<class T, class U>
using lvalue_scr_t = typename lvalue_simple_common_reference<T, U>::type;

// Specialization for case 1
template<class T, class U>
struct simple_common_reference<T&, U&>: lvalue_simple_common_reference<T, U>
{ };

// Case 2: T and U are rvalue references and there is an lvalue simple common
//         reference for T and U. C is that type's rvalue reference. If both
//         T and U are convertible to C, C is the simple common reference.
template<class T, class U, class LCR = detected_t<lvalue_scr_t, T, U>
        ,class C = rref_t<LCR>>
struct rvalue_simple_common_reference:
    std::enable_if<std::is_convertible_v<T&&, C> &&
                   std::is_convertible_v<U&&, C>
                  ,C>
{ };

// Specialization for case 2
template<class T, class U>
struct simple_common_reference<T&&, U&&>: rvalue_simple_common_reference<T, U>
{ };

// Case 3: one of T and U is an lvalue reference type A&, and the other is an
//         rvalue reference type B&&. D is the simple common reference type of
//         A& and B const &. If D exists and B&& is convertible to D, it is the
//         simple common reference
template<class A, class B, class D = detected_t<lvalue_scr_t, A, const B>>
struct mixed_simple_common_reference:
    std::enable_if<std::is_convertible_v<B&&, D>, D>
{ };

// Specializations for the two combinations of case 3
template<class A, class B>
struct simple_common_reference<A&, B&&>: mixed_simple_common_reference<A, B>
{ };

template<class A, class B>
struct simple_common_reference<A&&, B&>: simple_common_reference<B&, A&&>
{ };

template<class T, class U>
using simple_common_reference_t = typename simple_common_reference<T, U>::type;

/* --- Metafunction xref--- */
// xref<A>::type<U> is U with the addition os A's cv and reference qualifiers
// Depending on A, whatever U is passed to xref<A>::type<U>, will yield type<U>
// case 1 - A and U not references     -> type<U>: cvab U
// case 2 - A lvalue ref and U not ref -> type<U>: cvab U&
// case 3 - A lvalue ref and U ref     -> type<U>: cvb  U& (collapsed)
// case 4 - A rvalue ref and U not ref -> type<U>: cvab U&&
// case 5 - A rvalue ref and U ref     -> type<U>: cvb  U&/&& (collapsed)
// Note: cvab is the union of cv qualifiers of A and B.
template <class>
struct xref { template <class U> using type = U; };

// Specialization for lvalue references
template<class A>
struct xref<A&>
{
    // Need :: template to treat type as a dependant template name
    template<class U>
    using type =
        std::add_lvalue_reference_t<typename xref<A>:: template type<U>>;
};

// Specialization for rvalue references
template<class A>
struct xref<A&&>
{
    template<class U>
    using type =
        std::add_rvalue_reference_t<typename xref<A>:: template type<U>>;
};

// Specialization for const values
template<class A>
struct xref<const A>
{
    template<class U>
    using type =
        std::add_const_t<typename xref<A>:: template type<U>>;
};

// Specialization for volatile values
template<class A>
struct xref<volatile A>
{
    template<class U>
    using type =
        std::add_volatile_t<typename xref<A>:: template type<U>>;
};

// Specialization for const volatile values
template<class A>
struct xref<const volatile A>
{
    template<class U>
    using type =
        std::add_cv_t<typename xref<A>:: template type<U>>;
};

} // namespace detail

/* --- Metafunction basic_common_reference --- */
/**
 * @metafunction A program may specialize \c basic_common_reference on the
 * first two parameters T and U if each is the same as its respective decayed
 * version and at least one of them depends on a program-defined type.
 * @details For more details on the specialization requirements, please visit
 * https://en.cppreference.com/w/cpp/types/common_reference
 * @attention A program may not specialize basic_common_reference on the third
 * or fourth parameters, nor may it specialize common_reference itself.
 * @attention This function mimics the functionality of the future C++20 \c
 * std::basic_common_reference.
 * @attention Specializations for this implementation of
 * \c basic_common_reference should be done inside the library's namespace
 */
// basic_common_reference: base case
template<class T, class U
        ,template<class> class TQual
        ,template<class> class UQual>
struct basic_common_reference { };

template<class...>
struct common_reference;

/// Helper typedef to access \c common_reference member \c type
template<class... Ts>
using common_reference_t = typename common_reference<Ts...>::type;

// common_reference: case sizeof...(Ts) is 0, no member type.
template<>
struct common_reference<>
{ };

// common_reference: case sizeof...(Ts) only contains type T0, member type is T0
/**
 * @metafunction Determines the common reference of type T0
 * @details If only one argument is suplied, the member \c type names the same
 * type as T0.
 * @note The rules for determining the common_reference can be found at
 * https://en.cppreference.com/w/cpp/types/common_reference
 * @attention This function mimics the functionality of the future C++20 \c
 * std::common_type.
 */
template<class T0>
struct common_reference<T0>
{
    using type = T0;
};

namespace detail
{

// common_reference: case T and U ref types and exists simple common reference
template<class T, class U>
constexpr bool has_simple_common_ref_v =
    is_detected_v<simple_common_reference_t, T, U>;

template<class T, class U>
using basic_common_ref_t =
    typename basic_common_reference<traits::remove_cvref_t<T>
                                   ,traits::remove_cvref_t<U>
                                   ,detail::xref<T>::template type
                                   ,detail::xref<U>::template type
                                   >::type;

// common_reference: case exists basic common reference
template<class T, class U>
constexpr bool has_basic_common_ref_v =
    is_detected_v<basic_common_ref_t, T, U>;

// common_reference: case exists cond_res_t
template<class T, class U>
constexpr bool has_cond_res_v = is_detected_v<cond_res_t, T, U>;

// common_reference: base case (there is a common type)
template<class T, class U, class = void>
struct binary_common_ref: common_type<T, U>
{ };

template<class T, class U>
struct binary_common_ref<T, U
                        ,std::enable_if_t<has_simple_common_ref_v<T, U>>>
    : simple_common_reference<T, U>
{ };

template<class T, class U>
struct binary_common_ref<T, U
                        ,std::enable_if_t<has_basic_common_ref_v<T, U> &&
                                         !has_simple_common_ref_v<T, U>>>
{
    using type = basic_common_ref_t<T, U>;
};

template<class T, class U>
struct binary_common_ref<T, U
                        ,std::enable_if_t<has_cond_res_v<T, U> &&
                                         !has_basic_common_ref_v<T, U> &&
                                         !has_simple_common_ref_v<T, U>>>
{
    using type = cond_res_t<T, U>;
};

} // namespace detail

/**
 * @metafunction Determines the common reference type of the types T1 and T2
 * @details Determines the type to which all Ts... can be converted or bound.
 * If such a type exists the member \c type names that type. Otherwise,
 * there is no member \c type.
 * @note The rules for determining the common_reference can be found at
 * https://en.cppreference.com/w/cpp/types/common_reference
 * @attention This function mimics the functionality of the future C++20 \c
 * std::common_type.
 */
template<class T1, class T2>
struct common_reference<T1, T2>: detail::binary_common_ref<T1, T2>
{ };

namespace detail
{

// base case: no common_reference
template<class Void, class T1, class T2, class... Rest>
struct multiple_common_reference
{ };

template<class T1, class T2, class... Rest>
struct multiple_common_reference<std::void_t<common_reference_t<T1, T2>>
                                ,T1, T2, Rest...>
    : common_reference<common_reference_t<T1, T2>, Rest...>
{ };

} // namespace detail

/**
 * @metafunction Determines the common reference type of the types Ts...
 * @details Determines the type to which all Ts... can be converted or bound.
 * If such a type exists the member \c type names that type. Otherwise,
 * there is no member \c type.
 * @note The rules for determining the common_reference can be found at
 * https://en.cppreference.com/w/cpp/types/common_reference
 * @attention This function mimics the functionality of the future C++20 \c
 * std::common_type.
 */
template<class T1, class T2, class... Rest>
struct common_reference<T1, T2, Rest...>
    : detail::multiple_common_reference<void, T1, T2, Rest...>
{ };

} // namespace traits

#endif //DETAIL_COMMON_REFERENCE_H
