cmake_minimum_required(VERSION 3.13)
project(experimental_concepts)

set(CMAKE_CXX_STANDARD 17)

add_executable(experimental_concepts
        main.cpp
        experimental_concepts.hpp
        non_std_numerics.hpp
        universal_references.hpp
        tests/test_types.hpp
        requires_expression.hpp
        experimental_type_traits.hpp
        common_reference.hpp
        tests/test_type_traits.hpp
        common_type.hpp)