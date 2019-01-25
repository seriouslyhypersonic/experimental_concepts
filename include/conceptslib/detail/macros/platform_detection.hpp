/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef DETAIL_PLATFORM_DETECTION_H
#define DETAIL_PLATFORM_DETECTION_H

#include <ciso646> // Note: only gcc version 6.1 or newer define in ciso646

#ifdef __clang__
    #ifdef _LIBCPP_VERSION // Using LLVM libc++
        #define SUPPORTS_STD_INVOKE false
    #elif defined(__GLIBCXX__) // Using GNU libstdc++
        #define SUPPORTS_STD_INVOKE (__clang_major__ > 6)
    #endif
#elif defined(__GLIBCXX__ ) // Using GNU libstdc++
    #define SUPPORTS_STD_INVOKE true
#elif defined(_LIBCPP_VERSION) // Using LLVM libc++
    #define SUPPORTS_STD_INVOKE false???
#elif defined(_CPPLIB_VER) // Note: used by Visual Studio
    #define SUPPORTS_STD_INVOKE false???
#endif

#if SUPPORTS_STD_INVOKE
    #include <conceptslib/detail/concepts/invocable_default.hpp>
#else
    #include <conceptslib/detail/concepts/invocable_workaround.hpp>
#endif



#endif //DETAIL_PLATFORM_DETECTION_H
