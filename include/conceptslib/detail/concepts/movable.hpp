/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef DETAIL_MOVABLE_H
#define DETAIL_MOVABLE_H

#include <type_traits>
#include <conceptslib/detail/concepts/concepts.hpp>
#include <conceptslib/detail/concepts/core.hpp>

namespace concepts
{
/* --- Concept Movable --- */
/**
 * @concept Specifies that an object of a type can be moved and swapped
 * @details Specifies that T is an object type that can moved (that is, it can
 * be move constructed, move assigned, and lvalues of type T can be swapped).
 */
template<class T>
CONCEPT Movable =
        std::is_object_v<T> &&
        MoveConstructible<T> &&
        Assignable<detected_t<ops::lreference, T>, T> &&
Swappable<T>;

} // namespace concepts

#endif //DETAIL_MOVABLE_H
