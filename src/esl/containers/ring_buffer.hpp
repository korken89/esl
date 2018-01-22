//          Copyright Emil Fresk 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <type_traits>
#include <cstring>
#include <array>
#include <utility>
#include <tuple>

#include "allocate.hpp"
#include "../helpers/error_functions.hpp"
#include "../helpers/feature_defs.hpp"

namespace esl
{
//
// ring_buffer definition
//
template < typename T, typename ErrFun = error_functions::noop >
class ring_buffer;

//
// is_ring_buffer helper
//
template < typename >
struct is_ring_buffer : std::false_type
{
};

template < typename T, typename ErrFun >
struct is_ring_buffer< ring_buffer< T, ErrFun > > : std::true_type
{
};

namespace details
{
//
// Checker for powers of 2
//
constexpr bool is_power_of_2(std::size_t x)
{
  return ((x != 0) && !(x & (x - 1)));
}
}

//
// allocate specialized trait to force ring_buffers to be power of 2
//
template < typename T, typename F, std::size_t Capacity >
struct allocate_capacity_check< ring_buffer< T, F >, Capacity >
    : std::integral_constant< bool, details::is_power_of_2(Capacity) >
{
  static_assert(details::is_power_of_2(Capacity),
                "ring_buffer only accepts capacity in powers of 2.");
};

template < typename T, typename ErrFun >
class ring_buffer
{
protected:
  T* buffer_;
  std::size_t head_idx_ = 0;
  std::size_t tail_idx_ = 0;
  std::size_t mask_ = 0;

  using CheckBounds = std::integral_constant<
      bool, !std::is_same< ErrFun, error_functions::noop >::value >;

  constexpr void increment_head() noexcept
  {
    head_idx_ = (head_idx_ + 1) & mask_;
  }

  constexpr void increment_tail() noexcept
  {
    tail_idx_ = (tail_idx_ + 1) & mask_;
  }

public:
  //
  // Standard type definitions
  //
  using size_type = std::size_t;
  using value_type = T;
  using reference = T&;

  //
  // Constructor
  //
  constexpr ring_buffer(T* buffer, size_type capacity) noexcept
      : buffer_{buffer}, mask_{capacity - 1}
  {
    if
      ESL_CONSTEXPR_IF(CheckBounds())
      {
        if (!details::is_power_of_2(capacity))
          ErrFun{}("construction with size not a power of 2");

        if (buffer == nullptr)
          ErrFun{}("construction with nullptr");
      }
  }

  //
  // Element access
  //
  constexpr const T& front() const noexcept(noexcept(ErrFun{}("")))
  {
    if
      ESL_CONSTEXPR_IF(CheckBounds())
    if (empty())
      ErrFun{}("front on empty buffer");

    return buffer_[tail_idx_];
  }

  constexpr T& front() noexcept(noexcept(ErrFun{}("")))
  {
    if
      ESL_CONSTEXPR_IF(CheckBounds())
    if (empty())
      ErrFun{}("front on empty buffer");

    return buffer_[tail_idx_];
  }

  constexpr const T& back() const noexcept(noexcept(ErrFun{}("")))
  {
    if
      ESL_CONSTEXPR_IF(CheckBounds())
    if (empty())
      ErrFun{}("back on empty buffer");

    return buffer_[head_idx_ - 1];
  }

  constexpr T& back() noexcept(noexcept(ErrFun{}("")))
  {
    if
      ESL_CONSTEXPR_IF(CheckBounds())
    if (empty())
      ErrFun{}("back on empty buffer");

    return buffer_[head_idx_ - 1];
  }

  //
  // Capacity
  //
  constexpr auto size() const noexcept
  {
    return (head_idx_ - tail_idx_ + mask_ + 1) & mask_;
  }

  constexpr auto capacity() const noexcept
  {
    return mask_;
  }

  constexpr auto free() const noexcept
  {
    return capacity() - size();
  }

  constexpr bool empty() const noexcept
  {
    return (head_idx_ == tail_idx_);
  }

  constexpr bool full() const noexcept
  {
    return (size() == capacity());
  }

  //
  // Modifiers
  //
  template < typename... Args >
  constexpr void emplace_back(Args&&... args) noexcept(noexcept(ErrFun{}("")))
  {
    if
      ESL_CONSTEXPR_IF(CheckBounds())
    if (full())
      ErrFun{}("emplace_back on full buffer");

    // Use placement new
    new (&buffer_[head_idx_]) T(std::forward< Args >(args)...);
    increment_head();
  }

  template <
      typename T1,
      typename = std::enable_if_t< std::is_convertible< T1, T >::value > >
  constexpr void push_back(T1&& val) noexcept(noexcept(ErrFun{}("")))
  {
    if
      ESL_CONSTEXPR_IF(CheckBounds())
    if (full())
      ErrFun{}("push_back on full buffer");

    buffer_[head_idx_] = std::forward< T1 >(val);
    increment_head();
  }

  constexpr void push_back(const T* ptr,
                           std::size_t n) noexcept(noexcept(ErrFun{}("")))
  {
    if
      ESL_CONSTEXPR_IF(CheckBounds())
    if (free() < n)
      ErrFun{}("push_back: array too large");

    const auto space_left_head = mask_ + 1 - head_idx_;

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

      std::memcpy(&buffer_[0], (ptr + space_left_head), n * sizeof(T));
      head_idx_ = n;
    }
  }

  template < std::size_t S >
  constexpr void push_back(const T (&buf)[S]) noexcept(noexcept(ErrFun{}("")))
  {
    push_back(buf, S);
  }

  constexpr void clear() noexcept
  {
    head_idx_ = 0;
    tail_idx_ = 0;
  }

  constexpr void pop() noexcept(noexcept(ErrFun{}("")))
  {
    if
      ESL_CONSTEXPR_IF(CheckBounds())
    if (empty())
      ErrFun{}("pop on empty buffer");

    increment_tail();
  }

  // constexpr std::pair<T*, std::size_t> read_chunk(std::size_t read_size)
  // {
  // }
};

}  // namespace esl
