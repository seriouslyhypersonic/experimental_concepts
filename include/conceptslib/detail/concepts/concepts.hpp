/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * Acknowledgments: implementation strategy as suggested by Isabella Muerte
 *                  - GitHub: https://github.com/slurps-mad-rips
 */

#ifndef CONCEPTS_H
#define CONCEPTS_H

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
 * @concept Specifies that a type can be swapped
 * @details Specifies that lvalues of type T are swappable.
 * @note Standard library concept: core language concept
 */
template<class T>
CONCEPT Swappable = std::is_swappable_v<T>;

/**
 * @concept Specifies that two types can be swapped with each other [Incomplete]
 * @details Specifies that expressions of the type and value category encoded
 * by T and U are swappable with each other.
 * @note Standard library concept: core language concept
 * @todo CommonReference
 */
template<class T, class U>
CONCEPT SwappableWith = require<
std::is_swappable_with_v<T, T>,
std::is_swappable_with_v<U, U>,
std::is_swappable_with_v<T, U>,
std::is_swappable_with_v<U, T>
>;



//template<class T>
//EXP_CONCEPT CopyConstructible =
//        require<
//        MoveConstructible<T>,
//        Constructible<T, T&>, ConvertibleTo<T&, T>,
//        Constructible<T, const T&>, ConvertibleTo<const T&, T>,
//        Constructible<T, const T>, ConvertibleTo<const T, T>
//        >;

template<class T>
CONCEPT Pointer = std::is_pointer_v<T>;

/* Object concepts */

/**
 * @concept Specifies that an object of a type can be moved and swapped
 * @details Specifies that T is an object type that can moved (that is, it can
 * be move constructed, move assigned, and lvalues of type T can be swapped)
 * @note Standard library concept: object concept
 */
//template<class T>
//EXP_CONCEPT Movable =
//        require<
//        std::is_object_v<T>,
//        MoveConstructible<T>,
//        Assignable<T&, T>,
//        Swappable<T>
//        >;

/**
 * @concept Specifies that an object of a type can be copied, moved, and swapped
 * @details Specifies that T is an Movable object type that can also copied
 * (that is, it supports copy construction and copy assignment).
 * @note Standard library concept: object concept
 */
//template<class T>
//EXP_CONCEPT Copyable =
//        require<
//        CopyConstructible<T>,
//        Movable<T>,
//        Assignable<T&, const T&>
//        >;

/**
 * @concept Specifies that an object of a type can be copied, moved, swapped,
 * and default constructed
 * @details Specifies that a type is both copyable and default constructible.
 * It is satisfied by types that behave similarly to built-in types like int,
 * except that they need not support comparison with ==.
 * @note Standard library concept: object concept
 */
//template<class T>
//EXP_CONCEPT Semiregular =
//        require<
//        Copyable<T>,
//        DefaultConstructible<T>
//        >;



/* Comparison concepts */

/**
 * @concept Specifies that a type can be used in Boolean contexts
 * @details Specifies the requirements for a type usable in Boolean contexts.
 * For Boolean to be satisfied, the logical operators must have the usual
 * behavior (including short-circuiting).
 * @note Standard library concept: comparison concept
 */
//template <class B>
//EXP_CONCEPT Boolean =
//        require<
//        Movable<traits::remove_cvref_t<B>>,
//        ConvertibleTo<const std::remove_reference_t<B>&, bool>,
//
//        exists<ops::negation, B>,
//        ConvertibleTo<detected_t<ops::negation, B>, bool>,
//
//        exists<ops::logical_conjunction, B, const bool>,
//        Same<detected_t<ops::logical_conjunction, B, const bool>, bool>,
//
//        exists<ops::logical_disjuntion, B, const bool>,
//        Same<detected_t<ops::logical_disjuntion, B, const bool>, bool>,
//
//        exists<ops::logical_conjunction, B, B>,
//        Same<detected_t<ops::logical_conjunction, B, B>, bool>,
//
//        exists<ops::logical_conjunction, const bool, B>,
//        Same<detected_t<ops::logical_conjunction, const bool, B>, bool>,
//
//        exists<ops::logical_disjuntion, B, B>,
//        Same<detected_t<ops::logical_disjuntion, B, B>, bool>,
//
//        exists<ops::logical_disjuntion, const bool, B>,
//        Same<detected_t<ops::logical_disjuntion, const bool, B>, bool>,
//
//        exists<ops::equal_to, B, B>,
//        Same<detected_t<ops::equal_to, B, B>, bool>,
//
//        exists<ops::equal_to, B, const bool>,
//        Same<detected_t<ops::equal_to, B, const bool>, bool>,
//
//        exists<ops::equal_to, const bool, B>,
//        Same<detected_t<ops::equal_to, const bool, B>, bool>,
//
//        exists<ops::not_equal_to, B, B>,
//        Same<detected_t<ops::not_equal_to, B, B>, bool>,
//
//        exists<ops::not_equal_to, B, const bool>,
//        Same<detected_t<ops::not_equal_to, B, const bool>, bool>,
//
//        exists<ops::not_equal_to, const bool,  B>,
//        Same<detected_t<ops::not_equal_to, const bool, B>, bool>
//        >;


//concept EqualityComparable = converts_to<bool, ops::equal_to, T, U>;
//namespace detail
//{
//template<class T, class U>
//EXP_CONCEPT WeaklyEqualityComparable =
//        require<
//        exists<ops::equal_to ,T, U>,
//        Boolean<detected_t<ops::equal_to, T, U>>,
//
//        exists<ops::not_equal_to, T, U>,
//        Boolean<detected_t<ops::not_equal_to, T, U>>,
//
//        exists<ops::equal_to , U, T>,
//        Boolean<detected_t<ops::equal_to, U, T>>,
//
//        exists<ops::not_equal_to, U, T>,
//        Boolean<detected_t<ops::not_equal_to, U, T>>
//        >;
//} // namespace detail

/**
 * @concept Specifies that operator == is an equivalence relation
 * @details Specifies that the comparison operators == and != on T reflects
 * equality: == yields true if and only if the operands are equal.
 * @note Standard library concept: comparison concept
 */
//template<class T>
//EXP_CONCEPT EqualityComparable = detail::WeaklyEqualityComparable<T, T>;

/**
 * @concept Specifies that operator == is an equivalence relation [Incomplete]
 * @details Specifies that the comparison operators == and != on (possibly
 * mixed) T and U operands yield results consistent with equality.
 * @note Standard library concept: comparison concept
 * @todo Ensure thet comparing mixed operands yields results equivalent to
 * comparing the operands converted to their common type.
 */
//template<class T, class U>
//EXP_CONCEPT EqualityComparableWith =
//        require<
//        EqualityComparable<T>,
//        EqualityComparable<U>,
//        detail::WeaklyEqualityComparable<T, U>
//        >;
} // namespace concepts


#endif //CONCEPTS_H
