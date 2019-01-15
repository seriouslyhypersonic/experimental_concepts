/*
 * Copyright Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <iostream>
#include <type_traits>

#include <conceptslib/type_traits.hpp>

#include "tests/test_type_traits.hpp"

int main()
{
    static_assert(std::is_same_v<traits::common_type<int, double, float>::type, double>);
    static_assert(std::is_same_v<traits::rref<int&>::type, int&&>);

    std::cout << "All tests succeeded!\n";

    return 0;
}