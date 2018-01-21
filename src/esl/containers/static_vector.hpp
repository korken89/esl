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

#include "../helpers/error_functions.hpp"
#include "../helpers/feature_defs.hpp"

namespace esl
{
template < typename T, typename ErrFun = error_functions::noop >
class static_vector;

template < typename >
struct is_static_vector : std::false_type
{
};

template < typename T, typename ErrFun >
struct is_static_vector< static_vector< T, ErrFun > > : std::true_type
{
};

template < typename T, typename ErrFun >
class static_vector
{
public:
  //
  // Standard type definitions
  //
  using size_type      = std::size_t;
  using value_type     = T;
  using reference      = T &;
  using iterator       = T *;
  using const_iterator = const T *;

protected:
  T *buffer_;
  size_type curr_idx_ = 0;
  size_type capacity_ = 0;

  using CheckBounds = std::integral_constant<
      bool, !std::is_same< ErrFun, error_functions::noop >::value >;

public:
  //
  // Constructor / Destructor
  //
  constexpr static_vector(T *buffer, size_type capacity) noexcept
      : buffer_{buffer},
        capacity_{capacity}
  {
  }

  ~static_vector() noexcept
  {
    for (auto idx = 0U; idx < curr_idx_; ++idx)
      (&buffer_[idx])->~T();
  }

  //
  // Element access
  //
  constexpr T &operator[](std::size_t idx) noexcept(noexcept(ErrFun{}("")))
  {
    if ESL_CONSTEXPR_IF (CheckBounds())
      if (idx >= size())
        ErrFun{}("operator[] out of bounds");

    return buffer_[idx];
  }

  constexpr const T &operator[](std::size_t idx) const
      noexcept(noexcept(ErrFun{}("")))
  {
    if ESL_CONSTEXPR_IF (CheckBounds())
      if (idx >= size())
        ErrFun{}("operator[] out of bounds");

    return buffer_[idx];
  }

  constexpr const T &front() const noexcept(noexcept(ErrFun{}("")))
  {
    if ESL_CONSTEXPR_IF (CheckBounds())
      if (empty())
        ErrFun{}("front on empty vector");

    return buffer_[0];
  }

  constexpr T &front() noexcept(noexcept(ErrFun{}("")))
  {
    if ESL_CONSTEXPR_IF (CheckBounds())
      if (empty())
        ErrFun{}("front on empty vector");

    return buffer_[0];
  }

  constexpr const T &back() const noexcept(noexcept(ErrFun{}("")))
  {
    if ESL_CONSTEXPR_IF (CheckBounds())
      if (empty())
        ErrFun{}("back on empty vector");

    return buffer_[curr_idx_ - 1];
  }

  constexpr T &back() noexcept(noexcept(ErrFun{}("")))
  {
    if ESL_CONSTEXPR_IF (CheckBounds())
      if (empty())
        ErrFun{}("back on empty vector");

    return buffer_[curr_idx_ - 1];
  }

  constexpr T *data() noexcept
  {
    return buffer_;
  }

  constexpr const T *data() const noexcept
  {
    return buffer_;
  }

  //
  // Capacity
  //
  constexpr size_type size() const noexcept
  {
    return curr_idx_;
  }

  constexpr size_type capacity() const noexcept
  {
    return capacity_;
  }

  constexpr size_type free() const noexcept
  {
    return capacity() - size();
  }

  constexpr bool empty() const noexcept
  {
    return (curr_idx_ == 0);
  }

  constexpr bool full() const noexcept
  {
    return (curr_idx_ >= capacity_);
  }

  //
  // Iterators
  //
  constexpr T *begin() noexcept
  {
    return &buffer_[0];
  }

  constexpr T *end() noexcept
  {
    return &buffer_[curr_idx_];
  }

  constexpr const T *cbegin() const noexcept
  {
    return &buffer_[0];
  }

  constexpr const T *cend() const noexcept
  {
    return &buffer_[curr_idx_];
  }

  //
  // Modifiers
  //
  template < typename... Args >
  constexpr void emplace_back(Args &&... args) noexcept(noexcept(ErrFun{}("")))
  {
    if ESL_CONSTEXPR_IF (CheckBounds())
      if (full())
        ErrFun{}("emplace_back on full vector");

    // Use placement new
    new (&buffer_[curr_idx_]) T(std::forward< Args >(args)...);
    ++curr_idx_;
  }

  template < typename T1, typename = std::enable_if_t<
                              std::is_convertible< T1, T >::value > >
  constexpr void push_back(T1 &&val) noexcept(noexcept(ErrFun{}("")))
  {
    if ESL_CONSTEXPR_IF (CheckBounds())
      if (full())
        ErrFun{}("push_back on full vector");

    buffer_[curr_idx_] = std::forward< T1 >(val);
    ++curr_idx_;
  }

  constexpr void push_back(const T *ptr,
                           size_type n) noexcept(noexcept(ErrFun{}("")))
  {
    if ESL_CONSTEXPR_IF (CheckBounds())
      if (free() < n)
        ErrFun{}("push_back: array too large");

    std::memcpy(&buffer_[curr_idx_], ptr, n * sizeof(T));
    curr_idx_ += n;
  }

  template < std::size_t S >
  constexpr void push_back(const T (&buf)[S]) noexcept(noexcept(ErrFun{}("")))
  {
    push_back(buf, S);
  }

  template < typename F, typename T2 >
  constexpr void push_back(const static_vector< T2, F > &v) noexcept(
      noexcept(ErrFun{}("")))
  {
    push_back(v.cbegin(), v.size());
  }

  constexpr void clear() noexcept
  {
    for (auto idx = 0U; idx < curr_idx_; ++idx)
      (&buffer_[idx])->~T();

    curr_idx_ = 0;
  }

  constexpr void pop_back() noexcept(noexcept(ErrFun{}("")))
  {
    if ESL_CONSTEXPR_IF (CheckBounds())
      if (empty())
        ErrFun{}("pop_back on empty vector");

    --curr_idx_;
    (&buffer_[curr_idx_])->~T();
  }

  constexpr auto erase(T *begin, T *end)
  {
    if ESL_CONSTEXPR_IF (CheckBounds())
    {
      if (begin < this->begin() || end > this->end())
        ErrFun{}("erase out of bounds");

      if (begin > end)
        ErrFun{}("erase: begin > end");
    }

    // Save end pointer
    auto vend = this->end();

    if (begin == end || empty())
      return vend;

    const auto dist = std::distance(begin, end);

    // Call destructors
    auto curr = begin;

    for (; curr != end; ++curr)
      curr->~T();

    // Move elements
    auto next = end;

    while (next != vend)
    {
      *begin = *next;
      ++begin;
      ++next;
    }

    // Update count
    curr_idx_ -= dist;

    return end;
  }

  constexpr auto erase(T *element)
  {
    return erase(element, element + 1);
  }
};

}  // namespace esl
