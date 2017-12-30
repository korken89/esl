//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>
#include <cmath>
#include <array>
#include <type_traits>
#include <esl/helpers/utils.hpp>

namespace esl
{
template < typename T, std::size_t N >
class vector
{
private:
  T storage_[N];

public:
  static_assert(N > 1, "Size must be larger than 1");

  //
  // Constructors
  //
  constexpr vector() noexcept
  {
    // repeat works like loop-unrolling
    esl::repeat< N >([&](auto i) {
      storage_[i] = T(0);  // op
    });
  }

  template < typename... Ts,
             typename = std::enable_if_t< details::all_true<
                 std::is_convertible< Ts, T >::value... >::value > >
  constexpr vector(Ts&&... vals) noexcept
      : storage_{static_cast< T >(std::forward< Ts >(vals))...}
  {
    static_assert(sizeof...(Ts) == N,
                  "Number of arguments does not match the size of the vector");
  }

  // Same or different sized vector
  template < std::size_t M >
  constexpr vector(vector< T, M > v) noexcept
  {
    static_assert(M <= N, "Size too big");

    esl::repeat< M >([&](auto i) {
      storage_[i] = v[i];  // op
    });

    esl::repeat< N - M >([&](auto i) {
      storage_[M + i] = T(0);  // op
    });
  }

  // Same or different sized array
  template < std::size_t M >
  constexpr vector(const std::array< T, M >& arr) noexcept
  {
    static_assert(M <= N, "Size too big");

    esl::repeat< M >([&](auto i) {
      storage_[i] = arr[i];  // op
    });

    esl::repeat< N - M >([&](auto i) {
      storage_[M + i] = T(0);  // op
    });
  }

  // Same or different sized raw array
  template < std::size_t M >
  constexpr vector(T (&arr)[M]) noexcept
  {
    static_assert(M <= N, "Size too big");

    esl::repeat< M >([&](auto i) {
      storage_[i] = arr[i];  // op
    });

    esl::repeat< N - M >([&](auto i) {
      storage_[M + i] = T(0);  // op
    });
  }

  //
  // Standard math functions
  //
  constexpr vector dot(const vector& rhs) const noexcept
  {
    vector v;

    esl::repeat< N >([&](auto i) {
      v.storage_[i] = this->storage_[i] * rhs.storage_[i];  // op
    });

    return v;
  }

  template < std::size_t M >
  constexpr vector< T, 3 > cross(const vector< T, M >& rhs) const noexcept
  {
    static_assert((M == 3) && (N == 3),
                  "Cross product only makes sense if both vectors are size 3");

    return {this->storage_[1] * rhs[2] - this->storage_[2] * rhs[1],
            this->storage_[2] * rhs[0] - this->storage_[0] * rhs[2],
            this->storage_[0] * rhs[1] - this->storage_[1] * rhs[0]};
  }

  constexpr T sum() const noexcept
  {
    auto s = T(0);

    esl::repeat< N >([&](auto i) {
      s += this->storage_[i];  // op
    });

    return s;
  }

  constexpr T norm_squared() const noexcept
  {
    auto s = T(0);

    esl::repeat< N >([&](auto i) {
      s += this->storage_[i] * this->storage_[i];  // op
    });

    return s;
  }

  constexpr T norm() const noexcept
  {
    return std::sqrt(norm_squared());
  }

  constexpr void normalize() noexcept
  {
    const auto nrm = norm();

    if (nrm != T(0))
    {
      const auto norm_inv = T(1) / nrm;
      *this *= norm_inv;
    }
  }

  constexpr vector square() const noexcept
  {
    vector v;

    esl::repeat< N >([&](auto i) {
      v.storage_[i] = this->storage_[i] * this->storage_[i];  // op
    });

    return v;
  }

  constexpr vector sqrt() const noexcept
  {
    vector v;

    esl::repeat< N >([&](auto i) {
      v.storage_[i] = std::sqrt(this->storage_[i]);  // op
    });

    return v;
  }

  constexpr vector abs() const noexcept
  {
    vector v;

    esl::repeat< N >([&](auto i) {
      v.storage_[i] = std::abs(this->storage_[i]);  // op
    });

    return v;
  }

  //
  // Access
  //
  constexpr T& operator[](int idx) noexcept
  {
    return storage_[idx];
  }

  constexpr const T& operator[](int idx) const noexcept
  {
    return storage_[idx];
  }

  constexpr T* data() noexcept
  {
    return storage_;
  }

  constexpr const T* data() const noexcept
  {
    return storage_;
  }

  constexpr T& x() noexcept
  {
    return storage_[0];
  }

  constexpr const T& x() const noexcept
  {
    return storage_[0];
  }

  constexpr T& y() noexcept
  {
    return storage_[1];
  }

  constexpr const T& y() const noexcept
  {
    return storage_[1];
  }

  constexpr T& z() noexcept
  {
    return storage_[2];
  }

  constexpr const T& z() const noexcept
  {
    return storage_[2];
  }

  //
  // Operators
  //
  template < std::size_t M >
  constexpr vector& operator=(const std::array< T, M >& arr) noexcept
  {
    static_assert(M <= N, "Size too big");

    esl::repeat< M >([&](auto i) {
      storage_[i] = arr[i];  // op
    });

    esl::repeat< N - M >([&](auto i) {
      storage_[M + i] = T(0);  // op
    });

    return *this;
  }

  constexpr vector& operator+=(const vector& rhs) noexcept
  {
    esl::repeat< N >([&](auto i) {
      storage_[i] += rhs.storage_[i];  // op
    });

    return *this;
  }

  constexpr vector& operator-=(const vector& rhs) noexcept
  {
    esl::repeat< N >([&](auto i) {
      storage_[i] -= rhs.storage_[i];  // op
    });

    return *this;
  }

  constexpr vector& operator*=(const T& rhs) noexcept
  {
    esl::repeat< N >([&](auto i) {
      storage_[i] *= rhs;  // op
    });

    return *this;
  }

  constexpr vector& operator/=(const T& rhs) noexcept
  {
    esl::repeat< N >([&](auto i) {
      storage_[i] /= rhs;  // op
    });

    return *this;
  }

  constexpr friend vector operator+(vector lhs, const vector& rhs) noexcept
  {
    lhs += rhs;
    return lhs;
  }

  constexpr friend vector operator-(vector lhs, const vector& rhs) noexcept
  {
    lhs -= rhs;
    return lhs;
  }

  constexpr friend vector operator-(vector rhs) noexcept
  {
    esl::repeat< N >([&](auto i) {
      rhs.storage_[i] = -rhs.storage_[i];  // op
    });

    return rhs;
  }

  constexpr friend vector operator*(vector lhs, const T& rhs) noexcept
  {
    lhs *= rhs;
    return lhs;
  }

  constexpr friend vector operator*(const T& lhs, vector rhs) noexcept
  {
    rhs *= lhs;
    return rhs;
  }

  constexpr friend vector operator/(vector lhs, const T& rhs) noexcept
  {
    lhs /= rhs;
    return lhs;
  }
};

//
// Common definitions
//
using vector2i = vector< int32_t, 2 >;
using vector2u = vector< uint32_t, 2 >;
using vector2f = vector< float, 2 >;
using vector2d = vector< double, 2 >;

using vector3i = vector< int32_t, 3 >;
using vector3u = vector< uint32_t, 3 >;
using vector3f = vector< float, 3 >;
using vector3d = vector< double, 3 >;

using vector4i = vector< int32_t, 4 >;
using vector4u = vector< uint32_t, 4 >;
using vector4f = vector< float, 4 >;
using vector4d = vector< double, 4 >;

using vector5i = vector< int32_t, 5 >;
using vector5u = vector< uint32_t, 5 >;
using vector5f = vector< float, 5 >;
using vector5d = vector< double, 5 >;

using vector6i = vector< int32_t, 6 >;
using vector6u = vector< uint32_t, 6 >;
using vector6f = vector< float, 6 >;
using vector6d = vector< double, 6 >;

}  // end namespace esl
