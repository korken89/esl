//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <gtest/gtest.h>
#include <esl/containers/allocate.hpp>
#include <esl/containers/static_vector.hpp>

struct test_throw
{
  void operator()(const char* msg) const
  {
    throw std::runtime_error(msg);
  }
};

int i = 11;

TEST(test_static_vector, test_begin_end)
{
  esl::allocate< esl::static_vector< int, test_throw >, 5 > vec;
  ASSERT_EQ(vec.begin(), vec.end());
}

TEST(test_static_vector, test_push_back)
{
  esl::allocate< esl::static_vector< int, test_throw >, 5 > vec;

  vec.push_back(1);
  vec.emplace_back(2);

  vec.push_back(1);
  vec.push_back(1);
  vec.push_back(1);

  EXPECT_ANY_THROW(vec.push_back(1););
  EXPECT_ANY_THROW(vec.emplace_back(2););
}

TEST(test_static_vector, test_pop_back)
{
  esl::allocate< esl::static_vector< int, test_throw >, 5 > vec;

  vec.push_back(1);
  vec.pop_back();

  EXPECT_ANY_THROW(vec.pop_back(););
}

TEST(test_static_vector, test_size)
{
  esl::allocate< esl::static_vector< int, test_throw >, 5 > vec;

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

  EXPECT_ANY_THROW(vec.push_back(a););

  esl::allocate< esl::static_vector< int, test_throw >, 10 > b;

  b.push_back(3);
  b.push_back(7);

  vec.push_back(b);
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
  esl::allocate< esl::static_vector< int, test_throw >, 5 > vec;

  EXPECT_ANY_THROW(vec[0];);
  EXPECT_ANY_THROW(vec.front(););
  EXPECT_ANY_THROW(vec.back(););

  vec.push_back(1);
  ASSERT_EQ(1, vec[0]);

  vec.push_back(2);
  ASSERT_EQ(1, vec[0]);
  ASSERT_EQ(2, vec[1]);
  ASSERT_EQ(1, vec.front());
  ASSERT_EQ(2, vec.back());

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

TEST(test_static_vector, test_access_and_modify)
{
  esl::allocate< esl::static_vector< int, test_throw >, 5 > vec;

  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  ASSERT_EQ(1, vec[0]);
  ASSERT_EQ(2, vec[1]);
  ASSERT_EQ(3, vec[2]);

  vec[0] = 10;
  vec[1]++;
  vec[2] += 10;
  ASSERT_EQ(10, vec[0]);
  ASSERT_EQ(3, vec[1]);
  ASSERT_EQ(13, vec[2]);

  vec.front() -= 3;
  vec.back() = 0;
  ASSERT_EQ(7, vec[0]);
  ASSERT_EQ(0, vec[2]);

  *vec.data() = 12;
  ASSERT_EQ(12, vec[0]);
}

TEST(test_static_vector, test_algorithm)
{
  esl::allocate< esl::static_vector< int, test_throw >, 5 > vec;

  vec.push_back(19);
  vec.push_back(13);
  vec.push_back(18);
  vec.push_back(1);
  vec.push_back(-10);

  std::sort(vec.begin(), vec.end());
  int sum = std::accumulate(vec.cbegin(), vec.cend(), 0);

  ASSERT_EQ(-10, vec[0]);
  ASSERT_EQ(1, vec[1]);
  ASSERT_EQ(13, vec[2]);
  ASSERT_EQ(18, vec[3]);
  ASSERT_EQ(19, vec[4]);
  ASSERT_EQ(41, sum);
}

void testfun_throw(const esl::static_vector< int, test_throw >& v)
{
  EXPECT_ANY_THROW(v.front(););
  EXPECT_ANY_THROW(v.back(););
  EXPECT_ANY_THROW(v[10];);
}

void testfun(const esl::static_vector< int, test_throw >& v)
{
  (void)v.data();
  (void)v.front();
  (void)v.back();
  (void)v[0];
}

TEST(test_static_vector, test_const)
{
  esl::allocate< esl::static_vector< int, test_throw >, 5 > vec;

  testfun_throw(vec);

  vec.push_back(19);
  vec.push_back(13);
  vec.push_back(18);

  testfun(vec);
}

TEST(test_static_vector, test_erase_elem)
{
  esl::allocate< esl::static_vector< int, test_throw >, 5 > vec;

  vec.push_back(1);

  vec.erase(vec.begin());
  ASSERT_EQ(0, vec.size());

  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  vec.push_back(4);
  vec.push_back(5);

  vec.erase(vec.begin() + 3);

  ASSERT_EQ(4, vec.size());
  ASSERT_EQ(1, vec[0]);
  ASSERT_EQ(2, vec[1]);
  ASSERT_EQ(3, vec[2]);
  ASSERT_EQ(5, vec[3]);

  vec.erase(vec.begin());

  ASSERT_EQ(3, vec.size());
  ASSERT_EQ(2, vec[0]);
  ASSERT_EQ(3, vec[1]);
  ASSERT_EQ(5, vec[2]);

  vec.erase(vec.begin());
  vec.erase(vec.begin());
  vec.erase(vec.begin());

  ASSERT_EQ(0, vec.size());

  vec.push_back(6);
  vec.push_back(7);
  vec.push_back(8);

  ASSERT_EQ(3, vec.size());

  vec.erase(vec.begin() + 1);
  ASSERT_EQ(2, vec.size());
  ASSERT_EQ(6, vec[0]);
  ASSERT_EQ(8, vec[1]);

  vec.erase(vec.begin() + 1);
  ASSERT_EQ(1, vec.size());
  ASSERT_EQ(6, vec[0]);

  vec.erase(vec.begin());

  ASSERT_EQ(0, vec.size());
}

TEST(test_static_vector, test_erase_range)
{
  esl::allocate< esl::static_vector< int, test_throw >, 5 > vec;

  vec.push_back(1);
  vec.push_back(2);
  vec.push_back(3);
  vec.push_back(4);
  vec.push_back(5);

  vec.erase(vec.begin() + 1, vec.begin() + 3);

  ASSERT_EQ(3, vec.size());
  ASSERT_EQ(1, vec[0]);
  ASSERT_EQ(4, vec[1]);
  ASSERT_EQ(5, vec[2]);

  vec.erase(vec.begin() + 1, vec.begin() + 3);
  ASSERT_EQ(1, vec.size());
  ASSERT_EQ(1, vec[0]);

  ASSERT_EQ(vec.end(), vec.erase(vec.begin(), vec.begin()));

  vec.push_back(6);
  vec.push_back(7);
  vec.push_back(8);

  ASSERT_EQ(4, vec.size());

  vec.erase(vec.begin(), vec.end());

  ASSERT_EQ(0, vec.size());
}

TEST(test_static_vector, test_erase_error)
{
  esl::allocate< esl::static_vector< int, test_throw >, 5 > vec;

  EXPECT_ANY_THROW(vec.erase(vec.begin() + 2));
  EXPECT_ANY_THROW(vec.erase(vec.begin() + 2, vec.begin()));
}

int main(int argc, char* argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
