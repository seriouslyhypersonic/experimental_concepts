/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef DETAIL_INVOCABLE_WORKAROUND_H
#define DETAIL_INVOCABLE_WORKAROUND_H

#include <conceptslib/detail/concepts/concepts.hpp>
#include <conceptslib/detail/functional/invoke.hpp>

namespace platform
{
template<class F, class... Args>
using invoke_result_t = functional::invoke_result_t<F, Args...>;


REQUIREMENT InvocableReqImp
{
    // Alternative implementation
    template<class F, class... Args>
    auto REQUIRES(F&&, Args&&... args) -> invoke_result_t<F, Args...>;
};

} // namespace platform

#endif //DETAIL_INVOCABLE_WORKAROUND_H
