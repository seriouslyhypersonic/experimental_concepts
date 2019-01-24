/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * Acknowledgments: Reimplementation of std::invoke as constexpr.
 *                  Copyright: Tristan Brindle
 *                     GitHub: https://github.com/tcbrindle/NanoRange/
 */
#ifndef DETAIL_INVOKE_H
#define DETAIL_INVOKE_H

#include <functional>
#include <utility>
#include <type_traits>

namespace detail
{

inline namespace invoke_
{

/* --- metafunction is_reference_wrapper --- */
template<class>
constexpr bool is_reference_wrapper_v = false;

template<class T>
constexpr bool is_reference_wrapper_v<std::reference_wrapper<T>> = true;

struct fn
{
private:

    // Member function

    template<class Base, class T, class Derived, class... Args>
    static constexpr auto
    impl(T Base::*pmf, Derived&& ref, Args&&... args) noexcept(noexcept((
        std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...)))
    -> std::enable_if_t<std::is_function_v<T> &&
                           std::is_base_of_v<Base, std::decay_t<Derived>>
                       ,decltype((std::forward<Derived>(ref).*pmf)
                           (std::forward<Args>(args)...))>
    {
        return (std::forward<Derived>(ref).*pmf)(std::forward<Args>(args)...);
    }

    template<class Base, class T, class RefWrap, class... Args>
    static constexpr auto
    impl(T Base::*pmf, RefWrap&& ref, Args&&... args) noexcept(noexcept((
        ref.get().*pmf)(std::forward<Args>(args)...)))
    -> std::enable_if_t<std::is_function_v<T> &&
                           is_reference_wrapper_v<std::decay_t<RefWrap>>
                       ,decltype((ref.get().*pmf)(std::forward<Args>(args)...))>
    {
        return (ref.get().*pmf)(std::forward<Args>(args)...);
    }

    template<class Base, class T, class Pointer, class... Args>
    static constexpr auto
    impl(T Base::*pmf, Pointer&& ptr, Args&&... args) noexcept(noexcept((
        *std::forward<Pointer>(ptr).*pmf)(std::forward<Args>(args)...)))
    -> std::enable_if_t<std::is_function_v<T> &&
                           !is_reference_wrapper_v<std::decay_t<Pointer>> &&
                           !std::is_base_of_v<Base, std::decay_t<Pointer>>
                       , decltype(((*std::forward<Pointer>(*ptr)).*pmf)
                           (std::forward<Args>(args)...))>
    {
        return ((*std::forward<Pointer>(ptr)).*pmf)(std::forward<Args>(args)...);
    }

    // Data member

    template<class Base, class T, class Derived>
    static constexpr auto
    impl(T Base::*pmd, Derived&& ref) noexcept(noexcept(
        std::forward<Derived>(ref).*pmd))
    -> std::enable_if_t<!std::is_function_v<T> &&
                           std::is_base_of_v<Base, std::decay_t<Derived>>
                       ,decltype(std::forward<Derived>(ref).*pmd)>
    {
        return std::forward<Derived>(ref).*pmd;
    }

    template<class Base, class T, class RefWrap>
    static constexpr auto
    impl(T Base::*pmd, RefWrap&& ref) noexcept(noexcept(ref.get().*pmd))
    -> std::enable_if_t<!std::is_function_v<T> &&
                           is_reference_wrapper_v<std::decay_t<RefWrap>>
                       , decltype(ref.get().*pmd)>
    {
        return ref.get().*pmd;
    }

    template<class Base, class T, class Pointer>
    static constexpr auto
    impl(T Base::*pmd, Pointer&& ptr) noexcept(noexcept((
        *std::forward<Pointer>(ptr)).*pmd))
    -> std::enable_if_t<!std::is_function_v<T> &&
                           !is_reference_wrapper_v<std::decay_t<Pointer>> &&
                           !std::is_base_of_v<Base, std::decay_t<Pointer>>
                       ,decltype((*std::forward<Pointer>(ptr)).*pmd)>
    {
        return (*std::forward<Pointer>(ptr)).*pmd;
    }

    // Function

    template<class F, class... Args>
    static constexpr auto
    impl(F&& f, Args&&... args) noexcept(noexcept(
        std::forward<F>(f)(std::forward<Args>(args)...)))
    -> std::enable_if_t<!std::is_member_pointer_v<std::decay_t<F>>
                       ,decltype(std::forward<F>(f)
                           (std::forward<Args>(args)...))>
    {
        return std::forward<F>(f)(std::forward<Args>(args)...);
    }

public:
    template<class F, class... Args>
    constexpr auto operator()(F&& f, Args&&... args) const noexcept(noexcept(
        fn::impl(std::forward<F>(f), std::forward<Args>(args)...)))
    -> decltype(fn::impl(std::forward<F>(f), std::forward<Args>(args)...))
    {
        return fn::impl(std::forward<F>(f), std::forward<Args>(args)...);
    }
};

} // namespace invoke
} // namespace detail

inline detail::invoke_::fn invoke;

namespace detail
{
template<class Void, class, class...>
struct invoke_result_helper { };

template<class F, class... Args>
struct invoke_result_helper<
    std::void_t<decltype(invoke(std::declval<F>(), std::declval<Args>()...))>
    ,F
    ,Args...>
{
        using type =
            decltype(invoke(std::declval<F>(), std::declval<Args>()...));
};

} // namespace detail

template<class F, class... Args>
struct invoke_result: detail::invoke_result_helper<void, F, Args...>
{ };

template<class F, class... Args>
using invoke_result_t = typename invoke_result<F, Args...>::type;

#endif //DETAIL_INVOKE_H
