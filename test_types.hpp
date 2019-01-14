#ifndef TEST_TYPES_H
#define TEST_TYPES_H

// Same
struct TypeA
{
    TypeA() = default;
};

using AlsoTypeA = TypeA;
struct NotTypeA {};

// DerivedFrom
struct Base {};
struct DerivedPublicly: Base {};
struct DerivedProtectedly: protected Base{};
struct DerivedPrivately: private Base{};

// ConvertibleTo
struct TypeImplicitelyConvertibleToA
{
    operator TypeA() { return {}; }
};

struct TypeOnlyExplicitelyConvertibleToA
{
    explicit operator TypeA() { return {}; }
};

// TEST
struct AddableNoValueType
{
    AddableNoValueType& operator++() { return *this; }
};

struct AddableValueTypeBool
{
    AddableValueTypeBool() = delete;
    int x;
    using value_type = bool;
    AddableValueTypeBool& operator++() { return *this; }
};

// CONVERTIBLE TO WITH REQUIRES
//template<class From, class To>
//EXP_CONCEPT ConvertibleTo =
//            reqexpr<From(&)(), To>
//            ([]( auto&& f, auto&&  ) -> decltype( static_cast<To>(f()) )
//             {});

#endif //TEST_TYPES_H
