//          Copyright Emil Fresk 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace esl
{
namespace details
{
// Singleton generator
template < class C >
struct singleton
{
  static C& instance()
  {
    static C inst;
    return inst;
  }
};

}  // namespace details

// Singleton helper
template < class C >
constexpr C& get_singleton()
{
  return details::singleton< C >::instance();
}

}  // namespace esl
