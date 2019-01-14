#ifndef EXPERIMENTAL_TYPE_TRAITS_H
#define EXPERIMENTAL_TYPE_TRAITS_H

#include <type_traits>

/**
 * @file experimental_type_traits.hpp
 *
 * Resources:
 *  - https://en.cppreference.com/w/cpp/experimental/is_detected
 *  - https://www.youtube.com/watch?v=o1ekBpEFcPc
 *
 */

/* --- Detection idiom --- */

/* Aliases to introduce detection essentials into the global namespace */

/// Alias for true_type if Op<Args...> is valid. Otherwise alias for false_type.
template<template<class...> class Op, class... Args>
using is_detected = std::experimental::is_detected<Op, Args...>;

/**
 * Alias for Op<Args...> if it is well formed. Otherwise nonesuch.
 * @note Can be used for dependent lookups
 */
template<template<class...> class Op, class... Args>
using detected_t = std::experimental::detected_t<Op, Args...>;

/**
 * Alias for a struct with two member typedefs.
 * @typedef \c type Alias for Op<Args...> if it denotes a valid type. Otherwise
 *  alias for Default.
 * \@ypedef \c value_t equivalent to \c is_detected
 */
template< class Default, template<class...> class Op, class... Args>
using detected_or = std::experimental::detected_or<Default, Op, Args...>;

/* Aliases to introduce additional utilities into the global namespace */

// Non instantiatable type to indicate detection failure
using nonesuch = std::experimental::nonesuch;

template<template<class...> class Op, class... Args>
constexpr bool is_detected_v = std::experimental::is_detected_v<Op, Args...>;

template<class Default, template<class...> class Op, class... Args >
using detected_or_t = std::experimental::detected_or_t<Default, Op, Args...>;

template <class Expected, template<class...> class Op, class... Args>
using is_detected_exact =
std::experimental::is_detected_exact<Expected, Op, Args...>;

template <class Expected, template<class...> class Op, class... Args>
constexpr bool is_detected_exact_v =
               std::experimental::is_detected_exact_v<Expected, Op, Args...>;

template <class To, template<class...> class Op, class... Args>
using is_detected_convertible =
std::experimental::is_detected_convertible<To, Op, Args...>;

template <class To, template<class...> class Op, class... Args>
constexpr bool is_detected_convertible_v =
               std::experimental::is_detected_convertible_v<To, Op, Args...>;

namespace traits
{

// METAFUNCTIONS

using std::declval;

// --- metafunction remove_cvref
/**
 *  @metafunction Remove topmost cv-qualifiers of T or of the type refered by it
 *  @details If the type T is a reference type, provides the member typedef
 *  \c type which is the type referred to by T with its topmost cv-qualifiers
 *  removed. Otherwise type is T with its topmost cv-qualifiers removed.
 */
template<class T>
struct remove_cvref
{
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};
/// Helper typedef to access remove_cvref member typedef type
template<class T>
using remove_cvref_t = typename remove_cvref<T>::type;

//--- metafunction copy_cv
namespace detail
{
// Case: From has no cv qualifiers
template<class From, class To>
struct copy_cv { using type = To; };
//Case: From has const qualifier
template<class From, class To>
struct copy_cv <const From, To>: std::add_const<To> { };
// Case: From has volatile qualifier
template<class From, class To>
struct copy_cv <volatile From, To>: std::add_volatile<To> { };
// Case: From has const volatile qualifier
template<class From, class To>
struct copy_cv <const volatile From, To>: std::add_cv<To> { };
} // namespace detail
/**
 * @metafunctiion Copy cv qualifiers from type From to type To
 * @details Provides a member typedef \c type which is the same as T, except
 * the qualifiers from type From are added to type To.
 */
template<class From, class To>
using copy_cv = detail::copy_cv<From, To>;
/// Helper typedef to access the member \c type of copy_cv
template<class From, class To>
using copy_cv_t = typename copy_cv<From, To>::type;

// --- metafunction select
/**
 * \c condition is a wrapper for the \c select metafunction. It wraps the result
 * of a test, and the associated type of that test. It provides a member
 * typedef \c value for the result of the test, and a member typedef \c type
 * for the associated type.
 * @tparam B The result of the test
 * @tparam T Type associated with the test
 */
template<bool B, class T>
struct condition
{
    static constexpr bool value = B;
    using type = T;
};

/**
 * @metafunction Conditionally select a type.
 *
 * @details Selects a type based on the result of a set of conditions. Each
 * condition is associated with the result of a test, and a type. Conditions
 * are provided using the \c condition wrapper. A default type may be provided
 * as the last argument. If no default type is provided, at least one condition
 * should yeld a successful test. If multiple conditions are true, the first
 * will be selected.
 * Provides the member typedef \c type associated with the first successful
 * test.
 */
template<class Head, class... Tail>
struct select: std::conditional_t<Head::value, Head, select<Tail...>>
{ };

/// Base case for metafunction \c select: Tail is a default type
template<class T>
struct select<T>
{
    using type = T;
};

/// Base case for metafunction \c select: Tail is a condition
template<bool B, class T>
struct select<condition<B, T>>
{
    // The last conditon must be true, otherwise a default type must be provided
    static_assert(B, "select failed because there isn't any true condition");
    using type = T;
};

/// Helper typedef to access the member \c type of \c select
template<class Head, class... Tail>
using select_t = typename select<Head, Tail...>::type;

// TRAITS

// --- trait has_type
namespace detail
{
template<class T>
using has_type_imp = typename T::type;
} // namespace detail
/**
 * @trait Check if a type has the member typedef \c type
 * @details If type T has the member typedef \c type, provides the member
 * constant value which is equal to true. Otherwise, value is equal to false.
 */
template<class T>
struct has_type: std::bool_constant<is_detected_v<detail::has_type_imp, T>> {};

/// Helper variable template to access the member \c value of \c has_type
template<class T>
inline constexpr bool has_type_v = has_type<T>::value;

// --- EXPERIMENTAL IMPLEMENTATION OF common_reference !-!-!-!-!-!-!-!-!-!
// https://ericniebler.github.io/std/wg21/D0022.html#appendix-1-reference-implementations-of-common_type-and-common_reference

namespace experimental
{
///// Template alias for the type defined by trait T
//template<class T> using __t = typename T::type;
//
///// The value defined by trait T
//template<class T>
//constexpr typename __t<T>::value_type __v = __t<T>::value;
//
///// Store the type of T, effectively creating a trait
//template<class T> struct __id { using type = T; };
//
///// Add lvalue ref and const to T, which is, by default, striped of const
//template <class T, class X = std::remove_reference_t<T>>
//using __cref = std::add_lvalue_reference_t<std::add_const_t<X>>;
//
///// Remove cv and ref qualifiers
//template <class T>
//using __uncvref = remove_cvref_t<T>;
//
///// Existence of builtin common reference
//template <class T, class U>
//using __cond = decltype(declval<bool>() ? declval<T>() : declval<U>());
//
//// builtint_common
//
//// definition 1 - default case of no simple common reference
//template<class T, class U>
//struct __builtin_common_def1 { };
//
//// definition 2 - exists builtin common reference (because _req is detected)
//template<class T, class U>
//constexpr bool __builtin_common_def2_req =
//    is_detected_v<__cond, __cref<T>, __cref<U>>;
//template<class T, class U>
//struct __builtin_common_def2:
//    std::decay<detected_t<__cond, __cref<T>, __cref<U> >>
//{};
//
//// select the right definition for __builtin_common
//template<class T, class U>
//struct __builtin_common:
//    select_t<condition<__builtin_common_def2_req<T, U>
//                      ,__builtin_common_def2<T, U>
//                      >
//            ,__builtin_common_def1<T, U>> { };
//
//// helper typedef (__builtin_common_def1 has no member typedef type!)
//template<class T, class U>
//using __builtin_common_t = typename __builtin_common<T, U>::type;
//
//// Specialization T and U are rvalue references:
//// rref specialization if builtin common reference of T& and U& exists and
//// convertible to the that type's corresponding rref type.
//template<class T, class U, class R = __builtin_common_t<T&, U&>>
//using __rref_res =
//    std::conditional_t<std::is_reference_v<R>, std::remove_reference_t<R>&&, R>;
//
//template<class T, class U>
//constexpr bool __builtin_common_rref_spec_req =
//    is_detected_v<__builtin_common_t, T&, U&> &&
//    std::is_convertible_v<T&&, __rref_res<T, U>> &&
//    std::is_convertible_v<U&&, __rref_res<T, U>>;
//
//// spec in case __builtin_common_rref_spec_req met
//template<class T, class U>
//struct __builtin_common_rref_spec: __id<__rref_res<T, U>>{ };
//
//// spec in case __builtin_common_spec_req not met
//template<class T, class U>
//struct __builtin_common_no_rref_spec { /* no coomon reference */ };
//
//// select the right specialization
//template<class T, class U>
//struct __builtin_common<T&&, U&&>:
//    select_t<condition<__builtin_common_rref_spec_req<T, U>
//                      ,__builtin_common_rref_spec<T, U>
//                      >
//             ,__builtin_common_no_rref_spec<T, U>>
//{ };
//
//// Specialization T and U are lvalue references:
//// T and U are both lvalue references and there is a built in common reference
//// when the union of their cv qualifiers is applied to both types
//template<class T, class U>
//using __lref_res = __cond<copy_cv_t<T, U>&, copy_cv_t<U, T>&>;
//
//template<class T, class U>
//constexpr bool __builtin_common_lref_spec_req = is_detected_v<__lref_res, T, U>;
//
//// spec in case __builtin_common_lref_spec_req met
//template<class T, class U>
//struct __builtin_common_lref_spec: __id<detected_t<__lref_res, T, U>> { };
//
//// spec in case __builtin_common_lref_spec_req not met
//template<class T, class U>
//struct __built_in_common_no_lref_spec { /* no coomon reference */ };
//
//// select the right specialization
//template<class T, class U>
//struct __builtin_common<T&, U&>:
//    select_t<condition<__builtin_common_lref_spec_req<T, U>
//                      ,__builtin_common_lref_spec<T, U>
//                      >
//            ,__built_in_common_no_lref_spec<T,U>>
//{ };
//
//// Specialization one type is lref and the other is rref
//template<class T, class U>
//constexpr bool __builtin_common_rlref_spec_req =
//    is_detected_v<__builtin_common_t, T&, const U&> &&
//    std::is_convertible_v<U&&, detected_t<__builtin_common_t, T&, const U&>>;
//
//// spec in case __builtin_common_rlref_spec_req met
//template<class T, class U>
//struct __builtin_common_rlref_spec: __builtin_common<T&, const U&>
//{ };
//
//// spec in case __builtin_common_rlref_spec_req not met
//template<class T, class U>
//struct __builtin_common_no_rlref_spec { using type = void;/* no coomon reference */ };
//
//// select the right specializations for the two lvalue and rvalue combinations
//template<class T, class U>
//struct __builtin_common<T&, U&&>:
//    select_t<condition<__builtin_common_rlref_spec_req<T,U>
//                      ,__builtin_common_rlref_spec<T, U>
//                      >
//            ,__builtin_common_no_rlref_spec<T, U>>
//{ };
//
//template<class T, class U>
//struct __builtin_common<T&&, U&>: __builtin_common<U&, T&&> { };
//
//
//
//static_assert(std::is_same_v<__builtin_common_def2<const int, char*>::type, nonesuch>);
//static_assert(std::is_same_v<__builtin_common_t<int&&, int&>, const int&>);


// TESTS FOR EXPERIMENTAL IMPLEMENTATION ---------------------------------------
struct WithType { using type = void; };
struct WithoutType { };

//Test trait has_type
static_assert(has_type<WithType>::value);
static_assert(!has_type<WithoutType>::value);

// Test copy_cv
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

// Test select
struct TypeA {};
struct TypeB {};
struct TypeC {};
struct TypeD {};
struct TypeE {};

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

// Test __builtin_common

struct A { };
struct B: A { };
struct C { };

static_assert(std::is_same_v<__builtin_common_t<A, A>, A>);
static_assert(std::is_same_v<__builtin_common_t<B, B>, B>);
static_assert(std::is_same_v<__builtin_common_t<A, B>, A>);
static_assert(std::is_same_v<__builtin_common_t<B, A>, A>);
static_assert(!is_detected_v<__builtin_common_t, A, C>);
static_assert(!is_detected_v<__builtin_common_t, C, A>);

static_assert(std::is_same_v<__builtin_common_t<A&, A&>, A&>);
static_assert(std::is_same_v<__builtin_common_t<B&, B&>, B&>);
static_assert(std::is_same_v<__builtin_common_t<A&, B&>, A&>);
static_assert(std::is_same_v<__builtin_common_t<B&, A&>, A&>);
static_assert(!is_detected_v<__builtin_common_t, A&, C&>);
static_assert(!is_detected_v<__builtin_common_t, C&, A&>);

static_assert(std::is_same_v<__builtin_common_t<const A&, B&>, const A&>);
static_assert(std::is_same_v<__builtin_common_t<A&, const B&>, const A&>);
static_assert(std::is_same_v<__builtin_common_t<A& , volatile B&>, volatile A&>);
static_assert(std::is_same_v<__builtin_common_t<volatile A& , B&>, volatile A&>);
static_assert(std::is_same_v<__builtin_common_t<A& , const volatile B&>, const volatile A&>);
static_assert(std::is_same_v<__builtin_common_t<const volatile A& , B&>, const volatile A&>);
static_assert(!is_detected_v<__builtin_common_t,const A&, C&>);
static_assert(!is_detected_v<__builtin_common_t, A&, const C&>);

static_assert(std::is_same_v<__builtin_common_t<A&&, B&&>, A&&>);
static_assert(std::is_same_v<__builtin_common_t<B&&, A&&>, A&&>);
static_assert(std::is_same_v<__builtin_common_t<const A&&, B&&>, const A&&>);
static_assert(std::is_same_v<__builtin_common_t<const B&&, A&&>, const A&&>);

static_assert(std::is_same_v<__builtin_common_t<A&&, B&>, const A&>);
static_assert(std::is_same_v<__builtin_common_t<B&&, A&>, const A&>);
static_assert(std::is_same_v<__builtin_common_t<const A&&, B&>, const A&>);
static_assert(std::is_same_v<__builtin_common_t<const B&&, A&>, const A&>);
//static_assert(std::is_same_v<__builtin_common_t<volatile A&&, B&>, const volatile A>);

static_assert(std::is_same_v<__builtin_common_t<volatile int &&, int&>, void>);
//static_assert(std::is_same_v<__builtin_common_t<int , A&>, const A&>);
} // namespace experimental

} // namespace traits


#endif //EXPERIMENTAL_TYPE_TRAITS_H
