/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#include <type_traits>
#include <bitset>
#include <vector>
#include <memory>
#include <cstddef>

#include <testing.hpp>

#include <conceptslib/concepts.hpp>
#include <conceptslib/type_traits.hpp>

class ComparisonConcepts: public ::testing::Test
{
public:
    struct ImplicitlyConvertibleToBool { operator bool() const; };
    struct ExplicitlyConvertibleToBool { explicit operator bool() const; };

    struct HasComparisonOperators
    {
        friend bool operator==(const HasComparisonOperators&,
                               const HasComparisonOperators&);
        friend bool operator!=(const HasComparisonOperators&,
                               const HasComparisonOperators&);
    };

    struct HasOddComparisonOperators
    {
        friend bool operator==(HasOddComparisonOperators&,
                               HasOddComparisonOperators&);
        friend bool operator!=(HasOddComparisonOperators&,
                               HasOddComparisonOperators&);
    };

    struct ComparableToInt{ operator int() const; };

    struct OddComparisonOperators
    {
        friend bool operator==(OddComparisonOperators& , int);
        friend bool operator==(int, OddComparisonOperators&);

        friend bool operator!=(OddComparisonOperators& , int);
        friend bool operator!=(int, OddComparisonOperators&);
    };

    /* --- EqualityComparableWith --- */
    // Both types must have an EqualityComparable common_reference

    struct X {
        // Must convert to the common_reference
        operator HasComparisonOperators() const;

        // Has to be WeaklyEqualityComparable
        friend bool operator==(const X&, const X&);
        friend bool operator!=(const X&, const X&);
    };

    struct ComparableToX
    {
        // Must convert to the common_reference
        operator HasComparisonOperators() const;

        // Has to be WeaklyEqualityComparable
        friend bool operator==(const ComparableToX&,
                               const ComparableToX&);
        friend bool operator!=(const ComparableToX&,
                               const ComparableToX&);

        // Provide the WeaklyComparibleWith requirements for the two types
        friend bool operator==(const ComparableToX& , const X&);
        friend bool operator==(const X&, const ComparableToX&);

        friend bool operator!=(const ComparableToX& , const X&);
        friend bool operator!=(const X&, const ComparableToX&);
    };

    /* --- StrictTotallyOrderedWith --- */
    struct HasComparisonOperotorsOrdered: HasComparisonOperators
    {
        friend bool operator>(const HasComparisonOperotorsOrdered&,
                              const HasComparisonOperotorsOrdered&);
        friend bool operator>=(const HasComparisonOperotorsOrdered&,
                               const HasComparisonOperotorsOrdered&);
        friend bool operator<(const HasComparisonOperotorsOrdered&,
                              const HasComparisonOperotorsOrdered&);
        friend bool operator<=(const HasComparisonOperotorsOrdered&,
                               const HasComparisonOperotorsOrdered&);
    };

    struct XOrdered: X
    {
        // Must convert to HasComparisonOperotorsOrdered
        operator HasComparisonOperotorsOrdered() const;

        // Has to be StrictTotallyOrdered
        friend bool operator<(const XOrdered&, const XOrdered&);
        friend bool operator<=(const XOrdered&, const XOrdered&);
        friend bool operator>(const XOrdered&, const XOrdered&);
        friend bool operator>=(const XOrdered&, const XOrdered&);
    };

    struct ComparableToXOrdered: ComparableToX
    {
        // Must convert to HasComparisonOperotorsOrdered
        operator HasComparisonOperotorsOrdered() const;

        // Has to be StrictTotallyOrdered
        friend bool operator<(const ComparableToXOrdered&,
                              const ComparableToXOrdered&);
        friend bool operator<=(const ComparableToXOrdered&,
                               const ComparableToXOrdered&);
        friend bool operator>(const ComparableToXOrdered&,
                              const ComparableToXOrdered&);
        friend bool operator>=(const ComparableToXOrdered&,
                               const ComparableToXOrdered&);

        // Provide inequality operators for the two types
        friend bool operator<(const ComparableToXOrdered&, const XOrdered&);
        friend bool operator<=(const ComparableToXOrdered&, const XOrdered&);
        friend bool operator>(const ComparableToXOrdered&, const XOrdered&);
        friend bool operator>=(const ComparableToXOrdered&, const XOrdered&);

        friend bool operator<(const XOrdered&, const ComparableToXOrdered&);
        friend bool operator<=(const XOrdered&, const ComparableToXOrdered&);
        friend bool operator>(const XOrdered&, const ComparableToXOrdered&);
        friend bool operator>=(const XOrdered&, const ComparableToXOrdered&);
    };
};

namespace traits
{
    using X = ComparisonConcepts::X;
    using XComparisonOperators = ComparisonConcepts::ComparableToX;
    using HasComparisonOperators = ComparisonConcepts::HasComparisonOperators;

    // HasComparisonOperators is an EqualityComparable common_reference

    template<template<class> class TQual,
    template<class> class UQual>
    struct basic_common_reference<X, XComparisonOperators, TQual, UQual>
    {
        using type = HasComparisonOperators;
    };
    // Must specialize both A NotA and NotA A
    template<template<class> class TQual,
    template<class> class UQual>
    struct basic_common_reference<XComparisonOperators, X, UQual, TQual>
    {
        using type = HasComparisonOperators;
    };

    // HasComparisonOperatorsOrdered is a StrictTotallyOrdered common_reference

    using XOrdered = ComparisonConcepts::XOrdered;
    using ComparibleToXOrdered = ComparisonConcepts::ComparableToXOrdered;
    using HasComparisonOperotorsOrdered =
        ComparisonConcepts::HasComparisonOperotorsOrdered;

    template<template<class> class TQual,
    template<class> class UQual>
    struct basic_common_reference<XOrdered, ComparibleToXOrdered, TQual, UQual>
    {
        using type = HasComparisonOperotorsOrdered;
    };
    // Must specialize both A NotA and NotA A
    template<template<class> class TQual,
    template<class> class UQual>
    struct basic_common_reference<ComparibleToXOrdered, XOrdered, UQual, TQual>
    {
        using type = HasComparisonOperotorsOrdered;
    };
}

/* --- Concept Boolean --- */
TEST_F(ComparisonConcepts, ConceptBoolean)
{
    using concepts::Boolean;

    CONCEPT_ASSERT(Boolean<bool>);
    CONCEPT_ASSERT(Boolean<int>);
    CONCEPT_ASSERT(Boolean<std::true_type>);
    CONCEPT_ASSERT(Boolean<std::false_type>);
    CONCEPT_ASSERT(Boolean<std::bitset<32>::reference>);
    CONCEPT_ASSERT(Boolean<std::vector<bool>::reference>);
    CONCEPT_ASSERT(Boolean<ImplicitlyConvertibleToBool>);

    CONCEPT_ASSERT(!Boolean<void>);
    CONCEPT_ASSERT(!Boolean<int*>);
    CONCEPT_ASSERT(!Boolean<std::unique_ptr<int>>);
    CONCEPT_ASSERT(!Boolean<ExplicitlyConvertibleToBool>);
}

/* --- Concept EqualityComparable --- */
TEST_F(ComparisonConcepts, ConceptEqualityComparable)
{
    using concepts::EqualityComparable;

    CONCEPT_ASSERT(EqualityComparable<int>);
    CONCEPT_ASSERT(EqualityComparable<int&>);
    CONCEPT_ASSERT(!EqualityComparable<void>);
    CONCEPT_ASSERT(EqualityComparable<std::string>);
    CONCEPT_ASSERT(EqualityComparable<std::nullptr_t>);

    CONCEPT_ASSERT(EqualityComparable<HasComparisonOperators>);
    CONCEPT_ASSERT(!EqualityComparable<HasOddComparisonOperators>);
}

/* --- Concept EqualityComparableWith --- */
TEST_F(ComparisonConcepts, ConceptEqualityComparableWith)
{
    using concepts::EqualityComparableWith;

    CONCEPT_ASSERT(EqualityComparableWith<int, int>);
    CONCEPT_ASSERT(EqualityComparableWith<const int&, int>);
    CONCEPT_ASSERT(EqualityComparableWith<double, double>);
    CONCEPT_ASSERT(EqualityComparableWith<bool, bool>);
    CONCEPT_ASSERT(EqualityComparableWith<std::string, std::string>);
    CONCEPT_ASSERT(EqualityComparableWith<std::string, const char*>);

    CONCEPT_ASSERT(EqualityComparableWith<ImplicitlyConvertibleToBool
                                         ,ImplicitlyConvertibleToBool>);
    CONCEPT_ASSERT(!EqualityComparableWith<ExplicitlyConvertibleToBool
                                          ,ExplicitlyConvertibleToBool>);

    CONCEPT_ASSERT(EqualityComparableWith<X, ComparableToX>);
    CONCEPT_ASSERT(EqualityComparableWith<ComparableToX, X>);

    CONCEPT_ASSERT(!EqualityComparableWith<OddComparisonOperators, int>);
    CONCEPT_ASSERT(EqualityComparableWith<ComparableToInt, int>);

    CONCEPT_ASSERT(!EqualityComparableWith<int, void>);
}

/* --- Concept StrictTotallyOrdered --- */
TEST_F(ComparisonConcepts, ConceptStrictTotallyOrdered)
{
    using concepts::StrictTotallyOrdered;

    CONCEPT_ASSERT(StrictTotallyOrdered<int>);
    CONCEPT_ASSERT(StrictTotallyOrdered<const int&>);
    CONCEPT_ASSERT(StrictTotallyOrdered<double>);
    CONCEPT_ASSERT(!StrictTotallyOrdered<void>);
    CONCEPT_ASSERT(StrictTotallyOrdered<std::string>);

    CONCEPT_ASSERT(StrictTotallyOrdered<ComparableToInt>);
    CONCEPT_ASSERT(StrictTotallyOrdered<XOrdered>);
    CONCEPT_ASSERT(StrictTotallyOrdered<ComparableToXOrdered>);
    CONCEPT_ASSERT(StrictTotallyOrdered<HasComparisonOperotorsOrdered>);
}

/* --- Concept StrictTotallyOrderedWith --- */
TEST_F(ComparisonConcepts, ConceptStrictTotallyOrderedWith)
{
    using concepts::StrictTotallyOrderedWith;

    CONCEPT_ASSERT(StrictTotallyOrderedWith<int, int>);
    CONCEPT_ASSERT(StrictTotallyOrderedWith<const int&, const int&>);
    CONCEPT_ASSERT(StrictTotallyOrderedWith<double, double>);
    CONCEPT_ASSERT(StrictTotallyOrderedWith<int, double>);
    CONCEPT_ASSERT(StrictTotallyOrderedWith<std::string, std::string>);
    CONCEPT_ASSERT(StrictTotallyOrderedWith<std::string, const char*>);
    CONCEPT_ASSERT(!StrictTotallyOrderedWith<void, void>);

    CONCEPT_ASSERT(StrictTotallyOrderedWith<ComparableToInt, int>);

    CONCEPT_ASSERT(StrictTotallyOrderedWith<XOrdered, ComparableToXOrdered>);
    CONCEPT_ASSERT(StrictTotallyOrderedWith<ComparableToXOrdered, XOrdered>);

    CONCEPT_ASSERT(!StrictTotallyOrderedWith<X, ComparableToX>);
}
