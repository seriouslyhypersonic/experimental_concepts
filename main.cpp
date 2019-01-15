/*
 * Copyright Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#include <iostream>

#include "common_reference.hpp"
#include "common_type.hpp"
#include "tests/test_type_traits.hpp"

int r32() { return 32; }

template<class T, class U>
struct Container { virtual const char* operator()() { return "general\n"; } };

template<class T, class U>
struct Container<T&, U> : Container<T, U>
{
    const char* operator()() override { return "lreference\n"; }
    using type = T;
};

template<class T, class U>
struct Container<T&&, U>: Container<T&, U> {
    const char* operator()() override { return "rreference\n"; }
    using type = U;
};

int main()
{
    std::cout << Container<int, void>()();
    std::cout << Container<int&, void>()();
    static_assert(std::is_same_v<Container<int&, void>::type, int>);

    std::cout << Container<int&&, void>()();
    static_assert(std::is_same_v<Container<int&&, void>::type, void>);

    static_assert(std::is_same_v<std::common_type_t<const int, volatile int>, int>);

    std::cout << "All tests succeeded!\n";

    return 0;
}