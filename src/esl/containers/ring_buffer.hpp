//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <type_traits>
#include <cstring>

#include "../helpers/error_functions.hpp"

namespace esl
{

template < bool CheckBounds, typename ErrFun, typename T, std::size_t N >
class ring_buffer_impl
{
protected:
  std::aligned_storage_t< sizeof(T), alignof(T) > buffer_[N];
  std::size_t head_idx_ = 0;
  std::size_t tail_idx_ = 0;

  constexpr void increment_head() noexcept
  {
    head_idx_ = (head_idx_ + 1) % N;
  }

  constexpr void increment_tail() noexcept
  {
    tail_idx_ = (tail_idx_ + 1) % N;
  }

public:
  //
  // Standard type definitions
  //
  using base_type  = std::array< T, N >;
  using size_type  = typename base_type::size_type;
  using value_type = typename base_type::value_type;
  using reference  = typename base_type::reference;

  //
  // Constructor
  //
  constexpr ring_buffer_impl() noexcept
  {
  }

  //
  // Element access
  //
  constexpr T front() const noexcept
  {
    if (CheckBounds)
      if (tail_idx_ == head_idx_)
        ErrFun{}("front on empty buffer");

    return *reinterpret_cast< T* >(&buffer_[tail_idx_]);
  }

  constexpr T& front() noexcept
  {
    if (CheckBounds)
      if (tail_idx_ == head_idx_)
        ErrFun{}("front on empty buffer");

    return *reinterpret_cast< T* >(&buffer_[tail_idx_]);
  }

  constexpr T back() const noexcept
  {
    if (CheckBounds)
      if (tail_idx_ == head_idx_)
        ErrFun{}("back on empty buffer");

    return *reinterpret_cast< T* >(&buffer_[head_idx_ - 1]);
  }

  constexpr T& back() noexcept
  {
    if (CheckBounds)
      if (tail_idx_ == head_idx_)
        ErrFun{}("back on empty buffer");

    return *reinterpret_cast< T* >(&buffer_[head_idx_ - 1]);
  }

  //
  // Capacity
  //
  constexpr std::size_t size() const noexcept
  {
    return (head_idx_ - tail_idx_ + N) % N;
  }

  constexpr std::size_t max_size() const noexcept
  {
    return N - 1;
  }

  constexpr std::size_t free() const noexcept
  {
    return max_size() - size();
  }

  constexpr bool empty() const noexcept
  {
    return (head_idx_ == tail_idx_);
  }

  constexpr bool full() const noexcept
  {
    return (size() == max_size());
  }

  //
  // Modifiers
  //
  template < typename... Args >
  constexpr void emplace_back(Args&&... args) noexcept
  {
    if (CheckBounds)
      if (full())
        ErrFun{}("emplace_back on full buffer");

    // Use placement new
    new (&buffer_[head_idx_]) T(std::forward< Args >(args)...);
    increment_head();
  }

  constexpr void push_back(T&& obj) noexcept
  {
    if (CheckBounds)
      if (full())
        ErrFun{}("push_back on full buffer");

    *reinterpret_cast< T* >(&buffer_[head_idx_]) = std::forward< T >(obj);
    increment_head();
  }

  constexpr void push_back(T const& obj) noexcept
  {
    if (CheckBounds)
      if (size() == max_size())
        ErrFun{}("push_back on full buffer");

    *reinterpret_cast< T* >(&buffer_[head_idx_]) = obj;
    increment_head();
  }

  constexpr void push_back(const T* ptr, std::size_t n) noexcept
  {
    if (CheckBounds)
      if (max_size() - size() < n)
        ErrFun{}("push_back: array too large");

    const auto space_left_head = N - head_idx_;

    if (space_left_head >= n)
    {
      // All will fit without the head_idx_ overflowing
      std::memcpy(&buffer_[head_idx_], ptr, n * sizeof(T));
      head_idx_ += n;
    }
    else
    {
      // The head_idx_ will overflow, write in 2 steps
      std::memcpy(&buffer_[head_idx_], ptr, space_left_head * sizeof(T));

      n -= space_left_head;

      std::memcpy(&buffer_[0], ptr, n * sizeof(T));
      head_idx_ = n;
    }
  }

  template < std::size_t S >
  constexpr void push_back(const T (&buf)[S]) noexcept
  {
    push_back(buf, S);
  }

  constexpr void clear() noexcept
  {
    head_idx_ = 0;
    tail_idx_ = 0;
  }

  constexpr void pop()
  {
    if (CheckBounds)
      if (tail_idx_ == head_idx_)
        ErrFun{}("pop on empty buffer");

    increment_tail();
  }
};

template < typename T, std::size_t N >
using ring_buffer = ring_buffer_impl< false, error_functions::noop, T, N >;

template < typename T, std::size_t N >
using ring_buffer_debug = ring_buffer_impl< true, error_functions::halt, T, N >;

}  // namespace esl
