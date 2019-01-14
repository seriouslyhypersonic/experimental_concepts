//
// Created by nuno on 11-01-2019.
//

#ifndef UNIVERSAL_REFERENCES_H
#define UNIVERSAL_REFERENCES_H

#include <iostream>
#include <vector>
#include <type_traits>

#include "non_std_numerics.hpp"

template<class T>
void onlyLvalues(T& param) // type of expression (T&) is lvalue => expression is an lvalue
{
    std::cout << __PRETTY_FUNCTION__ << '\n';
}

template<class T>
void f(T&& param) // Type deduction AND T&& => lvalue ref
{
    std::cout << __PRETTY_FUNCTION__ << '\n';
    if constexpr (std::is_const_v<std::remove_reference_t<T>>) {
        std::cout << "not incrementing " << param << '\n';
    } else {
        std::cout << "incrementing " << param << " to ";
        ++param;
        std::cout << param << '\n';
    }
}

template<class T>
void g(std::vector<T>&& param) // Type deduction BUT not T&& => rvalue ref
{
    std::cout << __PRETTY_FUNCTION__ << '\n';
}

template<class T>
void printType(T t)
{
    std::cout << __PRETTY_FUNCTION__ << '\n';
}

int r42() { return 42; }

void test()
{
    int i = 1;
    const int c1 = 1;

    /* lvalue references */

//    int& r1 = 1; // lvalue refs cannot bind to rvalues
    int& lri = i;
    const int& clr1 = 1; // const lvalue refs can bind to rvalues

    /* rvalue references */

    int&& rrInt = 2;
//    int&& rri = i; // rvalue refs cannot bind to lvalues
    rrInt = 1;
    const int&& crr1 = 1;
//    crr1 = 2; // const rvalue ref!

    /* lvalues can be queried for their address */

    std::cout << "&i = " << &i << '\n';
    std::cout << "&c1 = " << &c1 << '\n';
    std::cout << "&lri =" << &lri << '\n';
    std::cout << "&clr1 =" << &clr1 << '\n';
    std::cout << "&rrInt =" << &rrInt << '\n';

    /* universal references:
     * Where type deduction takes place: auto&& and template instantiation
     * */

    auto&& ur_rrInt = rrInt; // rrInt is lvalue
    std::cout << std::boolalpha;
    std::cout << "ur_rrInt is l_value ref: " << std::is_lvalue_reference_v<decltype(ur_rrInt)> << '\n';

    auto&& ur_int = 32; // 32 is rvalue
    std::cout << "ur_int is rvalue ref: " << std::is_rvalue_reference_v<decltype(ur_int)> << '\n';

    /* Type deduction */

    std::cout << "\nrvalue argument:\n";
    f(32);
    f(r42());

    std::cout << "\nlvalue argument:\n";

    f(i);
    f(c1);
    std::cout << "Reference striping for lvalues that are references\n";
    f(clr1);
    f(rrInt);
    f(crr1);

    std::cout << "i = " << i << '\n';
    std::cout << "c1 = " << c1 << '\n';
    std::cout << "clr1 = " << clr1 << '\n';
    std::cout << "rrInt ="  << rrInt << '\n';
    std::cout << "crr1 = " << crr1<< '\n';

    /* No type deduction */

    std::cout << "\nA reference appearing in an expression won't keep reference type \n";
    printType(1);
    printType<>(i);
    printType<>(lri);
    printType<>(rrInt);

    std::cout << '\n';
    g(std::vector<int>{1, 2, 3});
    g<int>({1, 2, 3});

    std::vector<int> lvv;
//    g(lvv); // error: lvv is lvalue

    std::cout << "\nf(T&) when the lvalue is a reference\n";
//    onlyLvalues(1);
    onlyLvalues(i);
    onlyLvalues(lri);
    onlyLvalues(rrInt);

    Double&& d32 = Double{32};
    std::cout << d32 << " is rvalue ref: " << std::is_rvalue_reference_v<decltype(d32)> << '\n';

    auto&& lvr32 = d32;
    std::cout << "lvr32 lvalue ref: " << std::is_lvalue_reference_v<decltype(lvr32)> << '\n';
    d32 = 33;
    std::cout << "lvr32 = " << lvr32 << '\n';
    lvr32 = 32;

    Double v32 = std::move(d32);
    std::cout << "d32 = " << d32 << '\n';
    std::cout << "v32 = " << v32 << '\n';
    std::cout << "v32 reference: " << std::is_reference_v<decltype(v32)> << '\n';

    lvr32 = 1;
    std::cout << "d32 = " << d32 << '\n';
    std::cout << "lvr32 = " << lvr32 << '\n';
    std::cout << "v32 = " << v32 << '\n';

}

#endif //UNIVERSAL_REFERENCES_H
