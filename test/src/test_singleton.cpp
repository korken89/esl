//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <esl/helpers/singleton.hpp>

struct foo
{
  int i;
};

TEST(test_singleton, write_test)
{
  auto &s = esl::get_singleton< foo >();

  s.i = 101;

  ASSERT_EQ(101, s.i);
}

void set_singleton()
{
  auto &s = esl::get_singleton< foo >();
  s.i     = 101;
}

TEST(test_singleton, access_test)
{
  auto &s = esl::get_singleton< foo >();

  set_singleton();

  ASSERT_EQ(101, s.i);
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
