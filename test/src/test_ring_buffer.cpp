//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <algorithm>
#include <gtest/gtest.h>
#include <esl/containers/ring_buffer.hpp>

TEST(test_ring_buffer, test_push_back)
{
  int i = 100;

  esl::ring_buffer< int, 5 > buf;

  buf.push_back(1);
  buf.push_back(i);
}

TEST(test_ring_buffer, test_pop)
{
  esl::ring_buffer< int, 5 > buf;

  buf.push_back(1);
  buf.pop();
}

TEST(test_ring_buffer, test_size)
{
  esl::ring_buffer< int, 5 > buf;

  ASSERT_EQ(0, buf.size());
  ASSERT_EQ(4, buf.free());
  ASSERT_EQ(true, buf.empty());
  ASSERT_EQ(false, buf.full());

  buf.push_back(1);
  ASSERT_EQ(1, buf.size());
  ASSERT_EQ(3, buf.free());
  ASSERT_EQ(false, buf.empty());
  ASSERT_EQ(false, buf.full());

  buf.push_back(2);
  ASSERT_EQ(2, buf.size());
  ASSERT_EQ(2, buf.free());
  ASSERT_EQ(false, buf.empty());
  ASSERT_EQ(false, buf.full());

  buf.push_back(3);
  ASSERT_EQ(3, buf.size());
  ASSERT_EQ(1, buf.free());
  ASSERT_EQ(false, buf.empty());
  ASSERT_EQ(false, buf.full());

  buf.pop();
  ASSERT_EQ(2, buf.size());
  ASSERT_EQ(2, buf.free());
  ASSERT_EQ(false, buf.empty());
  ASSERT_EQ(false, buf.full());

  buf.pop();
  ASSERT_EQ(1, buf.size());
  ASSERT_EQ(3, buf.free());
  ASSERT_EQ(false, buf.empty());
  ASSERT_EQ(false, buf.full());

  buf.pop();
  ASSERT_EQ(0, buf.size());
  ASSERT_EQ(4, buf.free());
  ASSERT_EQ(true, buf.empty());
  ASSERT_EQ(false, buf.full());

  constexpr const int a[3] = {5, 6, 7};
  buf.push_back(a);
  ASSERT_EQ(3, buf.size());
  ASSERT_EQ(1, buf.free());
  ASSERT_EQ(false, buf.empty());
  ASSERT_EQ(false, buf.full());

  buf.push_back(3);
  ASSERT_EQ(4, buf.size());
  ASSERT_EQ(0, buf.free());
  ASSERT_EQ(false, buf.empty());
  ASSERT_EQ(true, buf.full());
}

TEST(test_ring_buffer, test_access)
{
  esl::ring_buffer< int, 5 > buf;

  buf.push_back(1);
  ASSERT_EQ(1, buf.front());
  ASSERT_EQ(1, buf.back());

  buf.push_back(2);
  ASSERT_EQ(1, buf.front());
  ASSERT_EQ(2, buf.back());

  buf.push_back(3);
  ASSERT_EQ(1, buf.front());
  ASSERT_EQ(3, buf.back());

  buf.pop();
  ASSERT_EQ(2, buf.front());
  ASSERT_EQ(3, buf.back());

  buf.pop();
  ASSERT_EQ(3, buf.front());
  ASSERT_EQ(3, buf.back());

  buf.pop();

  constexpr const int a[3] = {5, 6, 7};
  buf.push_back(a);
  ASSERT_EQ(5, buf.front());
  ASSERT_EQ(7, buf.back());
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}