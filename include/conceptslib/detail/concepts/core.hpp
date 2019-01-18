/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef DETAIL_CONCEPTS_CORE_H
#define DETAIL_CONCEPTS_CORE_H

#include <type_traits>

#include <conceptslib/detail/concepts/concepts.hpp>

namespace concepts
{
using traits::common_reference_t;
using traits::common_type_t;
using traits::detected_t;


/* --- Concept Same --- */
namespace detail
{
template<class T, class U>
CONCEPT SameHelper = std::is_same_v<T, U>;
} // namespace detail
/**
 * @concept Specifies that a type is the same as another type
 * @details Satisfied if and only if T and U denote the same type.
 */
template<class T, class U>
CONCEPT Same = detail::SameHelper<T, U> && detail::SameHelper<U, T>;

/* --- Concept Derived --- */
/**
 * @concept Specifies that a type is derived from another type
 * @details Satisfied if and only if Base is a class type that is either
 * Derived or a public and unambiguous base of Derived, ignoring cv-qualifiers.
 */
template<class Derived, class Base>
CONCEPT DerivedFrom =
    std::is_base_of_v<Base, Derived> &&
    std::is_convertible_v<const volatile std::remove_reference_t<Derived>*
                         ,const volatile std::remove_reference_t<Base>*>;

/* --- Concept ConvertibleTo --- */
namespace detail
{
// Convert a function call of type Fun = From(&)() to type To
template<class Fun, class To>
using ConvertibleToReq = decltype(static_cast<To>(std::declval<Fun>()()));
} // namespace detail
/**
 * @concept Specifies that a type is implicitly convertible to
 * another type
 * @details Specifies that an expression of the type and value category
 * specified by From can be implicitly and explicitly converted to the type To,
 * and the two forms of conversion are equivalent.
 */
template<class From, class To>
CONCEPT ConvertibleTo =
    std::is_convertible_v<From, To> &&
    exists<detail::ConvertibleToReq, From(&)(), To>;

/* --- Concept CommonReference --- */
/**
 * @concept Specifies that two types share a common reference type
 * @details Specifies that two types T and U share a common reference type  (as
 * computed by \c common_reference_t) to which both can be converted.
 */
template<class T, class U>
CONCEPT CommonReference =
    Same<detected_t<common_reference_t, T, U>
        ,detected_t<common_reference_t, U, T>> &&
    ConvertibleTo<T, detected_t<common_reference_t, T, U>> &&
    ConvertibleTo<U, detected_t<common_reference_t, T, U>>;

/* --- Concept Common --- */
/**
 * @concept Specifies that two types share a common type
 * @details Specifies that two types T and U share a common type (as computed
 * by std::common_type_t) to which both can be converted.
 */
template<class T, class U>
CONCEPT Common =
    Same<detected_t<common_type_t, T, U>
        ,detected_t<common_type_t, U, T>> &&
    ConvertibleTo<T, detected_t<common_type_t, T, U>> &&
    ConvertibleTo<U, detected_t<common_type_t, T, U>> &&
    CommonReference<std::add_lvalue_reference_t<const T>
                   ,std::add_lvalue_reference_t<const U>> &&
    CommonReference<std::add_lvalue_reference_t<detected_t<common_type_t, T, U>>
                   ,detected_t<common_reference_t
                              ,std::add_lvalue_reference_t<const T>
                              ,std::add_lvalue_reference_t<const U>
                              >
                   >;

/* --- Concept Integral --- */
/**
 * @concept Specifies that a type is an integral type
 */
template<class T>
CONCEPT Integral = std::is_integral_v<T>;

/* --- Concept SignedInteral --- */
/**
 * @concept Specifies that a type is an integral type that is signed
 */
template<class T>
CONCEPT SignedInteral = Integral<T> && std::is_signed_v<T>;

/* --- Concept UnsignedInteral --- */
/**
 * @concept Specifies that a type is an integral type that is unsigned
 */
template<class T>
CONCEPT UnsignedInteral = Integral<T> && std::is_unsigned_v<T>;

/* --- Concept Assignable --- */
namespace detail
{
REQUIREMENT AssignableReq
{
    template<class LHS, class RHS>
    auto REQUIRES(LHS lhs, RHS&& rhs) -> decltype(valid_expr(
        lhs = std::forward<RHS>(rhs),
        valid_if<Same<decltype(lhs = std::forward<RHS>(rhs)), LHS>>()
    ));
};

} // namespace detail
/**
 * @concept Specifies that a type is assignable from another type
 * @details Only checks that a type has an assignment operator for a specific
 * argument.
 * @note Standard library concept: core language concept
 */
template<class LHS, class RHS>
CONCEPT Assignable =
    std::is_lvalue_reference_v<LHS> &&
    CommonReference<const std::remove_reference_t<LHS>&
                   ,const std::remove_reference_t<RHS>&> &&
    requires_<detail::AssignableReq, LHS, RHS>;

/* --- Concept Swappable --- */
/**
 * @concept Specifies that a type can be swapped
 * @details Specifies that lvalues of type T are swappable.
 * @note Standard library concept: core language concept
 */
template<class T>
CONCEPT Swappable = std::is_swappable_v<T>;

/* --- Concept SwappableWith --- */
/**
 * @concept Specifies that two types can be swapped with each other [Incomplete]
 * @details Specifies that expressions of the type and value category encoded
 * by T and U are swappable with each other.
 * @note Standard library concept: core language concept
 * @todo CommonReference
 */
template<class T, class U>
CONCEPT SwappableWith =
    std::is_swappable_with_v<T, T> &&
    std::is_swappable_with_v<U, U> &&
    CommonReference<const std::remove_reference_t<T>&
                   ,const std::remove_reference_t<U>&> &&
    std::is_swappable_with_v<T, U> &&
    std::is_swappable_with_v<U, T>;

/* --- Concept Destructible --- */
/**
 * @concept Specifies that an object of the type can be destroyed
 * @details The concept Destructible specifies the concept of all types whose
 * instances can safely be destroyed at the end of their lifetime (including
 * reference types).
 */
template<class T>
CONCEPT Destructible = std::is_nothrow_destructible_v<T>;

/* --- Concept Destructible --- */
/**
 * @concept Sspecifies that a variable of the type can be constructed from or
 * bound to a set of argument types
 * @details Specifies that a variable of type T can be initialized with the
 * given set of argument types Args...
 */
template<class T, class... Args>
CONCEPT Constructible = Destructible<T> && std::is_constructible_v<T, Args...>;

/* --- Concept Destructible --- */
/**
 * @concept Specifies that a variable of the type can be constructed from or
 * bound to a set of argument types
 * @details The DefaultConstructible concept provides a shorthand for the
 * common case when the question is whether a type can be constructed with no
 * arguments.
 */
template<class T>
CONCEPT DefaultConstructible = Constructible<T>;

/* --- Concept MoveConstructible --- */
/**
 * @concept Specifies that a variable of the type can be move constructed
 * @details Satisfied if T is a reference type, or if it is an object type
 * where an object of that type can be constructed from an rvalue of that type
 * in both direct- and copy-initialization contexts, with the usual semantics.
 * @note Types without a move constructor, but with a copy constructor that
 * accepts const T& arguments, satisfy \c std::is_move_constructible.
 */
template<class T>
CONCEPT MoveConstructible = Constructible<T, T> && ConvertibleTo<T, T>;

/* --- Concept CopyConstructible --- */
/**
 * @concept Specifies that an object of a type can be copy constructed and move
 * @details The concept CopyConstructible is satisfied if T is an lvalue
 * reference type, or if it is a MoveConstructible object type where an object
 * of that type can constructed from a (possibly const) lvalue or const rvalue
 * of that type in both direct- and copy-initialization contexts with the usual
 * semantics (a copy is constructed with the source unchanged).
 */
template<class T>
CONCEPT CopyConstructible =
    MoveConstructible<T> &&
    Constructible<T, T&> && ConvertibleTo<T&, T> &&
    Constructible<T, const T&> && ConvertibleTo<const T&, T> &&
    Constructible<T, const T> && ConvertibleTo<const T, T>;

} // namespace concepts

#endif //DETAIL_CONCEPTS_CORE_H
