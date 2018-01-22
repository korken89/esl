//          Copyright Emil Fresk 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#if !defined(__cpp_constexpr) || (__cpp_constexpr < 201603)
#define ESL_CONSTEXPR_LAMBDA_AVAILABLE
#endif

#if !defined(__cpp_if_constexpr) || (__cpp_if_constexpr < 201606)
#define ESL_CONSTEXPR_IF
#else
#define ESL_CONSTEXPR_IF constexpr
#endif
