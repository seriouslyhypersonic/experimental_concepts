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

struct TypeTraitCommonType: ::testing::Test
{
    // No implicit conversion
    struct X { };
    struct Y { };
    struct Z { };
    struct W { };

    struct ImplicitToX { operator X() const { return {}; } };
    struct ExplicitToX { explicit operator X() const { return {}; } };
};

namespace std
{
    using X = TypeTraitCommonType::X;
    using Y = TypeTraitCommonType::Y;
    using Z = TypeTraitCommonType::Z;

    template<> struct common_type<X, Y> { using type = Z; };
    template<> struct common_type<Y, X> { using type = Z; };

    template<> struct common_type<X*, Y*> { using type = Z*; };
} // namespace std

namespace traits
{
    using X = TypeTraitCommonType::X;
    using Y = TypeTraitCommonType::Y;

    template<> struct common_type<X, Y>: std::common_type<X, Y> { };
    template<> struct common_type<Y, X>: std::common_type<Y, X> { };

    template<> struct common_type<X*, Y*>: std::common_type<X*, Y*> { };
} // namespace traits

TEST_F(TypeTraitCommonType, MetafunctionCommonType)
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
