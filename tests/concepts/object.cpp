/*
 * Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * Acknowledgments: Copyright (c) Casey Carter 2015
 *                  - GitHub: https://github.com/caseycarter/cmcstl2
 */
#include <string>

#include <testing.hpp>

#include <conceptslib/concepts.hpp>

class ObjectConcepts: public ::testing::Test
{
protected:
    struct CopyableType { };

    struct MoveOnly
    {
        MoveOnly(MoveOnly&&) = default;
        MoveOnly& operator=(MoveOnly&&) = default;
    };

    struct CopyOnly
    {
        CopyOnly(const CopyOnly&) = default;
        CopyOnly& operator=(const CopyOnly&) = default;

        CopyOnly(CopyOnly&&) = delete;
        CopyOnly& operator=(CopyOnly&&) = delete;
    };

    struct NonMovable
    {
        NonMovable() = default;
        NonMovable(NonMovable&&) = delete;
    };

    struct ExplicitMove
    {
        ExplicitMove() = default;
        explicit ExplicitMove(ExplicitMove&&) = default;
    };

    struct ExplicitCopy
    {
        ExplicitCopy() = default;
        explicit ExplicitCopy(const ExplicitCopy&) = default;
    };

    struct SemiregularType { };

    struct RegularType
    {
        friend bool operator==(const RegularType&, const RegularType&);
        friend bool operator!=(const RegularType&, const RegularType&);
    };
};

/* --- Concept Movable --- */
TEST_F(ObjectConcepts, ConceptMovable)
{
    using concepts::Movable;

    CONCEPT_ASSERT(Movable<int>);
    CONCEPT_ASSERT(!Movable<const int>);
    CONCEPT_ASSERT(Movable<double>);
    CONCEPT_ASSERT(Movable<std::string>);
    CONCEPT_ASSERT(!Movable<void>);
    CONCEPT_ASSERT(!Movable<int&>);
    CONCEPT_ASSERT(Movable<int*>);

    CONCEPT_ASSERT(Movable<CopyableType>);
    CONCEPT_ASSERT(Movable<MoveOnly>);
    CONCEPT_ASSERT(!Movable<NonMovable>);
    CONCEPT_ASSERT(!Movable<CopyOnly>);
    CONCEPT_ASSERT(!Movable<ExplicitMove>);
    CONCEPT_ASSERT(!Movable<ExplicitCopy>);
}

/* --- Concept Copyable --- */
TEST_F(ObjectConcepts, ConceptCopyable)
{
    using concepts::Copyable;

    CONCEPT_ASSERT(Copyable<int>);
    CONCEPT_ASSERT(!Copyable<const int>);
    CONCEPT_ASSERT(Copyable<double>);
    CONCEPT_ASSERT(Copyable<std::string>);
    CONCEPT_ASSERT(!Copyable<void>);
    CONCEPT_ASSERT(!Copyable<int&>);
    CONCEPT_ASSERT(Copyable<int*>);

    CONCEPT_ASSERT(Copyable<CopyableType>);
    CONCEPT_ASSERT(!Copyable<MoveOnly>);
    CONCEPT_ASSERT(!Copyable<NonMovable>);
    CONCEPT_ASSERT(!Copyable<CopyOnly>);
    CONCEPT_ASSERT(!Copyable<ExplicitMove>);
    CONCEPT_ASSERT(!Copyable<ExplicitCopy>);
}

/* --- Concept Semiregular --- */
TEST_F(ObjectConcepts, ConceptSemiregular)
{
    using concepts::Semiregular;

    CONCEPT_ASSERT(Semiregular<int>);
    CONCEPT_ASSERT(!Semiregular<const int>);
    CONCEPT_ASSERT(Semiregular<int>);
    CONCEPT_ASSERT(Semiregular<double>);
    CONCEPT_ASSERT(Semiregular<std::string>);
    CONCEPT_ASSERT(!Semiregular<void>);
    CONCEPT_ASSERT(!Semiregular<int&>);
    CONCEPT_ASSERT(Semiregular<int*>);

    CONCEPT_ASSERT(Semiregular<SemiregularType>);
    CONCEPT_ASSERT(Semiregular<RegularType>);
    CONCEPT_ASSERT(Semiregular<CopyableType>);
    CONCEPT_ASSERT(!Semiregular<MoveOnly>);
    CONCEPT_ASSERT(!Semiregular<NonMovable>);
    CONCEPT_ASSERT(!Semiregular<CopyOnly>);
    CONCEPT_ASSERT(!Semiregular<ExplicitMove>);
    CONCEPT_ASSERT(!Semiregular<ExplicitCopy>);
}

/* --- Concept Regular --- */
TEST_F(ObjectConcepts, ConceptRegular)
{
    using concepts::Regular;

    CONCEPT_ASSERT(Regular<int>);
    CONCEPT_ASSERT(!Regular<const int>);
    CONCEPT_ASSERT(Regular<int>);
    CONCEPT_ASSERT(Regular<double>);
    CONCEPT_ASSERT(Regular<std::string>);
    CONCEPT_ASSERT(!Regular<void>);
    CONCEPT_ASSERT(!Regular<int&>);
    CONCEPT_ASSERT(Regular<int*>);

    CONCEPT_ASSERT(!Regular<SemiregularType>);
    CONCEPT_ASSERT(Regular<RegularType>);
    CONCEPT_ASSERT(!Regular<CopyableType>);
    CONCEPT_ASSERT(!Regular<MoveOnly>);
    CONCEPT_ASSERT(!Regular<NonMovable>);
    CONCEPT_ASSERT(!Regular<CopyOnly>);
    CONCEPT_ASSERT(!Regular<ExplicitMove>);
    CONCEPT_ASSERT(!Regular<ExplicitCopy>);
}
