//          Copyright Emil Fresk 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <esl/math/vector.hpp>
#include <array>

TEST(test_vector, test_make)
{
  esl::vector3i vec1(1, 2, 3);

  ASSERT_EQ(1, vec1[0]);
  ASSERT_EQ(2, vec1[1]);
  ASSERT_EQ(3, vec1[2]);

  esl::vector3i vec2{1, 2, 3};

  ASSERT_EQ(1, vec2[0]);
  ASSERT_EQ(2, vec2[1]);
  ASSERT_EQ(3, vec2[2]);
}

TEST(test_vector, test_make_from_raw_array)
{
  int a[] = {1, 2, 3};
  esl::vector3i vec3(a);

  ASSERT_EQ(1, vec3[0]);
  ASSERT_EQ(2, vec3[1]);
  ASSERT_EQ(3, vec3[2]);

  esl::vector3i vec4{a};

  ASSERT_EQ(1, vec4[0]);
  ASSERT_EQ(2, vec4[1]);
  ASSERT_EQ(3, vec4[2]);

  esl::vector3i vec5 = a;

  ASSERT_EQ(1, vec5[0]);
  ASSERT_EQ(2, vec5[1]);
  ASSERT_EQ(3, vec5[2]);
}

TEST(test_vector, test_make_from_smaller_raw_array)
{
  int a[] = {1, 2};
  esl::vector3i vec3(a);

  ASSERT_EQ(1, vec3[0]);
  ASSERT_EQ(2, vec3[1]);
  ASSERT_EQ(0, vec3[2]);

  esl::vector3i vec4{a};

  ASSERT_EQ(1, vec4[0]);
  ASSERT_EQ(2, vec4[1]);
  ASSERT_EQ(0, vec4[2]);

  esl::vector3i vec5 = a;

  ASSERT_EQ(1, vec5[0]);
  ASSERT_EQ(2, vec5[1]);
  ASSERT_EQ(0, vec5[2]);
}

TEST(test_vector, test_make_from_array)
{
  std::array< int, 3 > a{{1, 2, 3}};
  esl::vector3i vec3(a);

  ASSERT_EQ(1, vec3[0]);
  ASSERT_EQ(2, vec3[1]);
  ASSERT_EQ(3, vec3[2]);

  esl::vector3i vec4{a};

  ASSERT_EQ(1, vec4[0]);
  ASSERT_EQ(2, vec4[1]);
  ASSERT_EQ(3, vec4[2]);

  esl::vector3i vec5 = a;

  ASSERT_EQ(1, vec5[0]);
  ASSERT_EQ(2, vec5[1]);
  ASSERT_EQ(3, vec5[2]);
}

TEST(test_vector, test_make_from_smaller_array)
{
  std::array< int, 2 > a{{1, 2}};
  esl::vector3i vec3(a);

  ASSERT_EQ(1, vec3[0]);
  ASSERT_EQ(2, vec3[1]);
  ASSERT_EQ(0, vec3[2]);

  esl::vector3i vec4{a};

  ASSERT_EQ(1, vec4[0]);
  ASSERT_EQ(2, vec4[1]);
  ASSERT_EQ(0, vec4[2]);

  esl::vector3i vec5 = a;

  ASSERT_EQ(1, vec5[0]);
  ASSERT_EQ(2, vec5[1]);
  ASSERT_EQ(0, vec5[2]);
}

TEST(test_vector, test_make_from_vector)
{
  esl::vector3i vec1{1, 2, 3};

  esl::vector3i vec2{vec1};
  esl::vector3i vec3 = vec1;

  ASSERT_EQ(1, vec2[0]);
  ASSERT_EQ(2, vec2[1]);
  ASSERT_EQ(3, vec2[2]);

  ASSERT_EQ(1, vec3[0]);
  ASSERT_EQ(2, vec3[1]);
  ASSERT_EQ(3, vec3[2]);
}

TEST(test_vector, test_make_from_smaller_vector)
{
  esl::vector2i vec1{1, 2};

  esl::vector3i vec2{vec1};
  esl::vector3i vec3 = vec1;

  ASSERT_EQ(1, vec2[0]);
  ASSERT_EQ(2, vec2[1]);
  ASSERT_EQ(0, vec2[2]);

  ASSERT_EQ(1, vec3[0]);
  ASSERT_EQ(2, vec3[1]);
  ASSERT_EQ(0, vec3[2]);
}

TEST(test_vector, test_access)
{
  esl::vector3i vec{1, 2, 3};

  ASSERT_EQ(1, vec[0]);
  ASSERT_EQ(2, vec[1]);
  ASSERT_EQ(3, vec[2]);

  ASSERT_EQ(1, vec.x());
  ASSERT_EQ(2, vec.y());
  ASSERT_EQ(3, vec.z());
}

TEST(test_vector, test_access_and_modify)
{
  esl::vector3i vec{1, 2, 3};

  vec[0] = 6;
  vec[1] = 7;
  vec[2] = 8;

  ASSERT_EQ(6, vec[0]);
  ASSERT_EQ(7, vec[1]);
  ASSERT_EQ(8, vec[2]);

  ASSERT_EQ(6, vec.x());
  ASSERT_EQ(7, vec.y());
  ASSERT_EQ(8, vec.z());

  vec.x() = 9;
  vec.y() = 10;
  vec.z() = 11;

  ASSERT_EQ(9, vec[0]);
  ASSERT_EQ(10, vec[1]);
  ASSERT_EQ(11, vec[2]);

  ASSERT_EQ(9, vec.x());
  ASSERT_EQ(10, vec.y());
  ASSERT_EQ(11, vec.z());
}

TEST(test_vector, test_operators)
{
  esl::vector3i vec{1, 2, 3};
  auto v1 = -vec;

  ASSERT_EQ(1, vec[0]);
  ASSERT_EQ(2, vec[1]);
  ASSERT_EQ(3, vec[2]);

  ASSERT_EQ(-1, v1[0]);
  ASSERT_EQ(-2, v1[1]);
  ASSERT_EQ(-3, v1[2]);
}

TEST(test_vector, test_math_functions)
{
  esl::vector3i vec{1, 2, 3};

  auto v1 = vec.square();
  auto v2 = v1.sqrt();
  auto v3 = (-v2).abs();
  auto v4 = vec.norm_squared();
  auto v5 = vec.norm();
  auto v6 = vec.sum();

  esl::vector3f v7{1, 2, 3};
  v7.normalize();

  ASSERT_EQ(1, v1[0]);
  ASSERT_EQ(4, v1[1]);
  ASSERT_EQ(9, v1[2]);

  ASSERT_EQ(1, v2[0]);
  ASSERT_EQ(2, v2[1]);
  ASSERT_EQ(3, v2[2]);

  ASSERT_EQ(1, v3[0]);
  ASSERT_EQ(2, v3[1]);
  ASSERT_EQ(3, v3[2]);

  ASSERT_EQ(1 + 4 + 9, v4);

  ASSERT_EQ(3, v5);

  ASSERT_EQ(6, v6);

  ASSERT_FLOAT_EQ(1, v7.norm());
}

TEST(test_vector, test_vector_math)
{
  esl::vector3i v1{1, 2, 3};
  esl::vector3i v2{4, 5, 6};

  auto v3 = v1 + v2;
  auto v4 = v1 - v2;
  auto v5 = v1 * 2;
  auto v6 = 2 * v1;
  auto v7 = v2 / 2;
  auto v8 = v1.dot(v2);
  auto v9 = v1.cross(v2);
  auto v10 = v1 + v2 + (v3 * 2).cross(v5);

  ASSERT_EQ(5, v3[0]);
  ASSERT_EQ(7, v3[1]);
  ASSERT_EQ(9, v3[2]);

  ASSERT_EQ(-3, v4[0]);
  ASSERT_EQ(-3, v4[1]);
  ASSERT_EQ(-3, v4[2]);

  ASSERT_EQ(2, v5[0]);
  ASSERT_EQ(4, v5[1]);
  ASSERT_EQ(6, v5[2]);

  ASSERT_EQ(2, v6[0]);
  ASSERT_EQ(4, v6[1]);
  ASSERT_EQ(6, v6[2]);

  ASSERT_EQ(2, v7[0]);
  ASSERT_EQ(2, v7[1]);
  ASSERT_EQ(3, v7[2]);

  ASSERT_EQ(4, v8[0]);
  ASSERT_EQ(10, v8[1]);
  ASSERT_EQ(18, v8[2]);

  ASSERT_EQ(-3, v9[0]);
  ASSERT_EQ(6, v9[1]);
  ASSERT_EQ(-3, v9[2]);

  ASSERT_EQ(17, v10[0]);
  ASSERT_EQ(-17, v10[1]);
  ASSERT_EQ(21, v10[2]);
}

TEST(test_vector, test_vector_math_operators)
{
  esl::vector3i v1{1, 2, 3};
  esl::vector3i v2{1, 1, 1};
  v2 += v1;

  ASSERT_EQ(2, v2[0]);
  ASSERT_EQ(3, v2[1]);
  ASSERT_EQ(4, v2[2]);

  esl::vector3i v3{1, 1, 1};
  v3 -= v1;

  ASSERT_EQ(0, v3[0]);
  ASSERT_EQ(-1, v3[1]);
  ASSERT_EQ(-2, v3[2]);

  esl::vector3i v4{1, 1, 1};
  v4 *= 2;

  ASSERT_EQ(2, v4[0]);
  ASSERT_EQ(2, v4[1]);
  ASSERT_EQ(2, v4[2]);

  esl::vector3i v5{1, 1, 1};
  v5 /= 2;

  ASSERT_EQ(0, v5[0]);
  ASSERT_EQ(0, v5[1]);
  ASSERT_EQ(0, v5[2]);
}

void testfun(const esl::vector3i& v)
{
  (void)v.data();
  (void)v[1];
  (void)v.x();
  (void)v.y();
  (void)v.z();
}

TEST(test_vector, test_const)
{
  esl::vector3i vec{1, 2, 3};
  testfun(vec);
}

int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
