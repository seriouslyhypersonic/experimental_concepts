cmake_minimum_required(VERSION 3.13)

project(experimental_concepts
        VERSION 0.1
        DESCRIPTION "C++17 experimental implementation of C++20 concepts"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Create an INTERFACE library
add_library(conceptslib INTERFACE)

# Specify target_sources: show headers in the IDE
target_sources(conceptslib INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/type_traits/common_reference.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/type_traits/common_type.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/type_traits/type_traits.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/concepts/concepts.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/concepts/core.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/type_traits.hpp)

target_include_directories(conceptslib INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include)


add_executable(concepts_test
        main.cpp
        tests/test_type_traits.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/type_traits/common_reference.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/type_traits/common_type.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/type_traits/type_traits.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/concepts/concepts.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/concepts/core.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/type_traits.hpp
        )

target_link_libraries(concepts_test conceptslib)