/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <iostream>
#include <type_traits>

#include <conceptslib/type_traits.hpp>
#include <conceptslib/detail/concepts/core.hpp>

#include "test_type_traits.hpp"
#include "../tests/test_concepts.hpp"

template<class... Ts>
int test(Ts&&...);

REQUIREMENT Req1
{
    template<class T>
    auto REQUIRES(T t) -> decltype(concepts::detail::valid_expr(
    concepts::detail::valid_if<concepts::Same<decltype(t.type), int>>()
    ));
};

struct A { int type; };

int main()
{
    static_assert(std::is_same_v<decltype(test(std::declval<void>())), int>);
    static_assert(concepts::requires_<Req1, A>);
    std::cout << "All tests succeeded!\n";

    return 0;
}