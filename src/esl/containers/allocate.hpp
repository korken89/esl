//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <type_traits>
#include <utility>

namespace esl
{
template < typename, std::size_t >
struct allocate_capacity_check : std::true_type
{
};

//
// Allocator for creating aligned buffers
//
template < typename Container, std::size_t Capacity >
class allocate : public Container
{
  // Check the capacity check
  static_assert(
      allocate_capacity_check< Container, Capacity >::value,
      "The capacity does not follow the requirement of the container.");

  using T = typename Container::value_type;
  std::aligned_storage_t< sizeof(T), alignof(T) > buffer_[Capacity];

public:
  template < typename... Ts >
  constexpr allocate(Ts&&... args) noexcept
      : Container(reinterpret_cast< T* >(buffer_), Capacity,
                  std::forward< Ts >(args)...)
  {
  }
};

template < typename >
struct is_allocate : std::false_type
{
};

template < typename T, std::size_t Capacity >
struct is_allocate< allocate< T, Capacity > > : std::true_type
{
};
}
