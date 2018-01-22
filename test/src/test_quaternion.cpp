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

  ASSERT_EQ(1, q1.w());
  ASSERT_EQ(2, q1.x());
  ASSERT_EQ(3, q1.y());
  ASSERT_EQ(0, q1.z());

  esl::quaterniond q2{1, 2, 3, 0};

  ASSERT_EQ(1, q2.w());
  ASSERT_EQ(2, q2.x());
  ASSERT_EQ(3, q2.y());
  ASSERT_EQ(0, q2.z());
}

TEST(test_quaternion, test_access)
{
  esl::quaterniond q(1, 2, 3, 0);

  ASSERT_EQ(1, q[3]);
  ASSERT_EQ(2, q[0]);
  ASSERT_EQ(3, q[1]);
  ASSERT_EQ(0, q[2]);

  ASSERT_EQ(1, q.w());
  ASSERT_EQ(2, q.x());
  ASSERT_EQ(3, q.y());
  ASSERT_EQ(0, q.z());

  auto v = q.vec();

  ASSERT_EQ(2, v.x());
  ASSERT_EQ(3, v.y());
  ASSERT_EQ(0, v.z());
}

TEST(test_quaternion, test_access_and_modify)
{
  esl::quaterniond q;

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

  ASSERT_EQ(-2, q1.x());
  ASSERT_EQ(-3, q1.y());
  ASSERT_EQ(-10, q1.z());
  ASSERT_EQ(1, q1.w());
}


TEST(test_quaternion, test_vector_math)
{
  esl::quaterniond q1(1, 2, 3, 4);
  q1.normalize();
  esl::quaterniond q2(4, 5, 6, 7);
  q2.normalize();

  ASSERT_DOUBLE_EQ(       0.18257418583505536, q1.w());
  ASSERT_DOUBLE_EQ(       0.36514837167011072, q1.x());
  ASSERT_DOUBLE_EQ(       0.547722557505166, q1.y());
  ASSERT_DOUBLE_EQ(       0.730296743340221, q1.z());


  ASSERT_DOUBLE_EQ(       0.356348322549899, q2.w());
  ASSERT_DOUBLE_EQ(       0.445435403187374, q2.x());
  ASSERT_DOUBLE_EQ(       0.534522483824849, q2.y());
  ASSERT_DOUBLE_EQ(       0.623609564462324, q2.z());

  auto q3 = q1 * q2;
  auto q4 = q1 * q2.conj();
  auto q5 = q1.conj() * q2;
  auto q6 = q1.conj() * q2.conj();

  ASSERT_DOUBLE_EQ(1.0, q3.norm());
  ASSERT_DOUBLE_EQ(1.0, q4.norm());
  ASSERT_DOUBLE_EQ(1.0, q5.norm());
  ASSERT_DOUBLE_EQ(1.0, q6.norm());


  ASSERT_DOUBLE_EQ(      -0.845780063222062, q3.w());
  ASSERT_DOUBLE_EQ(       0.16265001215808883, q3.x());
  ASSERT_DOUBLE_EQ(       0.39036002917941326, q3.y());
  ASSERT_DOUBLE_EQ(       0.32530002431617766, q3.z());

  ASSERT_DOUBLE_EQ(       0.975900072948533, q4.w());
  ASSERT_DOUBLE_EQ(      0.0975900072948533, q4.x());
  ASSERT_DOUBLE_EQ(   -5.5511151231257827e-17, q4.y());
  ASSERT_DOUBLE_EQ(       0.19518001458970666, q4.z());

  ASSERT_DOUBLE_EQ(       0.975900072948533, q5.w());
  ASSERT_DOUBLE_EQ(    0, q5.x());
  ASSERT_DOUBLE_EQ(      -0.19518001458970669, q5.y());
  ASSERT_DOUBLE_EQ(     -0.0975900072948533, q5.z());

  ASSERT_DOUBLE_EQ(      -0.845780063222062, q6.w());
  ASSERT_DOUBLE_EQ(      -0.260240019452942, q6.x());
  ASSERT_DOUBLE_EQ(      -0.19518001458970669, q6.y());
  ASSERT_DOUBLE_EQ(      -0.422890031611031, q6.z());
}

TEST(test_quaternion, test_vector_math_operators)
{
  esl::quaterniond q1(1, 2, 3, 4);
  q1.normalize();

  esl::vector3d v{1, 1, 1};

  auto rv1 = q1.rotate( v );

  ASSERT_DOUBLE_EQ(v.norm(), rv1.norm());

  ASSERT_DOUBLE_EQ(0.2, rv1.x());
  ASSERT_DOUBLE_EQ(1, rv1.y());
  ASSERT_DOUBLE_EQ(1.4, rv1.z());
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
