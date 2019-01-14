#include <iostream>

#include "experimental_concepts.hpp"
#include "requires_expression.hpp"
#include "test_types.hpp"

#include "universal_references.hpp"

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

    // Same
    static_assert(concepts::Same<TypeA, AlsoTypeA>);
    static_assert(concepts::Same<TypeA*, AlsoTypeA*>);
    static_assert(concepts::Same<TypeA&, AlsoTypeA&>);
    static_assert(concepts::Same<TypeA&&, AlsoTypeA&&>);
    static_assert(concepts::Same<void , void>);
    static_assert(!concepts::Same<TypeA, NotTypeA>);
    static_assert(!concepts::Same<TypeA, TypeA*>);
    static_assert(!concepts::Same<TypeA, TypeA&>);
    static_assert(!concepts::Same<TypeA, TypeA&&>);

    // DerivedFrom
    static_assert(concepts::DerivedFrom<Base, Base>);
    static_assert(concepts::DerivedFrom<DerivedPublicly, Base>);
    static_assert(concepts::DerivedFrom<DerivedPublicly, DerivedPublicly>);
    static_assert(!concepts::DerivedFrom<DerivedProtectedly, Base>);
    static_assert(!concepts::DerivedFrom<DerivedPrivately, Base>);

    // ConvertibleTo
    static_assert(concepts::ConvertibleTo<TypeImplicitelyConvertibleToA, TypeA>);
    static_assert(!concepts::ConvertibleTo<TypeOnlyExplicitelyConvertibleToA, TypeA>);

    // CommonReference
    std::cout << std::is_lvalue_reference_v<std::add_lvalue_reference_t<int&&>>;

    std::cout << "All tests succeeded!\n";

    return 0;
}