//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <algorithm>
#include <esl/containers/static_vector.hpp>

int i = 11;

TEST(test_static_vector, test_push_back)
{
  esl::static_vector< int, 5 > vec;

  vec.push_back(1);
  vec.emplace_back(2);
}

TEST(test_static_vector, test_pop_back)
{
  esl::static_vector< int, 5 > vec;

  vec.push_back(1);
  vec.pop_back();
}

TEST(test_static_vector, test_size)
{
  esl::static_vector< int, 5 > vec;

  ASSERT_EQ(0, vec.size());
  ASSERT_EQ(5, vec.free());
  ASSERT_EQ(true, vec.empty());
  ASSERT_EQ(false, vec.full());

  vec.push_back(1);
  ASSERT_EQ(1, vec.size());
  ASSERT_EQ(4, vec.free());
  ASSERT_EQ(false, vec.empty());
  ASSERT_EQ(false, vec.full());

  vec.push_back(2);
  ASSERT_EQ(2, vec.size());
  ASSERT_EQ(3, vec.free());
  ASSERT_EQ(false, vec.empty());
  ASSERT_EQ(false, vec.full());

  vec.push_back(3);
  ASSERT_EQ(3, vec.size());
  ASSERT_EQ(2, vec.free());
  ASSERT_EQ(false, vec.empty());
  ASSERT_EQ(false, vec.full());

  vec.pop_back();
  ASSERT_EQ(2, vec.size());
  ASSERT_EQ(3, vec.free());
  ASSERT_EQ(false, vec.empty());
  ASSERT_EQ(false, vec.full());

  vec.pop_back();
  ASSERT_EQ(1, vec.size());
  ASSERT_EQ(4, vec.free());
  ASSERT_EQ(false, vec.empty());
  ASSERT_EQ(false, vec.full());

  vec.pop_back();
  ASSERT_EQ(0, vec.size());
  ASSERT_EQ(5, vec.free());
  ASSERT_EQ(true, vec.empty());
  ASSERT_EQ(false, vec.full());

  constexpr const int a[3] = {5, 6, 7};
  vec.push_back(a);
  ASSERT_EQ(3, vec.size());
  ASSERT_EQ(2, vec.free());
  ASSERT_EQ(false, vec.empty());
  ASSERT_EQ(false, vec.full());

  vec.push_back(3);
  vec.push_back(7);
  ASSERT_EQ(5, vec.size());
  ASSERT_EQ(0, vec.free());
  ASSERT_EQ(false, vec.empty());
  ASSERT_EQ(true, vec.full());

  vec.clear();
  ASSERT_EQ(0, vec.size());
  ASSERT_EQ(5, vec.free());
  ASSERT_EQ(true, vec.empty());
  ASSERT_EQ(false, vec.full());
}

TEST(test_static_vector, test_access)
{
  esl::static_vector< int, 5 > vec;

  vec.push_back(1);
  ASSERT_EQ(1, vec[0]);

  vec.push_back(2);
  ASSERT_EQ(1, vec[0]);
  ASSERT_EQ(2, vec[1]);

  vec.push_back(3);
  ASSERT_EQ(1, vec[0]);
  ASSERT_EQ(2, vec[1]);
  ASSERT_EQ(3, vec[2]);

  vec.pop_back();
  ASSERT_EQ(1, vec[0]);
  ASSERT_EQ(2, vec[1]);

  vec.pop_back();
  ASSERT_EQ(1, vec[0]);

  vec.pop_back();

  constexpr const int a[3] = {5, 6, 7};

  vec.push_back(a);
  ASSERT_EQ(5, vec[0]);
  ASSERT_EQ(6, vec[1]);
  ASSERT_EQ(7, vec[2]);

  vec.push_back(3);
  vec.push_back(7);
  ASSERT_EQ(5, vec[0]);
  ASSERT_EQ(6, vec[1]);
  ASSERT_EQ(7, vec[2]);
  ASSERT_EQ(3, vec[3]);
  ASSERT_EQ(7, vec[4]);
}

TEST(test_static_vector, test_algorithm)
{
  esl::static_vector< int, 5 > vec;

  vec.push_back(19);
  vec.push_back(13);
  vec.push_back(18);
  vec.push_back(1);
  vec.push_back(-10);

  std::sort(vec.begin(), vec.end());

  ASSERT_EQ(-10, vec[0]);
  ASSERT_EQ(1, vec[1]);
  ASSERT_EQ(13, vec[2]);
  ASSERT_EQ(18, vec[3]);
  ASSERT_EQ(19, vec[4]);
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
