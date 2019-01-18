/*
 * Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * Acknowledgments: Eric Niebler test cases
 *                  - GitHub: https://github.com/ericniebler/range-v3
 */
#ifndef TEST_CONCEPTS_H
#define TEST_CONCEPTS_H

#include <type_traits>

#include <conceptslib/concepts.hpp>

namespace test_concepts
{

struct MoveOnly
{
    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(MoveOnly&&) = default;
};

struct A { };
using AlsoA = A;
struct B { };

static_assert(concepts::Same<A, A>);
static_assert(concepts::Same<A, AlsoA>);
static_assert(!concepts::Same<A, B>);

} // namespace test_concepts

#endif //TEST_CONCEPTS_H
