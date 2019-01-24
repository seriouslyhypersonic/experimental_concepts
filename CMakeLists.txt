cmake_minimum_required(VERSION 3.13)

project(experimental_concepts
        VERSION 0.1
        DESCRIPTION "C++17 experimental implementation of C++20 concepts"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic-errors")

# Create an INTERFACE library
add_library(conceptslib INTERFACE)

# Specify target_sources: show headers in the IDE
target_sources(conceptslib INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/type_traits/common_reference.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/type_traits/common_type.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/type_traits/type_traits.hpp

        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/concepts/concepts.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/concepts/core.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/concepts/comparison.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/concepts/movable.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/concepts/object.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/concepts/callable.hpp

        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/functional/invoke.hpp

        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/type_traits.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/concepts.hpp)

target_include_directories(conceptslib INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add tests
add_subdirectory(tests)

#include_directories(include)