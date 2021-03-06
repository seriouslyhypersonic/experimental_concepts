/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef DETAIL_TYPE_TRAITS_H
#define DETAIL_TYPE_TRAITS_H

#include <type_traits>
#include <experimental/type_traits>

namespace traits
{
/* --- Detection idiom --- */

/* Aliases to introduce detection essentials into the traits namespace */

/// Alias for true_type if Op<Args...> is valid. Otherwise alias for false_type.
template<template<class...> class Op, class... Args>
using is_detected = std::experimental::is_detected<Op, Args...>;

/**
 * Alias for Op<Args...> if it is well formed. Otherwise nonesuch.
 * @note Can be used for dependent lookups
 */
template<template<class...> class Op, class... Args>
using detected_t = std::experimental::detected_t<Op, Args...>;

/**
 * Alias for a struct with two member typedefs.
 * @typedef \c type Alias for Op<Args...> if it denotes a valid type. Otherwise
 *  alias for Default.
 * \@ypedef \c value_t equivalent to \c is_detected
 */
template< class Default, template<class...> class Op, class... Args>
using detected_or = std::experimental::detected_or<Default, Op, Args...>;

/* Aliases to introduce additional utilities into the traits namespace */

// Non instantiatable type to indicate detection failure
using nonesuch = std::experimental::nonesuch;

template<template<class...> class Op, class... Args>
constexpr bool is_detected_v = std::experimental::is_detected_v<Op, Args...>;

template<class Default, template<class...> class Op, class... Args >
using detected_or_t = std::experimental::detected_or_t<Default, Op, Args...>;

template <class Expected, template<class...> class Op, class... Args>
using is_detected_exact =
std::experimental::is_detected_exact<Expected, Op, Args...>;

template <class Expected, template<class...> class Op, class... Args>
constexpr bool is_detected_exact_v =
               std::experimental::is_detected_exact_v<Expected, Op, Args...>;

template <class To, template<class...> class Op, class... Args>
using is_detected_convertible =
std::experimental::is_detected_convertible<To, Op, Args...>;

template <class To, template<class...> class Op, class... Args>
constexpr bool is_detected_convertible_v =
               std::experimental::is_detected_convertible_v<To, Op, Args...>;

/* --- Metafunction clref --- */
/**
 * @metafunction Create a const lvalue referene from the underlying type of T.
 * @details If the type T is a reference type, provides the member typedef \c
 * type which is a const lvalue reference to the type refered to by T.
 * Otherwise, \c type is a const lvalue reference to T,
 */
 template<class T>
 struct clref
 {
     using type = std::add_lvalue_reference_t<const std::remove_reference_t<T>>;
 };

 /// Helper typedef to access \c clref member \c type.
 template<class T> using clref_t = typename clref<T>::type;

/* --- Metafunction remove_cvref --- */
/**
 *  @metafunction Remove topmost cv-qualifiers of T or of the type refered by it
 *  @details If the type T is a reference type, provides the member typedef
 *  \c type which is the type referred to by T with its topmost cv-qualifiers
 *  removed. Otherwise, \c type is T with its topmost cv-qualifiers removed.
 *  @note Same implementation of future std::remove_cvref (C++20)
 */
template<class T>
struct remove_cvref
{
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

/// Helper typedef to access remove_cvref member typedef \c type
template<class T>
using remove_cvref_t = typename remove_cvref<T>::type;

/* --- Metafunction copy_cv --- */
namespace detail
{
// Case: From has no cv qualifiers
template<class From, class To>
struct copy_cv_imp { using type = To; };
//Case: From has const qualifier
template<class From, class To>
struct copy_cv_imp <const From, To>: std::add_const<To> { };
// Case: From has volatile qualifier
template<class From, class To>
struct copy_cv_imp <volatile From, To>: std::add_volatile<To> { };
// Case: From has const volatile qualifier
template<class From, class To>
struct copy_cv_imp <const volatile From, To>: std::add_cv<To> { };

} // namespace detail
/**
 * @metafunctiion Copy cv qualifiers from type From to type To
 * @details Provides a member typedef \c type which is the same as T, except
 * the qualifiers from type From are added to type To (unless T is a function,
 * a reference, or already has this cv-qualifier)
 */
template<class From, class To>
using copy_cv = detail::copy_cv_imp<From, To>;

/// Helper typedef to access the member \c type of copy_cv
template<class From, class To>
using copy_cv_t = typename copy_cv<From, To>::type;

/* --- Metafunction select --- */
/**
 * \c condition is a wrapper for the \c select metafunction. It wraps the result
 * of a test, and the associated type of that test. It provides a member
 * typedef \c value for the result of the test, and a member typedef \c type
 * for the associated type.
 * @tparam B The result of the test
 * @tparam T Type associated with the test
 */
template<bool B, class T>
struct condition
{
    static constexpr bool value = B;
    using type = T;
};

/**
 * @metafunction Conditionally select a type.
 * @details Selects a type based on the result of a set of conditions. Each
 * condition is associated with the result of a test, and a type. Conditions
 * are provided using the \c condition wrapper. A default type may be provided
 * as the last argument. If no default type is provided, at least one condition
 * should yeld a successful test. If multiple conditions are true, the first
 * will be selected.
 * Provides the member typedef \c type associated with the first successful
 * test.
 */
template<class Head, class... Tail>
struct select: std::conditional_t<Head::value, Head, select<Tail...>>
{ };

/// Base case for metafunction \c select: Tail is a default type
template<class T>
struct select<T>
{
    using type = T;
};

/// Base case for metafunction \c select: Tail is a condition
template<bool B, class T>
struct select<condition<B, T>>
{
    // The last conditon must be true, otherwise a default type must be provided
    static_assert(B, "select failed because there isn't any true condition");
    using type = T;
};

/// Helper typedef to access \c select member \c type
template<class Head, class... Tail>
using select_t = typename select<Head, Tail...>::type;

// --- trait has_type
namespace detail
{
template<class T>
using has_type_imp = typename T::type;
} // namespace detail
/**
 * @trait Check if a type has the member typedef \c type
 * @details If type T has the member typedef \c type, provides the member
 * constant value which is equal to true. Otherwise, value is equal to false.
 */
template<class T>
struct has_type: std::bool_constant<is_detected_v<detail::has_type_imp, T>> {};

/// Helper variable template to access \c has_type member \c value
template<class T>
inline constexpr bool has_type_v = has_type<T>::value;

} // namespace traits


#endif //DETAIL_TYPE_TRAITS_H
