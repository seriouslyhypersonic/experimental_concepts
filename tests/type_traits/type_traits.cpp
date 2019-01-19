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

/* --- Test metafunction remove_cvref --- */
TEST(TypeTraits, MetafunctionRemoveCVRef)
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
TEST(TypeTraits, MetafunctionCopyCV)
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
TEST(TypeTraits, MetafunctionSelect)
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
TEST(TypeTraits, TypeTraitHasType)
{
    struct WithType { using type = void; };
    struct WithoutType { };

    static_assert(traits::has_type<WithType>::value);
    static_assert(!traits::has_type<WithoutType>::value);
}



