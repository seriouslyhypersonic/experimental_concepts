/*
 * Copyright (c) Nuno Alves de Sousa 2019
 *
 * Use, modification and distribution is subject to the Boost Software License,
 * Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TESTING_H
#define TESTING_H

#include <gtest/gtest.h>

#define CONCEPT_ASSERT(...) static_assert(__VA_ARGS__, \
    "concept assertion failed due to " #__VA_ARGS__)

#endif //TESTING_H
