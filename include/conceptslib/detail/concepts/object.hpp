/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef DETAIL_OBJECT_H
#define DETAIL_OBJECT_H

#include <type_traits>

#include <conceptslib/detail/concepts/concepts.hpp>
#include <conceptslib/detail/concepts/core.hpp>
#include <conceptslib/detail/concepts/movable.hpp>
#include <conceptslib/detail/concepts/comparison.hpp>


namespace concepts
{
/* --- Concept Copyable --- */
/**
 * @concept Specifies that an object of a type can be copied, moved, and swapped
 * @details Specifies that T is an Movable object type that can also copied
 * (that is, it supports copy construction and copy assignment).
 */
template<class T>
CONCEPT Copyable =
    CopyConstructible<T> &&
    Movable<T> &&
    Assignable<detected_t<ops::lreference, T>, detected_t<ops::clreference, T>>;

/* --- Concept Semiregular --- */
/**
 * @concept Specifies that an object of a type can be copied, moved, swapped,
 * and default constructed
 * @details Specifies that a type is both copyable and default constructible.
 * It is satisfied by types that behave similarly to built-in types like int,
 * except that they need not support comparison with ==.
 */
template<class T>
CONCEPT Semiregular = Copyable<T> && DefaultConstructible<T>;

/* --- Concept Regular --- */
/**
 * @concept Specifies that a type is regular, that is, it is both Semiregular
 * and EqualityComparable
 * @details Specifies that a type is regular, that is, it is copyable, default
 * constructible, and equality comparable. It is satisfied by types that behave
 * similarly to built-in types like int, and that are comparable with ==.
 */
template<class T>
CONCEPT Regular = Semiregular<T> && EqualityComparable<T>;

} // namespace concepts
#endif //DETAIL_OBJECT_H
