//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <algorithm>
#include <gtest/gtest.h>
#include <esl/containers/ring_buffer.hpp>
#include <stdexcept>

struct test_throw
{
  void operator()(const char* msg) const
  {
    throw std::runtime_error(msg);
  }
};

TEST(test_ring_buffer, test_push_back)
{
  int i = 100;

  esl::ring_buffer< int, 5, test_throw > buf;

  buf.push_back(1);
  buf.push_back(i);

  ASSERT_EQ(1, buf.front());
  ASSERT_EQ(100, buf.back());

  constexpr const int a[] = {6, 7};
  buf.push_back(a);

  ASSERT_EQ(1, buf.front());
  ASSERT_EQ(7, buf.back());


  EXPECT_ANY_THROW( buf.push_back(1); );
  EXPECT_ANY_THROW( buf.push_back(a); );
}

TEST(test_ring_buffer, test_emplace_back)
{
  int i = 100;

  esl::ring_buffer< int, 5, test_throw > buf;

  buf.emplace_back(1);
  buf.emplace_back(i);

  ASSERT_EQ(1, buf.front());
  ASSERT_EQ(100, buf.back());

  buf.emplace_back(1);
  buf.emplace_back(1);

  EXPECT_ANY_THROW( buf.emplace_back(1); );
}

TEST(test_ring_buffer, test_pop)
{
  esl::ring_buffer< int, 5, test_throw > buf;

  EXPECT_ANY_THROW( buf.pop(); );

  buf.push_back(1);
  buf.pop();

  EXPECT_ANY_THROW( buf.pop(); );
}

TEST(test_ring_buffer, test_size_free_empty_full)
{
  esl::ring_buffer< int, 5, test_throw > buf;

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

TEST(test_ring_buffer, test_clear)
{
  esl::ring_buffer< int, 5, test_throw > buf;

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

  buf.clear();
  ASSERT_EQ(0, buf.size());
  ASSERT_EQ(4, buf.free());
  ASSERT_EQ(true, buf.empty());
  ASSERT_EQ(false, buf.full());
}

TEST(test_ring_buffer, test_access)
{
  esl::ring_buffer< int, 5, test_throw > buf;

  EXPECT_ANY_THROW( buf.front(); );
  EXPECT_ANY_THROW( buf.back(); );

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
  ASSERT_EQ(0, buf.size());
  ASSERT_EQ(4, buf.free());
  ASSERT_EQ(true, buf.empty());
  ASSERT_EQ(false, buf.full());

  constexpr const int a[3] = {5, 6, 7};
  buf.push_back(a);
  ASSERT_EQ(5, buf.front());
  ASSERT_EQ(7, buf.back());
}


TEST(test_ring_buffer, test_access_and_modify)
{
  esl::ring_buffer< int, 5, test_throw > buf;

  buf.push_back(1);
  ASSERT_EQ(1, buf.front());
  ASSERT_EQ(1, buf.back());

  buf.push_back(2);
  ASSERT_EQ(1, buf.front());
  ASSERT_EQ(2, buf.back());

  buf.front() = 10;
  buf.back() -= 2;
  ASSERT_EQ(10, buf.front());
  ASSERT_EQ(0, buf.back());
}

void testconst_throw(const esl::ring_buffer< int, 5, test_throw > &buf)
{
  EXPECT_ANY_THROW( buf.front(); );
  EXPECT_ANY_THROW( buf.back(); );
}

void testconst(const esl::ring_buffer< int, 5, test_throw > &buf)
{
  ASSERT_EQ(1, buf.front());
  ASSERT_EQ(2, buf.back());
}

TEST(test_ring_buffer, test_const_access)
{
  esl::ring_buffer< int, 5, test_throw > buf;

  testconst_throw(buf);

  buf.push_back(1);
  buf.push_back(2);

  testconst(buf);
}

TEST(test_ring_buffer, test_sizeof)
{
  ASSERT_EQ(7, sizeof(esl::ring_buffer< std::uint8_t, 5>));
  ASSERT_EQ(12, sizeof(esl::ring_buffer< std::int16_t, 5>));
  ASSERT_EQ(24, sizeof(esl::ring_buffer< std::int32_t, 5>));
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
