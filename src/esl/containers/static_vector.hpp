//          Copyright Emil Fresk 2017.
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

namespace esl
{

template < typename T, std::size_t N, bool CheckBounds = false,
           typename ErrFun = error_functions::noop >
class static_vector
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
  // Constructor / Destructor
  //
  constexpr static_vector() noexcept
  {
  }

  ~static_vector() noexcept
  {
    clear(); // Calls all destructors
  }

  //
  // Element access
  //
  constexpr T &operator[](std::size_t idx) noexcept
  {
    if (CheckBounds)
      if (idx >= size())
        ErrFun{}("operator[] out of bounds");

    return *reinterpret_cast< T * >(&buffer_[idx]);
  }

  constexpr const T &operator[](std::size_t idx) const noexcept
  {
    if (CheckBounds)
      if (idx >= size())
        ErrFun{}("operator[] out of bounds");

    return *reinterpret_cast< const T * >(&buffer_[idx]);
  }

  constexpr const T &front() const noexcept
  {
    if (CheckBounds)
      if (empty())
        ErrFun{}("front on empty vector");

    return *reinterpret_cast< const T * >(&buffer_[0]);
  }

  constexpr T &front() noexcept
  {
    if (CheckBounds)
      if (empty())
        ErrFun{}("front on empty vector");

    return *reinterpret_cast< T * >(&buffer_[0]);
  }

  constexpr const T &back() const noexcept
  {
    if (CheckBounds)
      if (empty())
        ErrFun{}("back on empty vector");

    return *reinterpret_cast< const T * >(&buffer_[curr_idx_ - 1]);
  }

  constexpr T &back() noexcept
  {
    if (CheckBounds)
      if (empty())
        ErrFun{}("back on empty vector");

    return *reinterpret_cast< T * >(&buffer_[curr_idx_ - 1]);
  }

  constexpr auto data() noexcept
  {
    return reinterpret_cast< T * >(&buffer_[0]);
  }

  constexpr auto data() const noexcept
  {
    return reinterpret_cast< const T * >(&buffer_[0]);
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

  constexpr std::size_t free() const noexcept
  {
    return max_size() - size();
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
      if (full())
        ErrFun{}("emplace_back on full vector");

    // Use placement new
    new (&buffer_[curr_idx_]) T(std::forward< Args >(args)...);
    ++curr_idx_;
  }

  constexpr void push_back(T &&obj) noexcept
  {
    if (CheckBounds)
      if (full())
        ErrFun{}("push_back on full vector");

    *reinterpret_cast< T * >(&buffer_[curr_idx_]) = std::forward< T >(obj);
    ++curr_idx_;
  }

  constexpr void push_back(T const &obj) noexcept
  {
    if (CheckBounds)
      if (full())
        ErrFun{}("push_back on full vector");

    *reinterpret_cast< T * >(&buffer_[curr_idx_]) = obj;
    ++curr_idx_;
  }

  constexpr void push_back(const T *ptr, std::size_t n) noexcept
  {
    if (CheckBounds)
      if (free() < n)
        ErrFun{}("push_back: array too large");

    std::memcpy(&buffer_[curr_idx_], ptr, n * sizeof(T));
    curr_idx_ += n;
  }

  template < std::size_t S >
  constexpr void push_back(const T (&buf)[S]) noexcept
  {
    push_back(buf, S);
  }

  template < bool B, typename F, typename T2, std::size_t M >
  constexpr void push_back(const static_vector< T2, M, B, F > &v) noexcept
  {
    push_back(v.cbegin(), v.size());
  }

  constexpr void clear() noexcept
  {
    for (auto idx = 0U; idx < curr_idx_; ++idx)
      reinterpret_cast< T * >(&buffer_[idx])->~T();

    curr_idx_ = 0;
  }

  constexpr void pop_back() noexcept
  {
    if (CheckBounds)
      if (empty())
        ErrFun{}("pop_back on empty vector");

    reinterpret_cast< T * >(&buffer_[curr_idx_])->~T();
    --curr_idx_;
  }
};

}  // namespace esl
