/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef DETAIL_PLATFORM_DETECTION_H
#define DETAIL_PLATFORM_DETECTION_H

#ifdef __clang__
    #ifdef __APPLE__
        #define SUPPORTS_STD_INVOKE false
    #else
#define SUPPORTS_STD_INVOKE (__clang_major__ > 6)
    #endif
#else
    #define SUPPORTS_STD_INVOKE true
#endif

#if SUPPORTS_STD_INVOKE
    #include <conceptslib/detail/concepts/invocable_default.hpp>
#else
    #include <conceptslib/detail/concepts/invocable_workaround.hpp>
#endif



#endif //DETAIL_PLATFORM_DETECTION_H
