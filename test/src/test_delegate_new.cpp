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

using callback = esl::delegate< int(int), 3 * sizeof(void*) >;
TEST(test_delegate_new, function_test)
{
  int i  = 9;
  auto d = callback::from< foo >();
  auto d2 = callback::from(foo);

  ASSERT_EQ(25, d(5));
  ASSERT_EQ(100, d(10));
  ASSERT_EQ(81, d(i));

  ASSERT_EQ(25, d2(5));
  ASSERT_EQ(100, d2(10));
  ASSERT_EQ(81, d2(i));
}

TEST(test_delegate_new, method_test)
{
  bar a;

  int i  = 9;
  auto d = callback::from< bar, &bar::foo >(a);
  auto d2 = callback::from(a, &bar::foo);

  ASSERT_EQ(25, d(5));
  ASSERT_EQ(100, d(10));
  ASSERT_EQ(81, d(i));

  ASSERT_EQ(25, d2(5));
  ASSERT_EQ(100, d2(10));
  ASSERT_EQ(81, d2(i));
}

TEST(test_delegate_new, const_method_test)
{
  int i = 9;
  bar a;

  auto d = callback::from< bar, &bar::const_foo >(a);
  auto d2 = callback::from(a, &bar::const_foo);

  ASSERT_EQ(25, d(5));
  ASSERT_EQ(100, d(10));
  ASSERT_EQ(81, d(i));

  ASSERT_EQ(25, d2(5));
  ASSERT_EQ(100, d2(10));
  ASSERT_EQ(81, d2(i));
}

void callback_test(const callback& cb)
{
  int i = 9;

  ASSERT_EQ(25, cb(5));
  ASSERT_EQ(100, cb(10));
  ASSERT_EQ(81, cb(i));
}

TEST(test_delegate_new, const_ref_test)
{
  bar a;

  auto d = callback::from< bar, &bar::const_foo >(a);
  auto d2 = callback::from(a, &bar::const_foo);

  callback_test(d);
  callback_test(d2);
}

TEST(test_delegate_new, comparison_test)
{
  bar a;

  auto d = callback::from< bar, &bar::const_foo >(a);
  auto d2 = callback::from(a, &bar::const_foo);

  ASSERT_EQ(true, d == d);
  ASSERT_EQ(false, d != d);

  ASSERT_EQ(true, d2 == d2);
  ASSERT_EQ(false, d2 != d2);

  ASSERT_EQ(false, d == d2);
  ASSERT_EQ(true, d != d2);
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
