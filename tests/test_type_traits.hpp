/*
 * Copyright Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * Acknowledgments: also includes some of Eric Niebler's tests to
 *                  common_reference.
 *                  - Copyright  : Eric Niebler
 *                  - Project URL: https://github.com/ericniebler/range-v3
 *                  - License    : Project URL/blob/master/LICENSE.txt
 */

#ifndef TEST_TYPE_TRAITS_H
#define TEST_TYPE_TRAITS_H

#include <conceptslib/type_traits.hpp>

#include "test_type_traits.hpp"

// TESTS

/* --- Test metafunction remove_cvref --- */
using traits::remove_cvref_t;
static_assert(std::is_same_v<remove_cvref_t<const int>         , int>);
static_assert(std::is_same_v<remove_cvref_t<volatile int>      , int>);
static_assert(std::is_same_v<remove_cvref_t<const volatile int>, int>);

static_assert(std::is_same_v<remove_cvref_t<const int&>         , int>);
static_assert(std::is_same_v<remove_cvref_t<volatile int&>      , int>);
static_assert(std::is_same_v<remove_cvref_t<const volatile int&>, int>);

static_assert(std::is_same_v<remove_cvref_t<const int&&>         , int>);
static_assert(std::is_same_v<remove_cvref_t<volatile int&&>      , int>);
static_assert(std::is_same_v<remove_cvref_t<const volatile int&&>, int>);

/* --- Test metafunction copy_cv --- */
using traits::copy_cv_t;
using Cint = const int;
using Vint = volatile int;
using CVint = const volatile int;

using Cvoid = const void;
using CVvoid = const volatile void;

static_assert(std::is_same_v<copy_cv_t<int, int>, int>);
static_assert(std::is_same_v<copy_cv_t<Cint, int>, Cint>);
static_assert(std::is_same_v<copy_cv_t<CVint, int>, CVint>);

// Permutation
static_assert(std::is_same_v<copy_cv_t<int, Cint>, Cint>);
static_assert(std::is_same_v<copy_cv_t<int, Vint>, Vint>);
static_assert(std::is_same_v<copy_cv_t<int, CVint>, CVint>);

// Using void
static_assert(std::is_same_v<copy_cv_t<void, int>, int>);
static_assert(std::is_same_v<copy_cv_t<Cvoid, int>, Cint>);
static_assert(std::is_same_v<copy_cv_t<CVvoid, int>, CVint>);

/* --- Test metafunction select --- */
using traits::select_t;
using traits::condition;
struct TypeA {};
struct TypeB {};
struct TypeC {};
struct TypeD {};
struct TypeE {};

using expectA =
select_t<traits::condition<true, TypeA>
,traits::condition<true, TypeB>
,traits::condition<false, TypeC>>;

using expectB =
traits::select_t<traits::condition<false, TypeA>
,traits::condition<true, TypeB>
,traits::condition<true, TypeC>>;

using expectC =
select_t<condition<false, TypeA>
,condition<false, TypeB>
,condition<true, TypeC>>;

using expectD =
select_t<condition<false, TypeA>
,condition<false, TypeB>
,condition<false, TypeC>
,TypeD>;

// Error - only last element can be a non condition type
//using expectError =
//    select_t<condition<false, TypeA>
//            ,condition<false, TypeB>
//            ,TypeE
//            ,TypeC>;

// Error - no true condition
//using expectError =
//    select_t<condition<false, TypeA>
//            ,condition<false, TypeB>
//            ,condition<false, TypeC>>;

static_assert(std::is_same_v<expectA, TypeA>);
static_assert(std::is_same_v<expectB, TypeB>);
static_assert(std::is_same_v<expectC, TypeC>);
static_assert(std::is_same_v<expectD, TypeD>);

/* --- Test trait has_type --- */
struct WithType { using type = void; };
struct WithoutType { };

static_assert(traits::has_type<WithType>::value);
static_assert(!traits::has_type<WithoutType>::value);

/* --- Test metafunction cref --- */
using traits::cref_t;
static_assert(std::is_same_v<cref_t<int>, const int&>);
static_assert(std::is_same_v<cref_t<const int>, const int&>);
static_assert(std::is_same_v<cref_t<const volatile int>, const volatile int&>);

static_assert(std::is_same_v<cref_t<int&>, const int&>);
static_assert(std::is_same_v<cref_t<const int&>, const int&>);
static_assert(std::is_same_v<cref_t<const volatile int&>, const volatile int&>);

static_assert(std::is_same_v<cref_t<int&&>, const int&>);
static_assert(std::is_same_v<cref_t<const int&&>, const int&>);
static_assert(std::is_same_v<cref_t<const volatile int&&>, const volatile int&>);

/* --- Test metafunction rref --- */
using traits::rref_t;
static_assert(std::is_same_v<rref_t<int>, int&&>);
static_assert(std::is_same_v<rref_t<const int>, const int&&>);
static_assert(std::is_same_v<rref_t<volatile int>, volatile int&&>);
static_assert(std::is_same_v<rref_t<const volatile int>, const volatile int&&>);

static_assert(std::is_same_v<rref_t<int&>, int&&>);
static_assert(std::is_same_v<rref_t<const int&>, const int&&>);
static_assert(std::is_same_v<rref_t<volatile int&>, volatile int&&>);
static_assert(std::is_same_v<rref_t<const volatile int&>, const volatile int&&>);

static_assert(std::is_same_v<rref_t<int&&>, int&&>);
static_assert(std::is_same_v<rref_t<const int&&>, const int&&>);
static_assert(std::is_same_v<rref_t<volatile int&&>, volatile int&&>);
static_assert(std::is_same_v<rref_t<const volatile int&&>, const volatile int&&>);

/* --- Test metafunction xref --- */
using detail::xref;
static_assert(std::is_same_v<xref<double>::type<int>, int>);
static_assert(std::is_same_v<xref<double>::type<int&>, int&>);
static_assert(std::is_same_v<xref<double>::type<int&&>, int&&>);

static_assert(std::is_same_v<xref<const double>::type<int>, const int>);
static_assert(std::is_same_v<xref<const double>::type<int&>, int&>);   // cannot add_const to ref
static_assert(std::is_same_v<xref<const double>::type<int&&>, int&&>); // cannot add_const to ref

static_assert(std::is_same_v<xref<volatile double>::type<int>, volatile int>);
static_assert(std::is_same_v<xref<volatile double>::type<int&>, int&>);   // cannot add_volatile to ref
static_assert(std::is_same_v<xref<volatile double>::type<int&&>, int&&>); // cannot add_volatile to ref

static_assert(std::is_same_v<xref<const volatile double>::type<int>, const volatile int>);
static_assert(std::is_same_v<xref<const volatile double>::type<int&>, int&>);   // cannot add_cv to ref
static_assert(std::is_same_v<xref<const volatile double>::type<int&&>, int&&>); // cannot add_cv to ref

// lvalue ref: no cv qualifiers from A (unless U is not a ref) and collapsed lvalue ref
static_assert(std::is_same_v<xref<double&               >::type<int>, int&>);
static_assert(std::is_same_v<xref<const double&         >::type<int>, const int&>);
static_assert(std::is_same_v<xref<volatile double&      >::type<int>, volatile int&>);
static_assert(std::is_same_v<xref<const volatile double&>::type<int>, const volatile int&>);

static_assert(std::is_same_v<xref<double&               >::type<int&>, int&>);
static_assert(std::is_same_v<xref<const double&         >::type<int&>, int&>);
static_assert(std::is_same_v<xref<volatile double&      >::type<int&>, int&>);
static_assert(std::is_same_v<xref<const volatile double&>::type<int&>, int&>);

static_assert(std::is_same_v<xref<const double&         >::type<int&&>, int&>);
static_assert(std::is_same_v<xref<volatile double&      >::type<int&&>, int&>);
static_assert(std::is_same_v<xref<const volatile double&>::type<int&&>, int&>);

static_assert(std::is_same_v<xref<double&>::type<const int& >, const int&>);
static_assert(std::is_same_v<xref<double&>::type<const int&&>, const int&>);

// rvalue ref: no cv qualifiers from A (unless U is not a ref) and collapsed rvalue ref
static_assert(std::is_same_v<xref<double&&               >::type<int>, int&&>);
static_assert(std::is_same_v<xref<const double&&         >::type<int>, const int&&>);
static_assert(std::is_same_v<xref<volatile double&&      >::type<int>, volatile int&&>);
static_assert(std::is_same_v<xref<const volatile double&&>::type<int>, const volatile int&&>);

static_assert(std::is_same_v<xref<double&&               >::type<int&>, int&>);
static_assert(std::is_same_v<xref<const double&&         >::type<int&>, int&>);
static_assert(std::is_same_v<xref<volatile double&&      >::type<int&>, int&>);
static_assert(std::is_same_v<xref<const volatile double&&>::type<int&>, int&>);

static_assert(std::is_same_v<xref<const double&&         >::type<int&&>, int&&>);
static_assert(std::is_same_v<xref<volatile double&&      >::type<int&&>, int&&>);
static_assert(std::is_same_v<xref<const volatile double&&>::type<int&&>, int&&>);

static_assert(std::is_same_v<xref<double&&>::type<const int& >, const int&>);
static_assert(std::is_same_v<xref<double&&>::type<const int&&>, const int&&>);

/* --- Test metafunction common_reference --- */
struct B {};
struct D : B {};

static_assert(std::is_same_v<common_reference_t<B       &, D       &          >, B       &>);
static_assert(std::is_same_v<common_reference_t<B       &, D const &          >, B const &>);
static_assert(std::is_same_v<common_reference_t<B       &, D const &, D &     >, B const &>);
static_assert(std::is_same_v<common_reference_t<B const &, D       &          >, B const &>);
static_assert(std::is_same_v<common_reference_t<B       &, D       &, B &, D &>, B       &>);

static_assert(std::is_same_v<common_reference_t<B       &&, D       &&>, B       &&>);
static_assert(std::is_same_v<common_reference_t<B const &&, D       &&>, B const &&>);
static_assert(std::is_same_v<common_reference_t<B       &&, D const &&>, B const &&>);

static_assert(std::is_same_v<common_reference_t<B       &, D       &&>, B const &>);
static_assert(std::is_same_v<common_reference_t<B       &, D const &&>, B const &>);
static_assert(std::is_same_v<common_reference_t<B const &, D       &&>, B const &>);

static_assert(std::is_same_v<common_reference_t<B       &&, D       &>, B const &>);
static_assert(std::is_same_v<common_reference_t<B       &&, D const &>, B const &>);
static_assert(std::is_same_v<common_reference_t<B const &&, D       &>, B const &>);

static_assert(std::is_same_v<common_reference_t<int  , short  >, int>);
static_assert(std::is_same_v<common_reference_t<int  , short &>, int>);
static_assert(std::is_same_v<common_reference_t<int &, short &>, int>);
static_assert(std::is_same_v<common_reference_t<int &, short  >, int>);

static_assert(std::is_same_v<common_reference_t<int                &&
                                               ,int volatile        &>, int>);
static_assert(std::is_same_v<common_reference_t<int volatile        &
                                               ,int                &&>, int>);
static_assert(std::is_same_v<common_reference_t<int const volatile &&
                                               ,int volatile       &&>, int const volatile &&>);
static_assert(std::is_same_v<common_reference_t<int                &&
                                               ,int const &
                                               ,int volatile &       >, int const volatile &>);

/* Test metafunction common_type --- */
// No implicit conversion
struct X {};
struct Y {};
struct Z {};
struct W {};

namespace std
{
    template<>
    struct common_type<X, Y> { using type = Z; };

    template<>
    struct common_type<Y, X> { using type = Z; };
}

static_assert(std::is_same_v<std::common_type_t<X, Y>, Z>);
static_assert(std::is_same_v<std::common_type_t<Y, X>, Z>);
static_assert(!is_detected_v<std::common_type_t, X, W>);

template<>
struct common_type<X, Y> { using type = Z; };

template<>
struct common_type<Y, X> { using type = Z; };

static_assert(std::is_same_v<common_type_t<X, Y>, Z>);
static_assert(std::is_same_v<common_type_t<Y, X>, Z>);
static_assert(!is_detected_v<common_type_t, X, W>);


#endif //TEST_TYPE_TRAITS_H
