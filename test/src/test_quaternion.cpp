//          Copyright Emil Fresk 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <esl/math/quaternion.hpp>
#include <array>

TEST(test_quaternion, test_make)
{
  esl::quaterniond q1(1, 2, 3, 0);

  ASSERT_EQ(1, q1[0]);
  ASSERT_EQ(2, q1[1]);
  ASSERT_EQ(3, q1[2]);
  ASSERT_EQ(0, q1[3]);

  esl::quaterniond q2{1, 2, 3, 0};

  ASSERT_EQ(1, q2[0]);
  ASSERT_EQ(2, q2[1]);
  ASSERT_EQ(3, q2[2]);
  ASSERT_EQ(0, q2[3]);
}

TEST(test_quaternion, test_access)
{
  esl::quaterniond q(1, 2, 3, 0);

  ASSERT_EQ(1, q[0]);
  ASSERT_EQ(2, q[1]);
  ASSERT_EQ(3, q[2]);
  ASSERT_EQ(0, q[3]);

  ASSERT_EQ(1, q.x());
  ASSERT_EQ(2, q.y());
  ASSERT_EQ(3, q.z());
  ASSERT_EQ(0, q.w());

  auto v = q.vec();

  ASSERT_EQ(1, v.x());
  ASSERT_EQ(2, v.y());
  ASSERT_EQ(3, v.z());
}

TEST(test_quaternion, test_access_and_modify)
{
  esl::quaterniond q(1, 2, 3, 0);

  q[0] = 6;
  q[1] = 7;
  q[2] = 8;
  q[3] = 9;

  ASSERT_EQ(6, q[0]);
  ASSERT_EQ(7, q[1]);
  ASSERT_EQ(8, q[2]);
  ASSERT_EQ(9, q[3]);

  ASSERT_EQ(6, q.x());
  ASSERT_EQ(7, q.y());
  ASSERT_EQ(8, q.z());
  ASSERT_EQ(9, q.w());

  q.x() = 9;
  q.y() = 10;
  q.z() = 11;
  q.w() = 12;

  ASSERT_EQ(9, q[0]);
  ASSERT_EQ(10, q[1]);
  ASSERT_EQ(11, q[2]);
  ASSERT_EQ(12, q[3]);

  ASSERT_EQ(9, q.x());
  ASSERT_EQ(10, q.y());
  ASSERT_EQ(11, q.z());
  ASSERT_EQ(12, q.w());
}

TEST(test_quaternion, test_operators)
{
  esl::quaterniond q(1, 2, 3, 10);
  auto q1 = q.conj();

  ASSERT_EQ(1, q[0]);
  ASSERT_EQ(2, q[1]);
  ASSERT_EQ(3, q[2]);
  ASSERT_EQ(10, q[3]);

  ASSERT_EQ(-1, q1[0]);
  ASSERT_EQ(-2, q1[1]);
  ASSERT_EQ(-3, q1[2]);
  ASSERT_EQ(10, q1[3]);
}


TEST(test_quaternion, test_vector_math)
{
  esl::quaterniond q1(1, 2, 3, 4);
  q1.normalize();
  esl::quaterniond q2(4, 5, 6, 7);
  q2.normalize();

  auto q3 = q1 * q2;
  auto q4 = q1 * q2.conj();

  ASSERT_DOUBLE_EQ(1.0, q3.norm());
  ASSERT_DOUBLE_EQ(1.0, q4.norm());

  ASSERT_DOUBLE_EQ(0.32530002431617777, q3[0]);
  ASSERT_DOUBLE_EQ(0.65060004863235532, q3[1]);
  ASSERT_DOUBLE_EQ(0.68313005106397318, q3[2]);
  ASSERT_DOUBLE_EQ(-0.065060004863235626, q3[3]);

  ASSERT_DOUBLE_EQ(-0.09759000729485337, q4[0]);
  ASSERT_DOUBLE_EQ(-0.19518001458970666, q4[1]);
  ASSERT_DOUBLE_EQ(0, q4[2]);
  ASSERT_DOUBLE_EQ(0.9759000729485332, q4[3]);
}

TEST(test_quaternion, test_vector_math_operators)
{
  esl::quaterniond q1(1, 2, 3, 4);
  q1.normalize();

  esl::vector3d v{1, 1, 1};

  auto rv1 = q1.rotate( v );
  // auto rv2 = q1 * v;
  // auto rv3 = v * q1;

  ASSERT_DOUBLE_EQ(v.norm(), rv1.norm());
  ASSERT_DOUBLE_EQ(v.norm(), rv2.norm());
  ASSERT_DOUBLE_EQ(v.norm(), rv3.norm());

  ASSERT_DOUBLE_EQ(0.19999999999999984, rv1[0]);
  ASSERT_DOUBLE_EQ(1.3999999999999999, rv1[1]);
  ASSERT_DOUBLE_EQ(0.99999999999999978, rv1[2]);

  // ASSERT_DOUBLE_EQ(0.19999999999999984, rv2[0]);
  // ASSERT_DOUBLE_EQ(1.3999999999999999, rv2[1]);
  // ASSERT_DOUBLE_EQ(0.99999999999999978, rv2[2]);

  // ASSERT_DOUBLE_EQ(0.19999999999999984, rv3[0]);
  // ASSERT_DOUBLE_EQ(1.3999999999999999, rv3[1]);
  // ASSERT_DOUBLE_EQ(0.99999999999999978, rv3[2]);
}

void testconst(const esl::quaterniond& v)
{
  (void)v.data();
  (void)v[0];
  (void)v.x();
  (void)v.y();
  (void)v.z();
  (void)v.w();
  (void)v.vec();
}

TEST(test_vector, test_const)
{
  esl::quaterniond q1(1, 2, 3, 4);
  testconst(q1);
}

int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
