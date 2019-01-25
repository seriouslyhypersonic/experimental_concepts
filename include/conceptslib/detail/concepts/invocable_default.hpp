/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef DETAIL_INVOCABLE_DEFAULT_H
#define DETAIL_INVOCABLE_DEFAULT_H

#include <functional>
#include <utility>

#include <conceptslib/detail/concepts/concepts.hpp>

namespace platform
{
using std::invoke;

template<class F, class... Args>
using invoke_result_t = std::invoke_result_t<F, Args...>;

REQUIREMENT InvocableReqImp
{
    // As spec'd by the concepts library:
    // Compiles fine with gcc and Clang 7 (but not 6) on linux
    // Does not compile with Apple Clang version 10
    template<class F, class... Args>
    auto REQUIRES(F &&f, Args &&... args) -> decltype(
    invoke(std::forward<F>(f), std::forward<Args>(args)...)
    );
};

} // namespace platform

#endif //DETAIL_INVOCABLE_DEFAULT_H
