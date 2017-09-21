//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <type_traits>
#include <array>

#include "../helpers/error_functions.hpp"

namespace esl
{

template < bool CheckBounds, typename ErrFun, typename T, std::size_t N >
class static_vector_impl
{
protected:
  std::aligned_storage_t< sizeof(T), alignof(T) > buffer_[N];
  std::size_t curr_idx_ = 0;

public:
  //
  // Standard type definitions
  //
  using base_type      = std::array< T, N >;
  using size_type      = typename base_type::size_type;
  using value_type     = typename base_type::value_type;
  using reference      = typename base_type::reference;
  using iterator       = typename base_type::iterator;
  using const_iterator = typename base_type::const_iterator;

  //
  // Constructor
  //
  constexpr static_vector_impl() noexcept
  {
  }

  //
  // Element access
  //
  constexpr T &operator[](std::size_t idx) noexcept
  {
    if (CheckBounds)
      if (idx >= curr_idx_)
        ErrFun{}("operator[] out of bounds");

    return *reinterpret_cast< T * >(&buffer_[idx]);
  }

  constexpr T operator[](std::size_t idx) const noexcept
  {
    if (CheckBounds)
      if (idx >= curr_idx_)
        ErrFun{}("operator[] out of bounds");

    return *reinterpret_cast< T * >(&buffer_[idx]);
  }

  constexpr T front() const noexcept
  {
    if (CheckBounds)
      if (curr_idx_ == 0)
        ErrFun{}("front on zero sized");

    return *reinterpret_cast< T * >(&buffer_[0]);
  }

  constexpr T &front() noexcept
  {
    if (CheckBounds)
      if (curr_idx_ == 0)
        ErrFun{}("front on zero sized");

    return *reinterpret_cast< T * >(&buffer_[0]);
  }

  constexpr T back() const noexcept
  {
    if (CheckBounds)
      if (curr_idx_ == 0)
        ErrFun{}("back on zero sized");

    return *reinterpret_cast< T * >(&buffer_[curr_idx_ - 1]);
  }

  constexpr T &back() noexcept
  {
    if (CheckBounds)
      if (curr_idx_ == 0)
        ErrFun{}("back on zero sized");

    return *reinterpret_cast< T * >(&buffer_[curr_idx_ - 1]);
  }

  constexpr auto data() const noexcept
  {
    return reinterpret_cast< T * >(&buffer_[0]);
  }

  //
  // Capacity
  //
  constexpr std::size_t size() const noexcept
  {
    return curr_idx_;
  }

  constexpr std::size_t max_size() const noexcept
  {
    return N;
  }

  constexpr bool empty() const noexcept
  {
    return (curr_idx_ == 0);
  }

  constexpr bool full() const noexcept
  {
    return (curr_idx_ >= N);
  }

  //
  // Iterators
  //
  constexpr auto begin() noexcept
  {
    return reinterpret_cast< T * >(&buffer_[0]);
  }

  constexpr auto end() noexcept
  {
    return reinterpret_cast< T * >(&buffer_[curr_idx_]);
  }

  constexpr auto cbegin() const noexcept
  {
    return reinterpret_cast< const T * >(&buffer_[0]);
  }

  constexpr auto cend() const noexcept
  {
    return reinterpret_cast< const T * >(&buffer_[curr_idx_]);
  }

  //
  // Modifiers
  //
  template < typename... Args >
  constexpr void emplace_back(Args &&... args) noexcept
  {
    if (CheckBounds)
      if (curr_idx_ >= N)
        ErrFun{}("emplace_back out of bounds");

    // Use placement new
    new (&buffer_[curr_idx_]) T(std::forward< Args >(args)...);
    ++curr_idx_;
  }

  constexpr void push_back(T &&obj) noexcept
  {
    if (CheckBounds)
      if (curr_idx_ >= N)
        ErrFun{}("push_back out of bounds");

    *reinterpret_cast< T * >(&buffer_[curr_idx_]) = std::forward< T >(obj);
    ++curr_idx_;
  }

  constexpr void push_back(T const &obj) noexcept
  {
    if (CheckBounds)
      if (curr_idx_ >= N)
        ErrFun{}("push_back out of bounds");

    *reinterpret_cast< T * >(&buffer_[curr_idx_]) = obj;
    ++curr_idx_;
  }

  constexpr void clear() noexcept
  {
    curr_idx_ = 0;
  }

  constexpr void pop_back()
  {
    if (CheckBounds)
      if (curr_idx_ == 0)
        ErrFun{}("pop_back on zero size");

    --curr_idx_;
  }
};

template < typename T, std::size_t N >
using static_vector = static_vector_impl< false, error_functions::noop, T, N >;

template < typename T, std::size_t N >
using static_vector_debug =
    static_vector_impl< true, error_functions::halt, T, N >;

}  // namespace esl
