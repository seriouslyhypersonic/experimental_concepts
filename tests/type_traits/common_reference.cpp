/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include <type_traits>

#include <gtest/gtest.h>

#include <conceptslib/type_traits.hpp>

// Required due to compiler dependent behaviour
#if defined(__clang__)
    #define TRANSFORM_REF(x) x
#elif defined(__GNUC__)
    #define TRANSFORM_REF(x) x&&
#endif

/* --- Test metafunction clref --- */
TEST(TypeTraitCommonReference, MetafunctionCLRef)
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
TEST(TypeTraitCommonReference, MetafunctionRRef)
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
TEST(TypeTraitCommonReference, MetafunctionXRef)
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

/* Test metafunction common_reference --- */
TEST(TypeTraitCommonReference, MetafunctionCommonReference)
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

    // Base rref: using EXPECT to avoid aborting the test
    static_assert(std::is_same_v<common_reference_t<Base&&, const Derived>, Base>);
    // todo: check why this is compiler dependent
    bool resultCommonReference1 = std::is_same_v<common_reference_t<Base&&, Derived>, TRANSFORM_REF(Base)>;
    EXPECT_TRUE(resultCommonReference1) << "Compiler depedent: gcc expects Base&& - clang expects Base";

    bool resultCommonReference2 = std::is_same_v<common_reference_t<const Base&&, Derived>, TRANSFORM_REF(const Base)>;
    EXPECT_TRUE(resultCommonReference2) << "Compiler depedent: gcc expects const Base&& - clang expects const Base";

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

#undef TRANSFORM_REF
