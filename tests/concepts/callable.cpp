/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include <testing.hpp>
#include <string>
#include <functional>

#include <conceptslib/concepts.hpp>

/* --- Concept Invocable --- */
void increment(int& i) { ++i; }
int increment_by_clref(const int& i);

const auto lambda1 =
    [](const std::string& str){ return str + "Hello, World!"; };

const auto lambd2 = [](int&){ };

const auto genericLambda =
    [](const auto& lhs, const auto& rhs) { return lhs + rhs; };

const auto genericLambdaByRef = [](auto& ) { };

TEST(CallableConcepts, ConceptInvocable)
{
    using concepts::Invocable;

    struct Functor { void operator()(int&); };

    struct X
    {
        int intMember;
        void memberFunction(int&);
    };

    struct Z { };

    CONCEPT_ASSERT(!Invocable<void, void>);
    CONCEPT_ASSERT(Invocable<decltype(increment), int&>);
    CONCEPT_ASSERT(!Invocable<decltype(increment), int>);

    CONCEPT_ASSERT(Invocable<decltype(increment_by_clref), int>);
    CONCEPT_ASSERT(Invocable<decltype(increment_by_clref), int&>);
    CONCEPT_ASSERT(Invocable<decltype(increment_by_clref), const int&>);
    CONCEPT_ASSERT(Invocable<decltype(increment_by_clref), int&&>);

    CONCEPT_ASSERT(!Invocable<Functor, int>);
    CONCEPT_ASSERT(Invocable<Functor, int&>);
    CONCEPT_ASSERT(!Invocable<Functor, const int&>);
    CONCEPT_ASSERT(!Invocable<Functor, int&&>);

    CONCEPT_ASSERT(Invocable<int X::*, X>);
    CONCEPT_ASSERT(Invocable<decltype(&X::intMember), X>);
    CONCEPT_ASSERT(Invocable<decltype(&X::intMember), X*>);
    CONCEPT_ASSERT(!Invocable<decltype(&X::intMember), Z>);

    CONCEPT_ASSERT(Invocable<void(X::*)(int&), X, int&>);
    CONCEPT_ASSERT(!Invocable<void(X::*)(int&), X, int&&>);
    CONCEPT_ASSERT(Invocable<decltype(&X::memberFunction), X, int&>);
    CONCEPT_ASSERT(!Invocable<decltype(&X::memberFunction), X, int&&>);

    CONCEPT_ASSERT(Invocable<decltype(lambda1), std::string>);
    CONCEPT_ASSERT(Invocable<decltype(lambda1), std::string&>);
    CONCEPT_ASSERT(Invocable<decltype(lambda1), const std::string&>);
    CONCEPT_ASSERT(Invocable<decltype(lambda1), std::string&&>);

    CONCEPT_ASSERT(Invocable<decltype(lambd2), int&>);
    CONCEPT_ASSERT(!Invocable<decltype(lambd2), int>);
    CONCEPT_ASSERT(!Invocable<decltype(lambd2), const int&>);

    CONCEPT_ASSERT(Invocable<decltype(genericLambda), int, int>);
    CONCEPT_ASSERT(Invocable<decltype(genericLambda),
        std::string, std::string>);

    // Hard error
#if false
    CONCEPT_ASSERT(!Invocable<decltype(genericLambda), std::string, int>);
#endif

    CONCEPT_ASSERT(!Invocable<decltype(genericLambdaByRef), int>);
    CONCEPT_ASSERT(Invocable<decltype(genericLambdaByRef), int&>);
}

/* --- Concept Predicate --- */
bool pred(int, const std::string&);
bool oddPred(int, std::string&);
std::string notPred(int, int);
const auto lambdaPred = [](int, int){ return true; };

TEST(CallableConcepts, ConceptPredicate)
{
    using concepts::Predicate;

    struct PredicateFunctor { bool operator()(int, int); };
    struct X
    {
        bool predicate(int, int);
        std::string notPredicate();
    };

    CONCEPT_ASSERT(!Predicate<void, void>);

    CONCEPT_ASSERT(Predicate<PredicateFunctor, int, int>);

    CONCEPT_ASSERT(Predicate<bool(X::*)(int, int), X, int, int>);
    CONCEPT_ASSERT(Predicate<decltype(&X::predicate), X, int, int>);

    CONCEPT_ASSERT(concepts::Invocable<decltype(&X::notPredicate), X>);
    CONCEPT_ASSERT(!Predicate<decltype(&X::notPredicate), X>);

    CONCEPT_ASSERT(Predicate<decltype(pred), int, std::string>);
    CONCEPT_ASSERT(Predicate<decltype(pred), int, std::string&>);
    CONCEPT_ASSERT(Predicate<decltype(pred), int, const std::string&>);
    CONCEPT_ASSERT(Predicate<decltype(pred), int, std::string&&>);

    CONCEPT_ASSERT(Predicate<decltype(oddPred), int, std::string&>);
    CONCEPT_ASSERT(!Predicate<decltype(oddPred), int, std::string>);
    CONCEPT_ASSERT(!Predicate<decltype(oddPred), int, const std::string&>);

    CONCEPT_ASSERT(!Predicate<decltype(notPred), int, int>);

    CONCEPT_ASSERT(!Predicate<decltype(lambda1), std::string>);

    CONCEPT_ASSERT(Predicate<decltype(lambdaPred), int, int>);

    CONCEPT_ASSERT(Predicate<decltype(genericLambda), int, int>);
    CONCEPT_ASSERT(!Predicate<decltype(genericLambda),
        std::string, std::string>);
}

/* --- Concept Relation --- */
TEST(CallableConcepts, ConceptRelation)
{
    using concepts::Relation;

    CONCEPT_ASSERT(!Relation<void, void, void>);

    CONCEPT_ASSERT(Relation<std::equal_to<int>, int, int>);

    CONCEPT_ASSERT(Relation<decltype(genericLambda), int, int>);
}
