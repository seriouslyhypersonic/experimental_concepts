/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef DETAIL_COMPARISON_H
#define DETAIL_COMPARISON_H

#include <type_traits>

#include <conceptslib/detail/concepts/concepts.hpp>
#include <conceptslib/detail/concepts/core.hpp>
#include <conceptslib/detail/concepts/movable.hpp>


#include <conceptslib/type_traits.hpp>

namespace concepts
{

namespace detail
{
REQUIREMENT BooleanReq
{
    template<class B>
    auto REQUIRES(const std::remove_reference_t<B>& b1,
                  const std::remove_reference_t<B>& b2, const bool a)
        -> decltype(valid_expr(
            valid_if<ConvertibleTo<const std::remove_reference_t<B>&, bool>>(),
              !b1,    valid_if<ConvertibleTo<decltype(!b1), bool>>(),
            b1 && a,  valid_if<Same<decltype(b1 && a), bool>>(),
            b1 || a,  valid_if<Same<decltype(b1 || a), bool>>(),
            b1 && b2, valid_if<Same<decltype(b1 && b2), bool>>(),
             a && b2, valid_if<Same<decltype(a && b2), bool>>(),
            b1 || b2, valid_if<Same<decltype(b1 || b2), bool>>(),
             a || b2, valid_if<Same<decltype(a || b2), bool>>(),
            b1 == b2, valid_if<ConvertibleTo<decltype(b1 == b2), bool>>(),
            b1 == a,  valid_if<ConvertibleTo<decltype(b1 == a), bool>>(),
             a == b2, valid_if<ConvertibleTo<decltype(a == b2), bool>>(),
            b1 != b2, valid_if<ConvertibleTo<decltype(b1 != b2), bool>>(),
            b1 != a,  valid_if<ConvertibleTo<decltype(b1 != a), bool>>(),
             a != b2, valid_if<ConvertibleTo<decltype(a != b2), bool>>()
        ));
};
} // namespace detail

/* --- Concept Bolean --- */
/**
 * @concept Specifies that a type can be used in Boolean contexts.
 * and EqualityComparable
 * @details Specifies the requirements for a type usable in Boolean contexts.
 * For Boolean to be satisfied, the logical operators must have the usual
 * behavior (including short-circuiting).
 */
template<class B>
CONCEPT Boolean =
    Movable<traits::remove_cvref_t<B>> &&
    requires_<detail::BooleanReq, B>;

/* --- Concept EqualityComparable --- */
namespace detail
{
REQUIREMENT WeaklyEqualityComparableWithReq
{
    // Specifies that an object of type T and an object of type U can be
    // compared for equality with each other (in either order) using both ==
    // and !=, and the results of the comparisons are consistent.
    template<class T, class U>
    auto REQUIRES(const std::remove_reference_t<T>& t,
                  const std::remove_reference_t<U>& u)
    -> decltype(valid_expr(
        t == u, valid_if<Boolean<decltype(t == u)>>(),
        t != u, valid_if<Boolean<decltype(t != u)>>(),
        u == t, valid_if<Boolean<decltype(u == t)>>(),
        u != t, valid_if<Boolean<decltype(u != t)>>()
    ));
};

template<class T, class U>
CONCEPT WeaklyEqualityComparableWith =
    requires_<WeaklyEqualityComparableWithReq, T, U>;
} // namespace detail

/**
 * @concept Specifies that the comparison operators == and != on T reflects
 * equality: == yields true if and only if the operands are equal.
 * @details EqualityComparable<T> is satisfied only if, given objects a and b
 * of type T, bool(a == b) is true if and only if a and b are equal.
 */
template<class T>
CONCEPT EqualityComparable =
    requires_<detail::WeaklyEqualityComparableWithReq, T, T>;

/* --- Concept EqualityComparableWith --- */
/**
 * @concept Specifies that the comparison operators == and != on (possibly
 * mixed) T and U operands yield results consistent with equality.
 * equality: == yields true if and only if the operands are equal.
 * @details Comparing mixed operands yields results equivalent to comparing the
 * operands converted to their common type.
 */
template<class T, class U>
CONCEPT EqualityComparableWith =
    EqualityComparable<T> &&
    EqualityComparable<U> &&
    CommonReference<detected_t<traits::clref_t, T>
                   ,detected_t<traits::clref_t, U>> &&
    EqualityComparable<detected_t<traits::common_reference_t
                                 ,detected_t<traits::clref_t, T>
                                 ,detected_t<traits::clref_t, U>>> &&
    detail::WeaklyEqualityComparableWith<T, U>;

/* --- Concept StrictTotallyOrdered --- */
namespace detail
{
REQUIREMENT StrictTotallyOrderedReq
{
    template<class T>
    auto REQUIRES(const std::remove_reference_t<T>& a,
                  const std::remove_reference_t<T>& b)
        -> decltype(valid_expr(
            a < b, valid_if<Boolean<decltype(a < b)>>(),
            a > b, valid_if<Boolean<decltype(a > b)>>(),
            a <= b, valid_if<Boolean<decltype(a <= b)>>(),
            a >= b, valid_if<Boolean<decltype(a >= b)>>()
        ));
};
} // namespace detail
/**
 * @concept Specifies that the comparison operators ==,!=,<,>,<=,>= on T yield
 * results consistent with a strict total order on T.
 */
template<class T>
CONCEPT StrictTotallyOrdered =
    EqualityComparable<T> &&
    requires_<detail::StrictTotallyOrderedReq, T>;

/* --- Concept StrictTotallyOrderedWith --- */

namespace detail
{
REQUIREMENT StrictTotallyOrderedWithReq
{
    template<class T, class U>
    auto REQUIRES(const std::remove_reference_t<T>& t,
                  const std::remove_reference_t<U>& u)
        -> decltype(valid_expr(
            t <  u, valid_if<Boolean<decltype(t < u)>>(),
            t >  u, valid_if<Boolean<decltype(t > u)>>(),
            t <= u, valid_if<Boolean<decltype(t <= u)>>(),
            t >= u, valid_if<Boolean<decltype(t >= u)>>(),
            u <  t, valid_if<Boolean<decltype(u < t)>>(),
            u >  t, valid_if<Boolean<decltype(u > t)>>(),
            u <= t, valid_if<Boolean<decltype(u <= t)>>(),
            u >= t, valid_if<Boolean<decltype(u >= t)>>()
        ));
};
} // namespace detail

/**
 * @concept Specifies that the comparison operators ==,!=,<,>,<=,>= on
 * (possibly mixed) T and U operands yield results consistent with a strict
 * total order.
 * @details Comparing mixed operands yields results equivalent to comparing the
 * operands converted to their common type.
 */
template<class T, class U>
CONCEPT StrictTotallyOrderedWith =
    StrictTotallyOrdered<T> &&
    StrictTotallyOrdered<U> &&
    CommonReference<detected_t<traits::clref_t, T>
                   ,detected_t<traits::clref_t, U>> &&
    StrictTotallyOrdered<detected_t<traits::common_reference_t
                                   ,detected_t<traits::clref_t, T>
                                   ,detected_t<traits::clref_t, U>>> &&
    EqualityComparableWith<T, U> &&
    requires_<detail::StrictTotallyOrderedWithReq, T, U>;

} // namespace concepts

#endif //DETAIL_COMPARISON_H
