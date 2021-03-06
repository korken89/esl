//          Copyright Emil Fresk 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "vector.hpp"

namespace esl
{
//
// Quaternion implementation is based on the passive Hamilton notation.
// See "Quaternion kinematics for the error-state KF" by Joan Solá for a good
// reference on Hamilton notation and quaternion integration. Or, for a more
// complete reference on quaternions, see "Quaternions and Rotation Sequences"
// by Jack B. Kupier (book).
//
// Note: Internal storage is [x, y, z, w]
//
template < typename T >
class quaternion : public vector< T, 4 >
{
public:
  constexpr quaternion() : vector< T, 4 >(0, 0, 0, 1)
  {
  }

  constexpr quaternion(T w, T x, T y, T z) : vector< T, 4 >(x, y, z, w)
  {
  }

  constexpr T& x()
  {
    return vector< T, 4 >::x();
  }

  constexpr const T& x() const
  {
    return vector< T, 4 >::x();
  }

  constexpr T& y()
  {
    return vector< T, 4 >::y();
  }

  constexpr const T& y() const
  {
    return vector< T, 4 >::y();
  }

  constexpr T& z()
  {
    return vector< T, 4 >::z();
  }

  constexpr const T& z() const
  {
    return vector< T, 4 >::z();
  }

  constexpr T& w()
  {
    return vector< T, 4 >::storage_[3];
  }

  constexpr const T& w() const
  {
    return vector< T, 4 >::storage_[3];
  }

  constexpr vector< T, 3 >& vec()
  {
    return *reinterpret_cast< vector< T, 3 >* >(this);
  }

  constexpr const vector< T, 3 >& vec() const
  {
    return *reinterpret_cast< const vector< T, 3 >* >(this);
  }

  constexpr quaternion conj() const
  {
    return quaternion(w(), -x(), -y(), -z());
  }

  constexpr vector< T, 3 > rotate(vector< T, 3 > v) const
  {
    // Rotation from the rotation matrix equations directly applied on v.
    const T qwsq = w() * w();
    const T qxsq = x() * x();
    const T qysq = y() * y();
    const T qzsq = z() * z();

    const T qxy = x() * y();
    const T qxz = x() * z();
    const T qxw = x() * w();
    const T qyz = y() * z();
    const T qyw = y() * w();
    const T qzw = z() * w();

    return {(qwsq + qxsq - qysq - qzsq) * v.x() + (T(2) * (qxy - qzw)) * v.y() +
                (T(2) * (qxz + qyw)) * v.z(),
            (T(2) * (qxy + qzw)) * v.x() + (qwsq - qxsq + qysq - qzsq) * v.y() +
                (T(2) * (qyz - qxw)) * v.z(),
            (T(2) * (qxz - qyw)) * v.x() + (T(2) * (qyz + qxw)) * v.y() +
                (qwsq - qxsq - qysq + qzsq) * v.z()};
  }

  constexpr quaternion& operator*=(const quaternion& rhs) noexcept
  {
    quaternion p{*this};

    x() = p.w() * rhs.x() + p.x() * rhs.w() + p.y() * rhs.z() - p.z() * rhs.y();
    y() = p.w() * rhs.y() - p.x() * rhs.z() + p.y() * rhs.w() + p.z() * rhs.x();
    z() = p.w() * rhs.z() + p.x() * rhs.y() - p.y() * rhs.x() + p.z() * rhs.w();
    w() = p.w() * rhs.w() - p.x() * rhs.x() - p.y() * rhs.y() - p.z() * rhs.z();

    return *this;
  }

  constexpr friend quaternion operator*(quaternion lhs,
                                        const quaternion& rhs) noexcept
  {
    lhs *= rhs;
    return lhs;
  }

  // Quaternion times vector is assumed to be a rotation
  // constexpr friend vector< T, 3 > operator*(const quaternion& lhs,
  //                                           const vector< T, 3 >& rhs)
  //                                           noexcept
  // {
  //   return lhs.rotate(rhs);
  // }

  // Quaternion times vector is assumed to be a rotation
  // constexpr friend vector< T, 3 > operator*(const vector< T, 3 >& lhs,
  //                                           const quaternion& rhs) noexcept
  // {
  //   return rhs.rotate(lhs);
  // }
};

//
// Common definitions
//
using quaternionf = quaternion< float >;
using quaterniond = quaternion< double >;
}
