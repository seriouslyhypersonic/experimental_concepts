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
        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/type_traits.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/concepts.hpp)

target_include_directories(conceptslib INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

# --- Google Test with CMake: http://www.kaizou.org/2014/11/gtest-cmake/ --- #

# We need thread support
find_package(Threads REQUIRED)

# Enable ExternalProject CMake module
include(ExternalProject)

# Download and install GoogleTest
ExternalProject_Add(
        gtest
        URL https://github.com/google/googletest/archive/master.zip
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
        # Disable install step
        INSTALL_COMMAND "")

# Get GTest source and binary directories from CMake project
ExternalProject_Get_Property(gtest source_dir binary_dir)

# Create a libgtest target to be used as a dependency by test programs
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)

# Set libgtest properties
set_target_properties(libgtest PROPERTIES
        "IMPORTED_LOCATION" "${binary_dir}/googlemock/gtest/libgtest.a"
        "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
        )

# Create a libgmock target to be used as a dependency by test programs
add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gtest)

# Set libgmock properties
set_target_properties(libgmock PROPERTIES
        "IMPORTED_LOCATION" "${binary_dir}/googlemock/libgmock.a"
        "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
        )

# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories("${source_dir}/googletest/include"
        "${source_dir}/googlemock/include")

add_executable(run_tests
        tests/test_mail.cpp
        tests/test_type_traits.hpp
        tests/test_concepts.hpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/type_traits/common_reference.hpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/type_traits/common_type.hpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/type_traits/type_traits.hpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/concepts/concepts.hpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/detail/concepts/core.hpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/include/conceptslib/type_traits.hpp
        )

target_link_libraries(run_tests
        conceptslib
        libgtest
        libgmock)

install(TARGETS run_tests DESTINATION bin)