//          Copyright Emil Fresk 2017-2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <esl/callable/function_view.hpp>

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

using callback = esl::function_view< int(int) >;

TEST(test_function_view, function_test)
{
  int i  = 9;
  auto d = callback::from< foo >();
  auto d2 = callback::from(foo);
  auto d3 = callback( [](int i){ return i * i; } );

  ASSERT_EQ(25, d(5));
  ASSERT_EQ(100, d(10));
  ASSERT_EQ(81, d(i));

  ASSERT_EQ(25, d2(5));
  ASSERT_EQ(100, d2(10));
  ASSERT_EQ(81, d2(i));

  ASSERT_EQ(25, d3(5));
  ASSERT_EQ(100, d3(10));
  ASSERT_EQ(81, d3(i));
}

TEST(test_function_view, method_test)
{
  bar a;

  int i  = 9;
  auto d = callback::from< bar, &bar::foo >(a);

  ASSERT_EQ(25, d(5));
  ASSERT_EQ(100, d(10));
  ASSERT_EQ(81, d(i));
}

TEST(test_function_view, const_method_test)
{
  int i = 9;
  bar a;

  auto d = callback::from< bar, &bar::const_foo >(a);

  ASSERT_EQ(25, d(5));
  ASSERT_EQ(100, d(10));
  ASSERT_EQ(81, d(i));
}

void test_const_ref(const callback& cb)
{
  int i = 9;

  ASSERT_EQ(25, cb(5));
  ASSERT_EQ(100, cb(10));
  ASSERT_EQ(81, cb(i));
}

TEST(test_function_view, const_ref_test)
{
  bar a;

  auto d1 = callback::from< bar, &bar::const_foo >(a);
  auto d2 = callback::from< bar, &bar::foo >(a);
  auto d3 = callback::from< foo >();

  test_const_ref(d1);
  test_const_ref(d2);
  test_const_ref(d3);
}

TEST(test_function_view, comparison_test)
{
  bar a;

  auto d1 = callback::from< bar, &bar::const_foo >(a);
  auto d2 = callback::from< bar, &bar::foo >(a);
  auto d3 = callback::from< foo >();
  auto d4 = callback::from(foo);

  ASSERT_EQ(true, d1 == d1);
  ASSERT_EQ(false, d1 != d1);

  ASSERT_EQ(true, d2 == d2);
  ASSERT_EQ(false, d2 != d2);

  ASSERT_EQ(true, d3 == d3);
  ASSERT_EQ(false, d3 != d3);

  ASSERT_EQ(true, d4 == d4);
  ASSERT_EQ(false, d4 != d4);

  ASSERT_EQ(false, d1 == d2);
  ASSERT_EQ(true, d1 != d2);
  ASSERT_EQ(false, d1 == d3);
  ASSERT_EQ(true, d1 != d3);
  ASSERT_EQ(false, d1 == d4);
  ASSERT_EQ(true, d1 != d4);

  ASSERT_EQ(false, d2 == d3);
  ASSERT_EQ(true, d2 != d3);
  ASSERT_EQ(false, d2 == d4);
  ASSERT_EQ(true, d2 != d4);

  ASSERT_EQ(true, d3 == d4);
  ASSERT_EQ(false, d3 != d4);

}

void test_assign(const callback& cb)
{
  int i = 9;

  callback d2 = cb;
  callback d3{cb};
  callback d4(cb);

  ASSERT_EQ(true, cb == d2);
  ASSERT_EQ(true, cb == d3);
  ASSERT_EQ(true, cb == d4);

  ASSERT_EQ(25, d2(5));
  ASSERT_EQ(100, d2(10));
  ASSERT_EQ(81, d2(i));

  ASSERT_EQ(25, d3(5));
  ASSERT_EQ(100, d3(10));
  ASSERT_EQ(81, d3(i));

  ASSERT_EQ(25, d4(5));
  ASSERT_EQ(100, d4(10));
  ASSERT_EQ(81, d4(i));
}

TEST(test_function_view, assignment_test)
{
  int i = 9;
  auto d1 = callback::from< foo >();

  callback d2 = d1;
  callback d3{d1};
  callback d4(d1);

  ASSERT_EQ(true, d1 == d2);
  ASSERT_EQ(true, d1 == d3);
  ASSERT_EQ(true, d1 == d4);

  ASSERT_EQ(25, d2(5));
  ASSERT_EQ(100, d2(10));
  ASSERT_EQ(81, d2(i));

  ASSERT_EQ(25, d3(5));
  ASSERT_EQ(100, d3(10));
  ASSERT_EQ(81, d3(i));

  ASSERT_EQ(25, d4(5));
  ASSERT_EQ(100, d4(10));
  ASSERT_EQ(81, d4(i));

  test_assign(d1);
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
