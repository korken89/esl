//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <esl/esl.hpp>

TEST(test_unsafe_flag, unsafe_flag)
{
  esl::unsafe_flag<4> f;

  ASSERT_EQ(false, f[0]);
  ASSERT_EQ(false, f[1]);
  ASSERT_EQ(false, f[2]);
  ASSERT_EQ(false, f[3]);
}

TEST(test_unsafe_flag, set_test)
{
  esl::unsafe_flag<4> f;

  f.set(0);
  f.set(1);
  f.set(2);

  ASSERT_EQ(true,  f[0]);
  ASSERT_EQ(true,  f[1]);
  ASSERT_EQ(true,  f[2]);
  ASSERT_EQ(false, f[3]);
}

TEST(test_unsafe_flag, clear_test)
{
  esl::unsafe_flag<4> f;

  f.set(0);
  f.set(1);
  f.set(2);
  f.reset(0);
  f.reset(2);

  ASSERT_EQ(false, f[0]);
  ASSERT_EQ(true,  f[1]);
  ASSERT_EQ(false, f[2]);
  ASSERT_EQ(false, f[3]);
}

TEST(test_unsafe_flag, clear_all_test)
{
  esl::unsafe_flag<4> f;

  f.set(1);
  f.set(2);

  f.reset();

  ASSERT_EQ(false, f[0]);
  ASSERT_EQ(false, f[1]);
  ASSERT_EQ(false, f[2]);
  ASSERT_EQ(false, f[3]);
}

TEST(test_unsafe_flag, set_all_test)
{
  esl::unsafe_flag<4> f;

  f.set();

  ASSERT_EQ(true, f[0]);
  ASSERT_EQ(true, f[1]);
  ASSERT_EQ(true, f[2]);
  ASSERT_EQ(true, f[3]);
}


TEST(test_unsafe_flag, all_test)
{
  esl::unsafe_flag<4> f;

  f.set();

  ASSERT_EQ(true, f.all());
}

TEST(test_unsafe_flag, none_test)
{
  esl::unsafe_flag<4> f;

  f.none();

  ASSERT_EQ(true, f.none());
}


int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
