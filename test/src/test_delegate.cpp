//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <esl/delegate/delegate.hpp>

struct bar
{
  int foo(int i)
  {
    return i * i;
  }

  int const_foo(int i) const
  {
    return i * i;
  }
};

int foo(int i)
{
  return i * i;
}

using callback = esl::delegate< int(int) >;
TEST(test_delegate, function_test)
{
  int i  = 9;
  auto d = callback::from< foo >();

  ASSERT_EQ(25, d(5));
  ASSERT_EQ(100, d(10));
  ASSERT_EQ(81, d(i));
}

TEST(test_delegate, method_test)
{
  bar a;

  int i  = 9;
  auto d = callback::from< bar, &bar::foo >(a);

  ASSERT_EQ(25, d(5));
  ASSERT_EQ(100, d(10));
  ASSERT_EQ(81, d(i));
}

TEST(test_delegate, const_method_test)
{
  int i = 9;
  bar a;

  auto d = callback::from< bar, &bar::const_foo >(a);

  ASSERT_EQ(25, d(5));
  ASSERT_EQ(100, d(10));
  ASSERT_EQ(81, d(i));
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
