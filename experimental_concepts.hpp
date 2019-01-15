/*
 * Copyright Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef EXPERIMENTAL_CONCEPTS_H
#define EXPERIMENTAL_CONCEPTS_H

#define EXP_CONCEPT constexpr bool

#include <experimental/type_traits>
#include <type_traits>

#include "experimental_type_traits.hpp"

/**
 * @file experimental_concepts
 *
 * Resources:
 *  - https://izzys.casa/2016/09/implementing-concepts-in-cxx/
 *  - https://gist.github.com/dexonsmith/509d31e0960d45958035b2bffd37bfc5
 */

/* Special names to use with Concepts */

// conjunction is short-circuit, so inside a require<>, we can first check that
// an expression is not ill-formed, and then use it (e.g. apply a concept)
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

namespace concepts
{
using std::declval;

/* --- Standard library concepts (tries to model the concepts library ) --- */

/* Core language concepts */

// Same
/**
 * @concept Specifies that a type is the same as another type
 * @details Satisfied if and only if T and U denote the same type.
 * @note Standard library concept: core language concept
 */
template<class T, class U>
EXP_CONCEPT Same = std::is_same_v<T, U> && std::is_same_v<U, T>;

// --- DerivedFrom
/**
 * @concept Specifies that a type is derived from another type
 * @details Satisfied if and only if Base is a class type that is either
 * Derived or a public and unambiguous base of Derived, ignoring cv-qualifiers.
 * @note Standard library concept: core language concept
 */
template<class Derived, class Base>
EXP_CONCEPT DerivedFrom =
        std::is_base_of_v<Base, Derived> &&
        std::is_convertible_v<const volatile Derived* ,const volatile Base*>;

// --- ConvertibleTo
namespace detail // Convert a call to of type F = From(&)() to type To
{
template<class F, class To>
using ConvertibleTo = decltype(static_cast<To>(declval<F>()()));
}
/**
 * @concept Specifies that a type is implicitly convertible to
 * another type
 * @details Specifies that an expression of the type and value category
 * specified by From can be implicitly and explicitly converted to the type To,
 * and the two forms of conversion are equivalent.
 * @note Standard library concept: core language concept
 */
template<class From, class To>
EXP_CONCEPT ConvertibleTo =
        std::is_convertible_v<From, To> &&
        exists<detail::ConvertibleTo, From(&)(), To>;

/* Missing: concept CommonReference
 *  Reason: missing std::common_reference metafunction in C++17
 */

// --- CommonReference


template<class From, class T>
using cast_to_common_type = decltype(std::common_type_t<From, T>(declval<From>()));
/**
 * @concept Specifies that two types share a common type [Incomplete]
 * @details Specifies that two types T and U share a common type (as computed
 * by std::common_type_t) to which both can be converted.
 * @note Standard library concept: core language concept
 * @todo Check CommonReference
 */
template<class T, class U>
EXP_CONCEPT Common =
        require<
        // detected_t because Same requires std::common_type_t<T, U> to exist
        // and it might not
        concepts::Same<detected_t<std::common_type_t, T, U>
        ,detected_t<std::common_type_t, U, T>
        >,
        // No need for detected_t for cast_to_common_type: it has the previous
        // requirement and require<> is short-circuiting!
        exists<cast_to_common_type, T, U>,
        exists<cast_to_common_type, U, T>
        > ;

/**
 * @concept Specifies that a type is an integral type
 * @note Standard library concept: core language concept
 */
template<class T>
EXP_CONCEPT Integral = std::is_integral_v<T>;

/**
 * @concept Specifies that a type is an integral type that is signed
 * @note Standard library concept: core language concept
 */
template<class T>
EXP_CONCEPT SignedInteral = require<
Integral<T>,
std::is_signed_v<T>
>;

/**
 * @concept Specifies that a type is an integral type that is unsigned
 * @note Standard library concept: core language concept
 */
template<class T>
EXP_CONCEPT UnsignedInteral = require<
Integral<T>,
std::is_unsigned_v<T>
>;
/**
 * @concept Specifies that a type is assignable from another type [Incomplete]
 * @details Only checks that a type has an assignment operator for a specific
 * argument.
 * @note Standard library concept: core language concept
 * @todo Actually check that an expression of the type and value category
 * specified by RHS can be assigned to an lvalue expression whose type is
 * specified by LHS.
 */
template<class T, class U>
EXP_CONCEPT Assignable = std::is_assignable_v<T, U>;

/**
 * @concept Specifies that a type can be swapped
 * @details Specifies that lvalues of type T are swappable.
 * @note Standard library concept: core language concept
 */
template<class T>
EXP_CONCEPT Swappable = std::is_swappable_v<T>;

/**
 * @concept Specifies that two types can be swapped with each other [Incomplete]
 * @details Specifies that expressions of the type and value category encoded
 * by T and U are swappable with each other.
 * @note Standard library concept: core language concept
 * @todo CommonReference
 */
template<class T, class U>
EXP_CONCEPT SwappableWith = require<
std::is_swappable_with_v<T, T>,
std::is_swappable_with_v<U, U>,
std::is_swappable_with_v<T, U>,
std::is_swappable_with_v<U, T>
>;

/**
 * @concept Specifies that an object of the type can be destroyed
 * @details The concept Destructible specifies the concept of all types whose
 * instances can safely be destroyed at the end of their lifetime (including
 * reference types).
 * @note Standard library concept: core language concept
 */
template<class T>
EXP_CONCEPT Destructible = std::is_nothrow_destructible_v<T>;

/**
 * @concept Sspecifies that a variable of the type can be constructed from or
 * bound to a set of argument types
 * @details Specifies that a variable of type T can be initialized with the
 * given set of argument types Args...
 * @note Standard library concept: core language concept
 */
template<class T, class... Args>
EXP_CONCEPT Constructible = require<
Destructible<T>,
std::is_constructible_v<T, Args...>
>;

/**
 * @concept Specifies that a variable of the type can be constructed from or
 * bound to a set of argument types
 * @details The DefaultConstructible concept provides a shorthand for the
 * common case when the question is whether a type can be constructed with no
 * arguments.
 * @note Standard library concept: core language concept
 */
template<class T>
EXP_CONCEPT DefaultConstructible = Constructible<T>;

/**
 * @concept Specifies that a variable of the type can be move constructed
 * @details Satisfied if T is a reference type, or if it is an object type
 * where an object of that type can constructed from an rvalue of that type in
 * both direct- and copy-initialization contexts, with the usual semantics.
 * @note Types without a move constructor, but with a copy constructor that
 * accepts const T& arguments, satisfy std::is_move_constructible.
 * @note Standard library concept: core language concept
 */
template<class T>
EXP_CONCEPT MoveConstructible = std::is_move_constructible_v<T>;

template<class T>
EXP_CONCEPT CopyConstructible =
        require<
        MoveConstructible<T>,
        Constructible<T, T&>, ConvertibleTo<T&, T>,
        Constructible<T, const T&>, ConvertibleTo<const T&, T>,
        Constructible<T, const T>, ConvertibleTo<const T, T>
        >;

template<class T>
EXP_CONCEPT Pointer = std::is_pointer_v<T>;

/* Object concepts */

/**
 * @concept Specifies that an object of a type can be moved and swapped
 * @details Specifies that T is an object type that can moved (that is, it can
 * be move constructed, move assigned, and lvalues of type T can be swapped)
 * @note Standard library concept: object concept
 */
template<class T>
EXP_CONCEPT Movable =
        require<
        std::is_object_v<T>,
        MoveConstructible<T>,
        Assignable<T&, T>,
        Swappable<T>
        >;

/**
 * @concept Specifies that an object of a type can be copied, moved, and swapped
 * @details Specifies that T is an Movable object type that can also copied
 * (that is, it supports copy construction and copy assignment).
 * @note Standard library concept: object concept
 */
template<class T>
EXP_CONCEPT Copyable =
        require<
        CopyConstructible<T>,
        Movable<T>,
        Assignable<T&, const T&>
        >;

/**
 * @concept Specifies that an object of a type can be copied, moved, swapped,
 * and default constructed
 * @details Specifies that a type is both copyable and default constructible.
 * It is satisfied by types that behave similarly to built-in types like int,
 * except that they need not support comparison with ==.
 * @note Standard library concept: object concept
 */
template<class T>
EXP_CONCEPT Semiregular =
        require<
        Copyable<T>,
        DefaultConstructible<T>
        >;



/* Comparison concepts */

/**
 * @concept Specifies that a type can be used in Boolean contexts
 * @details Specifies the requirements for a type usable in Boolean contexts.
 * For Boolean to be satisfied, the logical operators must have the usual
 * behavior (including short-circuiting).
 * @note Standard library concept: comparison concept
 */
template <class B>
EXP_CONCEPT Boolean =
        require<
        Movable<traits::remove_cvref_t<B>>,
        ConvertibleTo<const std::remove_reference_t<B>&, bool>,

        exists<ops::negation, B>,
        ConvertibleTo<detected_t<ops::negation, B>, bool>,

        exists<ops::logical_conjunction, B, const bool>,
        Same<detected_t<ops::logical_conjunction, B, const bool>, bool>,

        exists<ops::logical_disjuntion, B, const bool>,
        Same<detected_t<ops::logical_disjuntion, B, const bool>, bool>,

        exists<ops::logical_conjunction, B, B>,
        Same<detected_t<ops::logical_conjunction, B, B>, bool>,

        exists<ops::logical_conjunction, const bool, B>,
        Same<detected_t<ops::logical_conjunction, const bool, B>, bool>,

        exists<ops::logical_disjuntion, B, B>,
        Same<detected_t<ops::logical_disjuntion, B, B>, bool>,

        exists<ops::logical_disjuntion, const bool, B>,
        Same<detected_t<ops::logical_disjuntion, const bool, B>, bool>,

        exists<ops::equal_to, B, B>,
        Same<detected_t<ops::equal_to, B, B>, bool>,

        exists<ops::equal_to, B, const bool>,
        Same<detected_t<ops::equal_to, B, const bool>, bool>,

        exists<ops::equal_to, const bool, B>,
        Same<detected_t<ops::equal_to, const bool, B>, bool>,

        exists<ops::not_equal_to, B, B>,
        Same<detected_t<ops::not_equal_to, B, B>, bool>,

        exists<ops::not_equal_to, B, const bool>,
        Same<detected_t<ops::not_equal_to, B, const bool>, bool>,

        exists<ops::not_equal_to, const bool,  B>,
        Same<detected_t<ops::not_equal_to, const bool, B>, bool>
        >;


//concept EqualityComparable = converts_to<bool, ops::equal_to, T, U>;
namespace detail
{
template<class T, class U>
EXP_CONCEPT WeaklyEqualityComparable =
        require<
        exists<ops::equal_to ,T, U>,
        Boolean<detected_t<ops::equal_to, T, U>>,

        exists<ops::not_equal_to, T, U>,
        Boolean<detected_t<ops::not_equal_to, T, U>>,

        exists<ops::equal_to , U, T>,
        Boolean<detected_t<ops::equal_to, U, T>>,

        exists<ops::not_equal_to, U, T>,
        Boolean<detected_t<ops::not_equal_to, U, T>>
        >;
} // namespace detail

/**
 * @concept Specifies that operator == is an equivalence relation
 * @details Specifies that the comparison operators == and != on T reflects
 * equality: == yields true if and only if the operands are equal.
 * @note Standard library concept: comparison concept
 */
template<class T>
EXP_CONCEPT EqualityComparable = detail::WeaklyEqualityComparable<T, T>;

/**
 * @concept Specifies that operator == is an equivalence relation [Incomplete]
 * @details Specifies that the comparison operators == and != on (possibly
 * mixed) T and U operands yield results consistent with equality.
 * @note Standard library concept: comparison concept
 * @todo Ensure thet comparing mixed operands yields results equivalent to
 * comparing the operands converted to their common type.
 */
template<class T, class U>
EXP_CONCEPT EqualityComparableWith =
        require<
        EqualityComparable<T>,
        EqualityComparable<U>,
        detail::WeaklyEqualityComparable<T, U>
        >;
} // namespace concepts


#endif //EXPERIMENTAL_CONCEPTS_H
