/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <type_traits>
#include <string>

#include <gtest/gtest.h>

#include <conceptslib/concepts.hpp>
#include <conceptslib/type_traits.hpp>

#define CONCEPT_ASSERT(expr) static_assert(expr, #expr)

class CoreLanguageConceptTest: public ::testing::Test
{
public:
    struct B;
    struct C;
    struct D;

    struct A { A() = default; A(B) { }; A(C) { } A(D) { };};

    struct B { B() = default; B(int) { }; };

    struct C {
        C() = default; explicit C(int) { };
        explicit operator int () { return {}; };
//        C(D) { };
    };

    struct D { D() = default; D(B) { }; D(C) { };};

protected:
    struct Base { };
    struct Derived: Base { };
    struct ProtectedDerived: protected Base { };
    struct PrivateDerived: private Base { };
    struct NotDerived { };

    struct FromIntOnly
    {
        FromIntOnly(int);

        template<class T>
        FromIntOnly(T) = delete;
    };

    struct ToInt
    {
        operator int() const;
    };

    struct ExplicitToInt
    {
        explicit operator int() const;
    };
};

namespace traits
{
using A = CoreLanguageConceptTest::A;
using B = CoreLanguageConceptTest::B;
using C = CoreLanguageConceptTest::C;
using D = CoreLanguageConceptTest::D;

template <>
struct common_type<A, B> { using type = A; };

template <>
struct common_type<B, A> { using type = A; };

template <>
struct common_type<A, C> { using type = A; };

template <>
struct common_type<C, A> { using type = A; };

template <>
struct common_type<B, C> { using type = A; };

template <>
struct common_type<C, B> { using type = A; };

template <>
struct common_type<A, D> { using type = A; };

template <>
struct common_type<D, A> { using type = A; };

template <>
struct common_type<B, D> { using type = A; };

template <>
struct common_type<D, B> { using type = A; };
}

TEST_F(CoreLanguageConceptTest, ConceptSame)
{
    using concepts::Same;

    struct Incomplete;

    CONCEPT_ASSERT((Same<int, int>));
    CONCEPT_ASSERT((Same<float, float>));
    CONCEPT_ASSERT((Same<double, double>));
    CONCEPT_ASSERT((Same<void, void>));
    CONCEPT_ASSERT((!Same<int, double>));
    CONCEPT_ASSERT((!Same<int, float>));
    CONCEPT_ASSERT((!Same<float, double>));
    CONCEPT_ASSERT((!Same<int , void>));

    CONCEPT_ASSERT((!Same<Incomplete, void>));
    CONCEPT_ASSERT((Same<Incomplete, Incomplete>));

    CONCEPT_ASSERT((Same<Base, Base>));
    CONCEPT_ASSERT((Same<Derived, Derived>));
    CONCEPT_ASSERT((!Same<Base, Derived>));
}

TEST_F(CoreLanguageConceptTest, ConceptDerivedFrom)
{
    using concepts::DerivedFrom;

    CONCEPT_ASSERT((!DerivedFrom<int, int>));
    CONCEPT_ASSERT((!DerivedFrom<int, float>));
    CONCEPT_ASSERT((!DerivedFrom<int, double>));

    CONCEPT_ASSERT((DerivedFrom<Derived, Base>));
    CONCEPT_ASSERT((!DerivedFrom<ProtectedDerived, Base>));
    CONCEPT_ASSERT((!DerivedFrom<PrivateDerived, Base>));
}

TEST_F(CoreLanguageConceptTest, ConceptConvertibleTo)
{
    using concepts::ConvertibleTo;

    CONCEPT_ASSERT((ConvertibleTo<void, void>));
    CONCEPT_ASSERT((!ConvertibleTo<int, void>));
    CONCEPT_ASSERT((!ConvertibleTo<void, int>));

    CONCEPT_ASSERT((ConvertibleTo<int, int>));
    CONCEPT_ASSERT((ConvertibleTo<int, const int>));
    CONCEPT_ASSERT((ConvertibleTo<const int, int>));
    CONCEPT_ASSERT((ConvertibleTo<volatile int, int>));
    CONCEPT_ASSERT((ConvertibleTo<int, volatile int>));

    CONCEPT_ASSERT((ConvertibleTo<int*, int*>));
    CONCEPT_ASSERT((ConvertibleTo<int&, int>));
    CONCEPT_ASSERT((ConvertibleTo<int&, const volatile int>));
    CONCEPT_ASSERT((ConvertibleTo<int&, const int&>));
    CONCEPT_ASSERT((!ConvertibleTo<const int&, int&>));
    CONCEPT_ASSERT((!ConvertibleTo<int&, int&&>));
    CONCEPT_ASSERT((!ConvertibleTo<int&&, int&>));
    CONCEPT_ASSERT((ConvertibleTo<int&&, const int&>));

    CONCEPT_ASSERT((ConvertibleTo<int, bool>));
    CONCEPT_ASSERT((ConvertibleTo<int, float>));
    CONCEPT_ASSERT((ConvertibleTo<int, double>));
    CONCEPT_ASSERT((ConvertibleTo<bool, int>));
    CONCEPT_ASSERT((ConvertibleTo<float, int>));
    CONCEPT_ASSERT((ConvertibleTo<double, int>));

    CONCEPT_ASSERT((ConvertibleTo<Base, Base>));
    CONCEPT_ASSERT((ConvertibleTo<Derived, Derived>));
    CONCEPT_ASSERT((ConvertibleTo<Derived, Base>));
    CONCEPT_ASSERT((!ConvertibleTo<ProtectedDerived, Base>));
    CONCEPT_ASSERT((!ConvertibleTo<PrivateDerived, Base>));
    CONCEPT_ASSERT((!ConvertibleTo<Base, Derived>));

    CONCEPT_ASSERT((ConvertibleTo<Base*, Base*>));
    CONCEPT_ASSERT((ConvertibleTo<Derived*, Derived*>));
    CONCEPT_ASSERT((ConvertibleTo<Derived*, Base*>));
    CONCEPT_ASSERT((!ConvertibleTo<Base*, Derived*>));

    CONCEPT_ASSERT((ConvertibleTo<Base&, Base&>));
    CONCEPT_ASSERT((ConvertibleTo<Derived&, Derived&>));
    CONCEPT_ASSERT((ConvertibleTo<Derived&, Base&>));
    CONCEPT_ASSERT((!ConvertibleTo<Base&, Derived&>));

    CONCEPT_ASSERT((ConvertibleTo<int, FromIntOnly>));
    CONCEPT_ASSERT((!ConvertibleTo<long, FromIntOnly>));
    CONCEPT_ASSERT((!ConvertibleTo<FromIntOnly, int>));

    CONCEPT_ASSERT((ConvertibleTo<ToInt, int>));
    CONCEPT_ASSERT((ConvertibleTo<ToInt, long>));

    CONCEPT_ASSERT((!ConvertibleTo<ExplicitToInt, int>));

    CONCEPT_ASSERT((ConvertibleTo<int*, void*>));
    CONCEPT_ASSERT((!ConvertibleTo<void*, int*>)); // Must be explicit cast

    CONCEPT_ASSERT((ConvertibleTo<const char*, std::string>));
    CONCEPT_ASSERT((!ConvertibleTo<std::string, const char*>));
}

TEST_F(CoreLanguageConceptTest, ConceptCommonReference)
{
    using concepts::CommonReference;

    CONCEPT_ASSERT((!CommonReference<int, void>));
    CONCEPT_ASSERT((CommonReference<int, int>));
    CONCEPT_ASSERT((CommonReference<int&, int&>));
    CONCEPT_ASSERT((CommonReference<int&&, int&&>));

    CONCEPT_ASSERT((CommonReference<const int&, int&>));

    CONCEPT_ASSERT((CommonReference<int&&, const int&>));
    CONCEPT_ASSERT((CommonReference<const int&, int&&>));

    CONCEPT_ASSERT((!CommonReference<int, void>));
    CONCEPT_ASSERT((CommonReference<void, void>));
    using void_cr = traits::common_reference_t<void, void>;
    CONCEPT_ASSERT((concepts::Same<void_cr, void>));
}

TEST_F(CoreLanguageConceptTest, ConceptCommon)
{
    using concepts::Common;

    CONCEPT_ASSERT((Common<int, int>));

    CONCEPT_ASSERT((Common<A, A>));

    CONCEPT_ASSERT((Common<B, int>));
    CONCEPT_ASSERT((Common<int, B>));


    // Exists cast C <=> int
//    static_cast<int>(C{});
//    static_cast<C>(int{});
    // Needs explicit cast
    static_assert(!std::is_same_v<traits::common_type<C, int>, int>);

    CONCEPT_ASSERT((!Common<C, int>));

    // Specialization of common_type for A, B
    static_assert(std::is_same_v<traits::common_type_t<A, B>, A>);
    static_assert(std::is_same_v<traits::common_type_t<B, A>, A>);
    static_assert(std::is_same_v<traits::common_reference_t<A, B>, A>);
    static_assert(std::is_same_v<traits::common_reference_t<B, A>, A>);;
//    static_cast<A>(B{});

    CONCEPT_ASSERT((Common<A, B>));
    CONCEPT_ASSERT((Common<B, A>));

    // Specialization of common_type for A, C
    static_assert(std::is_same_v<traits::common_type_t<A, C>, A>);
    static_assert(std::is_same_v<traits::common_type_t<C, A>, A>);
    static_assert(std::is_same_v<traits::common_reference_t<A, C>, A>);
    static_assert(std::is_same_v<traits::common_reference_t<C, A>, A>);;
//    static_cast<A>(C{});

    CONCEPT_ASSERT((Common<A, C>));
    CONCEPT_ASSERT((Common<C, A>));

    // A is the common type of B and C
    static_assert(std::is_same_v<traits::common_type_t<B, C>, A>);
    static_assert(std::is_same_v<traits::common_type_t<C, B>, A>);
    // A is the common type of B and C
    static_assert(std::is_same_v<traits::common_reference_t<B, C>, A>);
    static_assert(std::is_same_v<traits::common_reference_t<B, C>, A>);;
//    static_cast<A>(B{});
//    static_cast<A>(C{});

    CONCEPT_ASSERT((Common<B, C>));
    CONCEPT_ASSERT((Common<C, B>));

    // A is the common type of B and D
    static_assert(std::is_same_v<traits::common_type_t<B, D>, A>);
    static_assert(std::is_same_v<traits::common_type_t<D, B>, A>);
//    static_cast<A>(B{});
//    static_cast<A>(D{});

    // B implicitly convertible to D and the common_reference precedence rules
    // state that decltype(false? val<B>() : val<D>()) has higher precedence
    // over common_type_t<B, D>
    static_assert(std::is_same_v<traits::common_reference_t<B, D>, D>);
    static_assert(std::is_same_v<traits::common_reference_t<D, B>, D>);

    static_assert(std::is_same_v<traits::common_reference_t<const B&, const D&>, const D>);

//    std::cout << typeid(traits::common_reference_t<const B&, const D&>).name() << '\n';

    CONCEPT_ASSERT((Common<B, D>));

//    static_assert(concepts::CommonReference<A&, >);


    CONCEPT_ASSERT((!Common<C, int>));
}