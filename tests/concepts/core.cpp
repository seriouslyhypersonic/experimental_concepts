/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <type_traits>
#include <string>
#include <memory>

#include <gtest/gtest.h>

#include <conceptslib/concepts.hpp>
#include <conceptslib/type_traits.hpp>

#define CONCEPT_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)

class CoreLanguageConcepts: public ::testing::Test
{
public:
    struct Incomplete;

    struct ImplicitlyConvertibleToA;

    struct A { A() = default; };

    struct ImplicitlyConvertibleToA { operator A() { return {}; } };
    struct ExplicitlyConvertibleToA { explicit operator A() { return {}; } };

    struct B { B(int) { } };

    // For CommonReference:
    struct NotA { };
    // For this to work, A and NotA must be convertible to CommonRefANotA
    struct CommonRefANotA { CommonRefANotA(A) { } CommonRefANotA(NotA) { }};

    // For Common
    struct NotAButConvertibleWithCommonRef
    {
        operator A() const { return { }; }
    };

    struct NotAButConvertibleWithNonConstCommonRef
    {
        // Note non-const member function
        operator A()  { return { }; }
    };

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

    struct NoThrowDestructor { ~NoThrowDestructor() noexcept = default; };
    struct ThrowDestructor { ~ThrowDestructor() noexcept(false); };
    class PrivateDestructor { ~PrivateDestructor() = default; };

    struct NoMove { NoMove(NoMove&&) = delete; };
    struct NoCopy { NoCopy(const NoCopy&) = delete; };
    struct NoMoveNoCopy: NoMove, NoCopy
    { };
};

namespace traits
{
using A = CoreLanguageConcepts::A;
using NotA = CoreLanguageConcepts::NotA;
using CommonRefANotA = CoreLanguageConcepts::CommonRefANotA;

// Specialization A, NotA
template<template<class> class TQual,
template<class> class UQual>
struct basic_common_reference<A, NotA, TQual, UQual>
{
    using type = CommonRefANotA;
};
// Must specialize both A NotA and NotA A
template<template<class> class TQual,
template<class> class UQual>
struct basic_common_reference<NotA, A, UQual, TQual>
{
    using type = CommonRefANotA;
};

// Specialization A, NotAButConvertibleWithCommonRef
using NotAButConvertibleWithCommonRef =
CoreLanguageConcepts::NotAButConvertibleWithCommonRef;

template<template<class> class TQual,
template<class> class UQual>
struct basic_common_reference<A, NotAButConvertibleWithCommonRef, TQual, UQual>
{
    using type = A;
};

template<template<class> class TQual,
template<class> class UQual>
struct basic_common_reference<NotAButConvertibleWithCommonRef, A, UQual, TQual>
{
    using type = A;
};

} // namespace traits

/* --- Concept Same --- */
TEST_F(CoreLanguageConcepts, ConceptSame)
{
    using concepts::Same;

    CONCEPT_ASSERT(Same<int, int>);
    CONCEPT_ASSERT(Same<float, float>);
    CONCEPT_ASSERT(Same<double, double>);
    CONCEPT_ASSERT(Same<void, void>);
    CONCEPT_ASSERT(!Same<int, double>);
    CONCEPT_ASSERT(!Same<int, float>);
    CONCEPT_ASSERT(!Same<float, double>);
    CONCEPT_ASSERT(!Same<int , void>);

    CONCEPT_ASSERT(!Same<Incomplete, void>);
    CONCEPT_ASSERT(Same<Incomplete, Incomplete>);

    CONCEPT_ASSERT(Same<Base, Base>);
    CONCEPT_ASSERT(Same<Derived, Derived>);
    CONCEPT_ASSERT(!Same<Base, Derived>);
}

/* --- Concept DerivedFrom --- */
TEST_F(CoreLanguageConcepts, ConceptDerivedFrom)
{
    using concepts::DerivedFrom;

    CONCEPT_ASSERT(!DerivedFrom<int, int>);
    CONCEPT_ASSERT(!DerivedFrom<int, float>);
    CONCEPT_ASSERT(!DerivedFrom<int, double>);
    CONCEPT_ASSERT(!DerivedFrom<void, Incomplete>);

    CONCEPT_ASSERT(DerivedFrom<Derived, Base>);
    CONCEPT_ASSERT(!DerivedFrom<ProtectedDerived, Base>);
    CONCEPT_ASSERT(!DerivedFrom<PrivateDerived, Base>);
}

/* --- Concept ConvertibleTo --- */
TEST_F(CoreLanguageConcepts, ConceptConvertibleTo)
{
    using concepts::ConvertibleTo;

    CONCEPT_ASSERT(ConvertibleTo<void, void>);
    CONCEPT_ASSERT(!ConvertibleTo<int, void>);
    CONCEPT_ASSERT(!ConvertibleTo<void, int>);

    CONCEPT_ASSERT(ConvertibleTo<int, int>);
    CONCEPT_ASSERT(ConvertibleTo<int, const int>);
    CONCEPT_ASSERT(ConvertibleTo<const int, int>);
    CONCEPT_ASSERT(ConvertibleTo<volatile int, int>);
    CONCEPT_ASSERT(ConvertibleTo<int, volatile int>);

    CONCEPT_ASSERT(ConvertibleTo<int*, int*>);
    CONCEPT_ASSERT(ConvertibleTo<int&, int>);
    CONCEPT_ASSERT(ConvertibleTo<int&, const volatile int>);
    CONCEPT_ASSERT(ConvertibleTo<int&, const int&>);
    CONCEPT_ASSERT(!ConvertibleTo<const int&, int&>);
    CONCEPT_ASSERT(!ConvertibleTo<int&, int&&>);
    CONCEPT_ASSERT(!ConvertibleTo<int&&, int&>);
    CONCEPT_ASSERT(ConvertibleTo<int&&, const int&>);

    CONCEPT_ASSERT(ConvertibleTo<int, bool>);
    CONCEPT_ASSERT(ConvertibleTo<int, float>);
    CONCEPT_ASSERT(ConvertibleTo<int, double>);
    CONCEPT_ASSERT(ConvertibleTo<bool, int>);
    CONCEPT_ASSERT(ConvertibleTo<float, int>);
    CONCEPT_ASSERT(ConvertibleTo<double, int>);

    CONCEPT_ASSERT(ConvertibleTo<Base, Base>);
    CONCEPT_ASSERT(ConvertibleTo<Derived, Derived>);
    CONCEPT_ASSERT(ConvertibleTo<Derived, Base>);
    CONCEPT_ASSERT(!ConvertibleTo<ProtectedDerived, Base>);
    CONCEPT_ASSERT(!ConvertibleTo<PrivateDerived, Base>);
    CONCEPT_ASSERT(!ConvertibleTo<Base, Derived>);

    CONCEPT_ASSERT(ConvertibleTo<Base*, Base*>);
    CONCEPT_ASSERT(ConvertibleTo<Derived*, Derived*>);
    CONCEPT_ASSERT(ConvertibleTo<Derived*, Base*>);
    CONCEPT_ASSERT(!ConvertibleTo<ProtectedDerived*, Base*>);
    CONCEPT_ASSERT(!ConvertibleTo<PrivateDerived*, Base*>);
    CONCEPT_ASSERT(!ConvertibleTo<Base*, Derived*>);

    CONCEPT_ASSERT(ConvertibleTo<Base&, Base&>);
    CONCEPT_ASSERT(ConvertibleTo<Derived&, Derived&>);
    CONCEPT_ASSERT(ConvertibleTo<Derived&, Base&>);
    CONCEPT_ASSERT(!ConvertibleTo<ProtectedDerived&, Base&>);
    CONCEPT_ASSERT(!ConvertibleTo<PrivateDerived&, Base&>);
    CONCEPT_ASSERT(!ConvertibleTo<Base&, Derived&>);

    CONCEPT_ASSERT(ConvertibleTo<int, FromIntOnly>);
    CONCEPT_ASSERT(!ConvertibleTo<long, FromIntOnly>);
    CONCEPT_ASSERT(!ConvertibleTo<FromIntOnly, int>);

    CONCEPT_ASSERT(ConvertibleTo<ToInt, int>);
    CONCEPT_ASSERT(ConvertibleTo<ToInt, long>);

    CONCEPT_ASSERT(!ConvertibleTo<ExplicitToInt, int>);

    CONCEPT_ASSERT(ConvertibleTo<int*, void*>);
    CONCEPT_ASSERT(!ConvertibleTo<void*, int*>); // Must be explicit cast

    CONCEPT_ASSERT(ConvertibleTo<const char*, std::string>);
    CONCEPT_ASSERT(!ConvertibleTo<std::string, const char*>);

    CONCEPT_ASSERT(ConvertibleTo<ImplicitlyConvertibleToA, A>);
    CONCEPT_ASSERT(!ConvertibleTo<ExplicitlyConvertibleToA, A>);
}

/* --- Concept CommonReference --- */
TEST_F(CoreLanguageConcepts, ConceptCommonReference)
{
    using concepts::CommonReference;

    CONCEPT_ASSERT(!CommonReference<int, void>);
    CONCEPT_ASSERT(CommonReference<int, int>);
    CONCEPT_ASSERT(CommonReference<int&, int&>);
    CONCEPT_ASSERT(CommonReference<int&&, int&&>);

    CONCEPT_ASSERT(CommonReference<const int&, int&>);

    CONCEPT_ASSERT(CommonReference<int&&, const int&>);
    CONCEPT_ASSERT(CommonReference<const int&, int&&>);

    CONCEPT_ASSERT(!CommonReference<int, void>);
    CONCEPT_ASSERT(CommonReference<void, void>);
    using void_cr = traits::common_reference_t<void, void>;
    CONCEPT_ASSERT((concepts::Same<void_cr, void>));

    CONCEPT_ASSERT(CommonReference<Base, Derived>);
    CONCEPT_ASSERT(!CommonReference<Base, ProtectedDerived>);
    CONCEPT_ASSERT(!CommonReference<Base, PrivateDerived>);

    CONCEPT_ASSERT(CommonReference<A, ImplicitlyConvertibleToA>);
    CONCEPT_ASSERT(!CommonReference<A, ExplicitlyConvertibleToA>);

    // Specialization of basic_common_reference
    CONCEPT_ASSERT(CommonReference<A, NotA>);

    static_assert(std::is_same_v<traits::common_reference_t<A, NotA>
                                ,CommonRefANotA>);
    static_assert(std::is_same_v<traits::common_reference_t<NotA, A>
                                ,CommonRefANotA>);

    CONCEPT_ASSERT(CommonReference<const A, NotA>);
    CONCEPT_ASSERT(CommonReference<A, const NotA>);
    CONCEPT_ASSERT(CommonReference<const A, const NotA>);
}

/* --- Concept Common --- */
TEST_F(CoreLanguageConcepts, ConceptCommon)
{
    using concepts::Common;
    using concepts::CommonReference;
    using concepts::Same;

    CONCEPT_ASSERT(Common<int, int>);
    CONCEPT_ASSERT(Common<B, int>);

    CONCEPT_ASSERT(Common<A, A>);

    CONCEPT_ASSERT(!Common<A, ImplicitlyConvertibleToA>);
    // Because even though
    static_assert(Same<traits::common_type_t<A, ImplicitlyConvertibleToA>
                      ,traits::common_type_t<ImplicitlyConvertibleToA, A>>);
    // There is no CommonReference
    static_assert(!CommonReference<const ImplicitlyConvertibleToA&, const A&>);

    CONCEPT_ASSERT(!Common<A, NotA>);
    // Because even though
    static_assert(CommonReference<const NotA&, const A&>);
    // There is no common_type
    static_assert(!traits::is_detected_v<traits::common_type_t, A, NotA>);

    CONCEPT_ASSERT(Common<A, NotAButConvertibleWithCommonRef>);
    static_assert(
        Same<traits::common_type_t<A, NotAButConvertibleWithCommonRef>
            ,traits::common_type_t<NotAButConvertibleWithCommonRef, A>>);
    static_assert(CommonReference<const NotAButConvertibleWithCommonRef&
                                 ,const A&>);

    CONCEPT_ASSERT(!Common<A, NotAButConvertibleWithNonConstCommonRef>);
    // Because even though
    static_assert(
        Same<traits::common_type_t<A ,NotAButConvertibleWithNonConstCommonRef>
            ,traits::common_type_t<NotAButConvertibleWithNonConstCommonRef, A>>);
    // (Due to non-const cast operator) there is no
    static_assert(
        !CommonReference<const A&
                        ,const NotAButConvertibleWithNonConstCommonRef&>);

}

/* --- Concept Integral --- */
TEST_F(CoreLanguageConcepts, ConceptIntegral)
{
    using concepts::Integral;

    CONCEPT_ASSERT(Integral<char>);
    CONCEPT_ASSERT(Integral<char>);
    CONCEPT_ASSERT(Integral<unsigned char>);
    CONCEPT_ASSERT(Integral<bool>);
    CONCEPT_ASSERT(Integral<int>);
    CONCEPT_ASSERT(Integral<long>);
    CONCEPT_ASSERT(Integral<long long>);

    CONCEPT_ASSERT(!Integral<int&>);
    CONCEPT_ASSERT(!Integral<float>);
    CONCEPT_ASSERT(!Integral<double>);
    CONCEPT_ASSERT(!Integral<void>);

    CONCEPT_ASSERT(!Integral<A>);
}

/* --- Concept SignedIntegral --- */
TEST_F(CoreLanguageConcepts, ConceptSignedInteral)
{
    using concepts::SignedInteral;

    CONCEPT_ASSERT(SignedInteral<char>);
    CONCEPT_ASSERT(SignedInteral<char>);
    CONCEPT_ASSERT(SignedInteral<int>);
    CONCEPT_ASSERT(SignedInteral<long>);
    CONCEPT_ASSERT(SignedInteral<long long>);

    CONCEPT_ASSERT(!SignedInteral<bool>);
    CONCEPT_ASSERT(!SignedInteral<unsigned char>);
    CONCEPT_ASSERT(!SignedInteral<unsigned int>);
    CONCEPT_ASSERT(!SignedInteral<unsigned long>);
    CONCEPT_ASSERT(!SignedInteral<unsigned long long>);

    CONCEPT_ASSERT(!SignedInteral<float>);
    CONCEPT_ASSERT(!SignedInteral<double>);
    CONCEPT_ASSERT(!SignedInteral<void>);

    CONCEPT_ASSERT(!SignedInteral<A>);
}

/* --- Concept UnsignedIntegral --- */
TEST_F(CoreLanguageConcepts, ConceptUnsignedIntegral)
{
    using concepts::UnsignedInteral;

    CONCEPT_ASSERT(UnsignedInteral<bool>);
    CONCEPT_ASSERT(UnsignedInteral<unsigned char>);
    CONCEPT_ASSERT(UnsignedInteral<unsigned int>);
    CONCEPT_ASSERT(UnsignedInteral<unsigned long>);
    CONCEPT_ASSERT(UnsignedInteral<unsigned long long>);

    CONCEPT_ASSERT(!UnsignedInteral<char>);
    CONCEPT_ASSERT(!UnsignedInteral<char>);
    CONCEPT_ASSERT(!UnsignedInteral<int>);
    CONCEPT_ASSERT(!UnsignedInteral<long>);
    CONCEPT_ASSERT(!UnsignedInteral<long long>);

    CONCEPT_ASSERT(!UnsignedInteral<float>);
    CONCEPT_ASSERT(!UnsignedInteral<double>);
    CONCEPT_ASSERT(!UnsignedInteral<void>);

    CONCEPT_ASSERT(!UnsignedInteral<A>);
}

/* --- Concept Assignable --- */
TEST_F(CoreLanguageConcepts, ConceptAssignable)
{
    using concepts::Assignable;

    struct RegularAssign{ RegularAssign& operator=(const RegularAssign&); };

    // Note: operator=() does not yield an lvalue referring to the left operand
    struct OddAssign { OddAssign operator=(const OddAssign&); };

    CONCEPT_ASSERT(Assignable<int&, int>);
    CONCEPT_ASSERT(Assignable<int&, int&>);
    CONCEPT_ASSERT(Assignable<int&, int&&>);

    CONCEPT_ASSERT(!Assignable<int, int>);
    CONCEPT_ASSERT(!Assignable<int, int&>);
    CONCEPT_ASSERT(!Assignable<int, int&&>);

    CONCEPT_ASSERT(!Assignable<const int&, int>);
    CONCEPT_ASSERT(!Assignable<const int&, int&>);
    CONCEPT_ASSERT(!Assignable<const int&, int&&>);

    CONCEPT_ASSERT(!Assignable<int&&, int>); // Is this correct?
    CONCEPT_ASSERT(!Assignable<int&&, int&>); // Is this correct?
    CONCEPT_ASSERT(!Assignable<int&&, int&&>); // Is this correct?
    
    CONCEPT_ASSERT(Assignable<std::string&, const char*>);
    CONCEPT_ASSERT(!Assignable<void, int>);

    CONCEPT_ASSERT(Assignable<RegularAssign&, RegularAssign>);
    CONCEPT_ASSERT(Assignable<RegularAssign&, RegularAssign&>);
    CONCEPT_ASSERT(Assignable<RegularAssign&, RegularAssign&&>);

    CONCEPT_ASSERT(!Assignable<const RegularAssign&, RegularAssign>);
    CONCEPT_ASSERT(!Assignable<const RegularAssign&, RegularAssign&>);
    CONCEPT_ASSERT(!Assignable<const RegularAssign&, RegularAssign&&>);

    CONCEPT_ASSERT(!Assignable<OddAssign&, OddAssign>);
    CONCEPT_ASSERT(!Assignable<OddAssign&, OddAssign&>);
    CONCEPT_ASSERT(!Assignable<OddAssign&, OddAssign&&>);
}

/* --- Concept Swappable --- */
TEST_F(CoreLanguageConcepts, ConceptSwappable)
{
    using concepts::Swappable;
    using concepts::SwappableWith;

    CONCEPT_ASSERT(Swappable<int>);
    CONCEPT_ASSERT(Swappable<std::string>);
    CONCEPT_ASSERT(Swappable<Base>);
    CONCEPT_ASSERT(Swappable<Derived>);
    CONCEPT_ASSERT(Swappable<ProtectedDerived>);
    CONCEPT_ASSERT(Swappable<PrivateDerived>);

    CONCEPT_ASSERT(Swappable<int*>);
    CONCEPT_ASSERT(Swappable<Base*>);
    CONCEPT_ASSERT(Swappable<Derived*>);

    CONCEPT_ASSERT(Swappable<int&>);
    CONCEPT_ASSERT(Swappable<Base&>);
    CONCEPT_ASSERT(Swappable<Derived&>);

    CONCEPT_ASSERT(Swappable<int&&>);
    CONCEPT_ASSERT(Swappable<Base&&>);
    CONCEPT_ASSERT(Swappable<Derived&&>);

    CONCEPT_ASSERT(!Swappable<void>);

    CONCEPT_ASSERT(SwappableWith<Base&, Base&>);
    CONCEPT_ASSERT(!SwappableWith<Base&, Derived&>);
    CONCEPT_ASSERT(!SwappableWith<Derived&, Base&>);
    CONCEPT_ASSERT(SwappableWith<Derived&, Derived&>);

    CONCEPT_ASSERT(!SwappableWith<void, void>);
    CONCEPT_ASSERT(SwappableWith<int&, int&>);
    CONCEPT_ASSERT(!SwappableWith<int&, long&>);
    CONCEPT_ASSERT(!SwappableWith<int&, const int&>);
}

/* --- Concept Destructible --- */
TEST_F(CoreLanguageConcepts, ConceptDestructible)
{
    using concepts::Destructible;

    CONCEPT_ASSERT(Destructible<NoThrowDestructor>);
    CONCEPT_ASSERT(!Destructible<ThrowDestructor>);
    CONCEPT_ASSERT(!Destructible<PrivateDestructor>);

    CONCEPT_ASSERT(Destructible<int>);
    CONCEPT_ASSERT(Destructible<std::string>);
    CONCEPT_ASSERT(!Destructible<void>);
}

/* --- Concept Constructible --- */
TEST_F(CoreLanguageConcepts, ConceptConstructible)
{
    using concepts::Constructible;

    CONCEPT_ASSERT(Constructible<Base, Derived>);
    CONCEPT_ASSERT(Constructible<Base, Derived&>);
    CONCEPT_ASSERT(Constructible<Base, const Derived&>);
    CONCEPT_ASSERT(Constructible<Base&, Derived&>);
    CONCEPT_ASSERT(Constructible<const Base&, Derived&>);
    CONCEPT_ASSERT(Constructible<const Base&, Derived&&>);
    CONCEPT_ASSERT(Constructible<const Base&, const Derived&>);

    CONCEPT_ASSERT(Constructible<FromIntOnly, int>);
    CONCEPT_ASSERT(Constructible<FromIntOnly, int&>);
    CONCEPT_ASSERT(!Constructible<FromIntOnly, char>);

    CONCEPT_ASSERT(!Constructible<void>);
    CONCEPT_ASSERT(Constructible<std::string, const char*>);
    CONCEPT_ASSERT(Constructible<std::string, const char(&)[]>);
    CONCEPT_ASSERT(Constructible<std::string, char, int, std::allocator<char>>);

}

/* --- Concept DefaultConstructible --- */
TEST_F(CoreLanguageConcepts, ConceptDefaultConstructible)
{
    using concepts::DefaultConstructible;

    CONCEPT_ASSERT(DefaultConstructible<int>);
    CONCEPT_ASSERT(DefaultConstructible<Base>);
    CONCEPT_ASSERT(DefaultConstructible<std::string>);
    CONCEPT_ASSERT(!DefaultConstructible<void>);

    CONCEPT_ASSERT(!DefaultConstructible<FromIntOnly>);
}

/* --- Concept MoveConstructible --- */
TEST_F(CoreLanguageConcepts, ConceptMoveMoveConstructible)
{
    using concepts::MoveConstructible;

    CONCEPT_ASSERT(MoveConstructible<Base>);
    CONCEPT_ASSERT(MoveConstructible<Derived>);
    CONCEPT_ASSERT(MoveConstructible<std::unique_ptr<int>>);
    CONCEPT_ASSERT(MoveConstructible<std::shared_ptr<int>>);

    CONCEPT_ASSERT(!MoveConstructible<NoMove>);
    CONCEPT_ASSERT(!MoveConstructible<NoCopy>);
    CONCEPT_ASSERT(!MoveConstructible<NoMoveNoCopy>);
}

/* --- Concept CopyConstructible --- */
TEST_F(CoreLanguageConcepts, ConceptCopyConstructible)
{
    using concepts::CopyConstructible;

    struct OddCopyCtor { OddCopyCtor(OddCopyCtor& ); };

    CONCEPT_ASSERT(CopyConstructible<Base>);
    CONCEPT_ASSERT(CopyConstructible<Derived>);
    CONCEPT_ASSERT(CopyConstructible<std::shared_ptr<int>>);
    CONCEPT_ASSERT(CopyConstructible<std::string>);

    CONCEPT_ASSERT(!CopyConstructible<void>);
    CONCEPT_ASSERT(!CopyConstructible<std::unique_ptr<int>>);
    CONCEPT_ASSERT(!CopyConstructible<NoCopy>);
    CONCEPT_ASSERT(!CopyConstructible<NoMove>);
    CONCEPT_ASSERT(!CopyConstructible<NoMoveNoCopy>);
    CONCEPT_ASSERT(!CopyConstructible<OddCopyCtor>);
}

