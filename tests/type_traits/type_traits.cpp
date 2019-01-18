/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 *
 */
#include <gtest/gtest.h>

#include <conceptslib/type_traits.hpp>

struct TypeTraitsTest: ::testing::Test
{
    struct ConstLRefToW;

    // No implicit conversion
    struct X { };
    struct Y { };
    struct Z { };
    struct W { };

    struct ImplicitToX { operator X() const { return {}; } };
    struct ExplicitToX { explicit operator X() const { return {}; } };
};

/* --- Test metafunction remove_cvref --- */
TEST_F(TypeTraitsTest, MetafunctionRemoveCVRef)
{
    using traits::remove_cvref_t;

    static_assert(std::is_same_v<remove_cvref_t<const int>, int>);
    static_assert(std::is_same_v<remove_cvref_t<volatile int>, int>);
    static_assert(std::is_same_v<remove_cvref_t<const volatile int>, int>);

    static_assert(std::is_same_v<remove_cvref_t<const int&>, int>);
    static_assert(std::is_same_v<remove_cvref_t<volatile int&>, int>);
    static_assert(std::is_same_v<remove_cvref_t<const volatile int&>, int>);

    static_assert(std::is_same_v<remove_cvref_t<const int&&>, int>);
    static_assert(std::is_same_v<remove_cvref_t<volatile int&&>, int>);
    static_assert(std::is_same_v<remove_cvref_t<const volatile int&&>, int>);
}

/* --- Test metafunction copy_cv --- */
TEST_F(TypeTraitsTest, MetafunctionCopyCV)
{
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
}

/* --- Test metafunction select --- */
TEST_F(TypeTraitsTest, MetafunctionSelect)
{
    using traits::select_t;
    using traits::condition;
    struct TypeA { };
    struct TypeB { };
    struct TypeC { };
    struct TypeD { };
    struct TypeE { };

    using expectA =
        select_t<condition<true, TypeA>
                ,condition<true, TypeB>
                ,condition<false, TypeC>>;

    using expectB =
        select_t<condition<false, TypeA>
                ,condition<true, TypeB>
                ,condition<true, TypeC>>;

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
}

/* --- Test trait has_type --- */
TEST_F(TypeTraitsTest, TypeTraitHasType)
{
    struct WithType { using type = void; };
    struct WithoutType { };

    static_assert(traits::has_type<WithType>::value);
    static_assert(!traits::has_type<WithoutType>::value);
}

/* --- Test metafunction clref --- */
TEST_F(TypeTraitsTest, MetafunctionCLRef)
{
    using traits::detail::clref_t;

    static_assert(std::is_same_v<clref_t<int>, const int&>);
    static_assert(std::is_same_v<clref_t<const int>, const int&>);
    static_assert(std::is_same_v<clref_t<const volatile int>, const volatile int&>);

    static_assert(std::is_same_v<clref_t<int&>, const int&>);
    static_assert(std::is_same_v<clref_t<const int&>, const int&>);
    static_assert(
    std::is_same_v<clref_t<const volatile int&>, const volatile int&>);

    static_assert(std::is_same_v<clref_t<int&&>, const int&>);
    static_assert(std::is_same_v<clref_t<const int&&>, const int&>);
    static_assert(
    std::is_same_v<clref_t<const volatile int&&>, const volatile int&>);
}

/* --- Test metafunction rref --- */
TEST_F(TypeTraitsTest, MetafunctionRRef)
{
    using traits::detail::rref_t;

    static_assert(std::is_same_v<rref_t<int>, int>);
    static_assert(std::is_same_v<rref_t<const int>, const int>);
    static_assert(std::is_same_v<rref_t<volatile int>, volatile int>);
    static_assert(std::is_same_v<rref_t<const volatile int>, const volatile int>);

    static_assert(std::is_same_v<rref_t<int&>, int&&>);
    static_assert(std::is_same_v<rref_t<const int&>, const int&&>);
    static_assert(std::is_same_v<rref_t<volatile int&>, volatile int&&>);
    static_assert(
    std::is_same_v<rref_t<const volatile int&>, const volatile int&&>);

    static_assert(std::is_same_v<rref_t<int&&>, int&&>);
    static_assert(std::is_same_v<rref_t<const int&&>, const int&&>);
    static_assert(std::is_same_v<rref_t<volatile int&&>, volatile int&&>);
    static_assert(
    std::is_same_v<rref_t<const volatile int&&>, const volatile int&&>);
}

/* --- Test metafunction xref --- */
TEST_F(TypeTraitsTest, MetafunctionXRef)
{
    using traits::detail::xref;
    static_assert(std::is_same_v<xref<double>::type<int>, int>);
    static_assert(std::is_same_v<xref<double>::type<int&>, int&>);
    static_assert(std::is_same_v<xref<double>::type<int&&>, int&&>);

    static_assert(std::is_same_v<xref<const double>::type<int>, const int>);
    static_assert(
    std::is_same_v<xref<const double>::type<int&>, int&>);   // cannot add_const to ref
    static_assert(
    std::is_same_v<xref<const double>::type<int&&>, int&&>); // cannot add_const to ref

    static_assert(std::is_same_v<xref<volatile double>::type<int>, volatile int>);
    static_assert(
    std::is_same_v<xref<volatile double>::type<int&>, int&>);   // cannot add_volatile to ref
    static_assert(
    std::is_same_v<xref<volatile double>::type<int&&>, int&&>); // cannot add_volatile to ref

    static_assert(
    std::is_same_v<xref<const volatile double>::type<int>, const volatile int>);
    static_assert(
    std::is_same_v<xref<const volatile double>::type<int&>, int&>);   // cannot add_cv to ref
    static_assert(
    std::is_same_v<xref<const volatile double>::type<int&&>, int&&>); // cannot add_cv to ref

    // lvalue ref: no cv qualifiers from A (unless U is not a ref) and collapsed lvalue ref
    static_assert(std::is_same_v<xref<double&>::type<int>, int&>);
    static_assert(std::is_same_v<xref<const double&>::type<int>, const int&>);
    static_assert(std::is_same_v<xref<volatile double&>::type<int>, volatile int&>);
    static_assert(
    std::is_same_v<xref<const volatile double&>::type<int>, const volatile int&>);

    static_assert(std::is_same_v<xref<double&>::type<int&>, int&>);
    static_assert(std::is_same_v<xref<const double&>::type<int&>, int&>);
    static_assert(std::is_same_v<xref<volatile double&>::type<int&>, int&>);
    static_assert(std::is_same_v<xref<const volatile double&>::type<int&>, int&>);

    static_assert(std::is_same_v<xref<const double&>::type<int&&>, int&>);
    static_assert(std::is_same_v<xref<volatile double&>::type<int&&>, int&>);
    static_assert(std::is_same_v<xref<const volatile double&>::type<int&&>, int&>);

    static_assert(std::is_same_v<xref<double&>::type<const int&>, const int&>);
    static_assert(std::is_same_v<xref<double&>::type<const int&&>, const int&>);

    // rvalue ref: no cv qualifiers from A (unless U is not a ref) and collapsed rvalue ref
    static_assert(std::is_same_v<xref<double&&>::type<int>, int&&>);
    static_assert(std::is_same_v<xref<const double&&>::type<int>, const int&&>);
    static_assert(
    std::is_same_v<xref<volatile double&&>::type<int>, volatile int&&>);
    static_assert(
    std::is_same_v<xref<const volatile double&&>::type<int>, const volatile int&&>);

    static_assert(std::is_same_v<xref<double&&>::type<int&>, int&>);
    static_assert(std::is_same_v<xref<const double&&>::type<int&>, int&>);
    static_assert(std::is_same_v<xref<volatile double&&>::type<int&>, int&>);
    static_assert(std::is_same_v<xref<const volatile double&&>::type<int&>, int&>);

    static_assert(std::is_same_v<xref<const double&&>::type<int&&>, int&&>);
    static_assert(std::is_same_v<xref<volatile double&&>::type<int&&>, int&&>);
    static_assert(
    std::is_same_v<xref<const volatile double&&>::type<int&&>, int&&>);

    static_assert(std::is_same_v<xref<double&&>::type<const int&>, const int&>);
    static_assert(std::is_same_v<xref<double&&>::type<const int&&>, const int&&>);
}

/* --- Test metafunction xref --- */

namespace std
{
    using X = TypeTraitsTest::X;
    using Y = TypeTraitsTest::Y;
    using Z = TypeTraitsTest::Z;

    template<> struct common_type<X, Y> { using type = Z; };
    template<> struct common_type<Y, X> { using type = Z; };

    template<> struct common_type<X*, Y*> { using type = Z*; };
} // namespace std

namespace traits
{
    using X = TypeTraitsTest::X;
    using Y = TypeTraitsTest::Y;

    template<> struct common_type<X, Y>: std::common_type<X, Y> { };
    template<> struct common_type<Y, X>: std::common_type<Y, X> { };

    template<> struct common_type<X*, Y*>: std::common_type<X*, Y*> { };
} // namespace traits

/* Test metafunction common_type --- */
TEST_F(TypeTraitsTest, MetafunctionCommonType)
{
    // std::common_type
    static_assert(std::is_same_v<std::common_type_t<int[], int*>, int*>);
    static_assert(std::is_same_v<std::common_type_t<int[42], int*>, int*>);
    static_assert(std::is_same_v<std::common_type_t<int(int), int(*)(int)>, int(*)(int)>);

    // traits::common_type
    static_assert(std::is_same_v<traits::common_type_t<int[], int*>, int*>);
    static_assert(std::is_same_v<traits::common_type_t<int[42], int*>, int*>);
    static_assert(std::is_same_v<traits::common_type_t<int(int), int(*)(int)>, int(*)(int)>);

    // std::common_type
    static_assert(std::is_same_v<std::common_type_t<X, Y>, Z>);
    static_assert(std::is_same_v<std::common_type_t<Y, X>, Z>);
    static_assert(!traits::is_detected_v<std::common_type_t, X, W>);

    // traits::common_type
    static_assert(std::is_same_v<traits::common_type_t<X, Y>, Z>);
    static_assert(std::is_same_v<traits::common_type_t<Y, X>, Z>);
    static_assert(!traits::is_detected_v<traits::common_type_t, X, W>);

    // ref X and Y do not decay to same type X and Y so the common type is
    // common_type<std::decay<T1>::type, std::decay<T2>::type>

    // Should work with std::common_type!
    // gcc bug: does not consider user specialization?
//    static_assert(std::is_same_v<std::common_type_t<X&, Y&>, Z>);
//    static_assert(std::is_same_v<std::common_type_t<X&&, Y&&>, Z>);

    // Works as expected with custom implementation
    static_assert(std::is_same_v<traits::common_type_t<X&, Y&>, Z>);
    static_assert(std::is_same_v<traits::common_type_t<X&&, Y&&>, Z>);

    // No specialization
    static_assert(!traits::is_detected_v<std::common_type_t, X, W>);
    static_assert(!traits::is_detected_v<traits::common_type_t, X, W>);

    // std::common_type
    static_assert(std::is_same_v<std::common_type_t<X, ImplicitToX>, X>);
    static_assert(std::is_same_v<std::common_type_t<ImplicitToX, X>, X>);

    // traits::common_type
    static_assert(std::is_same_v<traits::common_type_t<X, ImplicitToX>, X>);
    static_assert(std::is_same_v<traits::common_type_t<ImplicitToX, X>, X>);
    static_assert(std::is_same_v<traits::common_type_t<X, ImplicitToX>, X>);

    // ref X and Y do not decay to same type X and Y so the common type is
    // common_type<std::decay<T1>::type, std::decay<T2>::type>

    // std::common_type (works if there is an implicit conversion)
    static_assert(std::is_same_v<std::common_type_t<X&, ImplicitToX&>, X>);
    static_assert(std::is_same_v<std::common_type_t<X&&, ImplicitToX&&>, X>);

    // traits::common_type
    static_assert(std::is_same_v<traits::common_type_t<X&, ImplicitToX&>, X>);;
    static_assert(std::is_same_v<traits::common_type_t<X&&, ImplicitToX&&>, X>);

    // No specialization, no implicit conversion
    static_assert(!traits::is_detected_v<std::common_type_t, X, ExplicitToX>);
    static_assert(!traits::is_detected_v<traits::common_type_t, X, ExplicitToX>);

    // Variadic
    static_assert(std::is_same_v<std::common_type_t<X, ImplicitToX&, X>, X>);
    static_assert(std::is_same_v<std::common_type_t<X&, ImplicitToX&, X&>, X>);
    static_assert(std::is_same_v<std::common_type_t<X&&, ImplicitToX&, X&>, X>);

    static_assert(std::is_same_v<traits::common_type_t<X, ImplicitToX, X>, X>);
    static_assert(std::is_same_v<traits::common_type_t<X&, ImplicitToX&, X&>, X>);
    static_assert(std::is_same_v<traits::common_type_t<X&&, ImplicitToX&, X&>, X>);

    static_assert(std::is_same_v<std::common_type_t<X, ImplicitToX, Y>, Z>);
    static_assert(std::is_same_v<traits::common_type_t<X, ImplicitToX, Y>, Z>);

    // Again same gcc bug when there decay to different type?
//    static_assert(std::is_same_v<std::common_type_t<X&&, ImplicitToX, Y&>, Z>);
    static_assert(std::is_same_v<traits::common_type_t<X&&, ImplicitToX, Y&>, Z>);

    // Decay

    // std::common_type
    static_assert(std::is_same_v<std::common_type_t<X[], X*>, X*>);
    static_assert(std::is_same_v<std::common_type_t<X[42], X*>, X*>);
    static_assert(std::is_same_v<std::common_type_t<X(X), X(*)(X)>, X(*)(X)>);

    static_assert(std::is_same_v<std::common_type_t<X*, Y*>, Z*>);
    // does not work!
//    static_assert(std::is_same_v<std::common_type_t<X[], Y*>, Z*>);
//    static_assert(std::is_same_v<std::common_type_t<X*, Y[]>, Z*>);
    static_assert(std::is_same_v<traits::common_type_t<X[], Y[]>, Z*>);

    static_assert(std::is_same_v<std::common_type_t<X(Y), X(*)(Y)>, X(*)(Y)>);
    static_assert(std::is_same_v<std::common_type_t<X(Y[]), X(*)(Y[])>, X(*)(Y*)>);

    // traits::common_type
    static_assert(std::is_same_v<traits::common_type_t<X[], X*>, X*>);
    static_assert(std::is_same_v<traits::common_type_t<X[42], X*>, X*>);
    static_assert(std::is_same_v<traits::common_type_t<X(X), X(*)(X)>, X(*)(X)>);

    static_assert(std::is_same_v<traits::common_type_t<X*, Y*>, Z*>);
    static_assert(std::is_same_v<traits::common_type_t<X[], Y*>, Z*>);
    static_assert(std::is_same_v<traits::common_type_t<X*, Y[]>, Z*>);
    static_assert(std::is_same_v<traits::common_type_t<X[], Y[]>, Z*>);

    static_assert(std::is_same_v<traits::common_type_t<X(Y), X(*)(Y)>, X(*)(Y)>);
    static_assert(std::is_same_v<traits::common_type_t<X(Y[]), X(*)(Y[])>, X(*)(Y*)>);
}

/* Test metafunction common_reference --- */
TEST_F(TypeTraitsTest, MetafunctionCommonReference)
{
    using traits::common_reference_t;
    struct Base
    { };
    struct Derived: Base
    { };

    struct A { A(Base) { }};

    static_assert(std::is_same_v<common_reference_t<Base, Derived>, Base>);
    static_assert(std::is_same_v<common_reference_t<Base, const Derived>, Base>);
    static_assert(std::is_same_v<common_reference_t<const Base, Derived>, const Base>);

    // Base object type
    static_assert(std::is_same_v<common_reference_t<Base, Derived&>, Base>);
    static_assert(std::is_same_v<common_reference_t<Base, const Derived&>, Base>);
    static_assert(std::is_same_v<common_reference_t<const Base, Derived&>, const Base>);

    static_assert(std::is_same_v<common_reference_t<Base, Derived&&>, Base>);
    static_assert(std::is_same_v<common_reference_t<Base, const Derived&&>, Base>);
    static_assert(std::is_same_v<common_reference_t<const Base, Derived&&>, const Base>);

    // Base lref
    static_assert(std::is_same_v<common_reference_t<Base&, Derived>, Base>);
    static_assert(std::is_same_v<common_reference_t<Base&, const Derived>, Base>);
    static_assert(std::is_same_v<common_reference_t<const Base&, Derived>, const Base>);

    static_assert(std::is_same_v<common_reference_t<Base&, Derived&>, Base&>);
    static_assert(std::is_same_v<common_reference_t<Base&, const Derived&>, const Base&>);
    static_assert(std::is_same_v<common_reference_t<const Base&, Derived&>, const Base&>);

    static_assert(std::is_same_v<common_reference_t<Base&, Derived&&>, const Base&>);
    static_assert(std::is_same_v<common_reference_t<Base&, const Derived&&>, const Base&>);
    static_assert(std::is_same_v<common_reference_t<const Base&, Derived&&>, const Base&>);

    // Base rref
    bool result = std::is_same_v<common_reference_t<Base&&, Derived>, Base&&>;
    ASSERT_EQ(result, true);
    result = std::is_same_v<common_reference_t<Base&&, Derived>, Base&&>;
    ASSERT_EQ(result, true);
    static_assert(std::is_same_v<common_reference_t<Base&&, Derived>, Base&&>);             // red squiggles
    static_assert(std::is_same_v<common_reference_t<Base&&, const Derived>, Base>);
    static_assert(std::is_same_v<common_reference_t<const Base&&, Derived>, const Base&&>); // red squiggles

    static_assert(std::is_same_v<common_reference_t<Base&&, Derived&>, const Base&>);
    static_assert(std::is_same_v<common_reference_t<Base&&, const Derived&>, const Base&>);
    static_assert(std::is_same_v<common_reference_t<const Base&&, Derived&>, const Base&>);

    static_assert(std::is_same_v<common_reference_t<Base&&, Derived&&>, Base&&>);
    static_assert(std::is_same_v<common_reference_t<Base&&, const Derived&&>, const Base&&>);
    static_assert(std::is_same_v<common_reference_t<const Base&&, Derived&&>, const Base&&>);

    // Variadic
    static_assert(std::is_same_v<common_reference_t<Base&, Derived&, Derived&>, Base&>);
    static_assert(std::is_same_v<common_reference_t<Base&, const Derived&, Derived&>, const Base&>);
    static_assert(std::is_same_v<common_reference_t<Base&, Derived&&, Derived>, const Base>);

    // Built in and volatile
    static_assert(std::is_same_v<common_reference_t<int, short>, int>);
    static_assert(std::is_same_v<common_reference_t<int, short&>, int>);
    static_assert(std::is_same_v<common_reference_t<int&, short>, int>);
    static_assert(std::is_same_v<common_reference_t<int&, short&>, int>);

    static_assert(std::is_same_v<common_reference_t<int&&, int volatile&>, int>);
    static_assert(std::is_same_v<common_reference_t<int volatile&, int&&>, int>);
    static_assert(std::is_same_v<common_reference_t<const volatile int&&
                                                   ,volatile int&&>
                                                   ,const volatile int&&>);
    static_assert(std::is_same_v<common_reference_t<int&&, const int&
                                                   ,volatile int&>
                                                   ,const volatile int&>);
}
