//          Copyright Emil Fresk 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace esl
{
namespace error_functions
{

struct noop
{
  constexpr void operator()(const char* msg) const noexcept
  {
    (void)msg;
  }
};

struct halt
{
  void operator()(const char* msg) const noexcept
  {
    (void)msg;
    while (1);
  }
};

} // namespace error_functions
} // namespace esl
