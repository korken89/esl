//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <esl/esl.hpp>

enum class test_flags
{
  A,
  B,
  C,
  D
};

TEST(test_enum_flags, constructor_and_access_test)
{
  esl::flag_enum<test_flags> f(test_flags::A, test_flags::C);

  ASSERT_EQ(true,  f[test_flags::A]);
  ASSERT_EQ(false, f[test_flags::B]);
  ASSERT_EQ(true,  f[test_flags::C]);
  ASSERT_EQ(false, f[test_flags::D]);
}

TEST(test_enum_flags, set_test)
{
  esl::flag_enum<test_flags> f;

  f.set(test_flags::A);
  f.set(test_flags::B, test_flags::C);

  ASSERT_EQ(true,  f[test_flags::A]);
  ASSERT_EQ(true,  f[test_flags::B]);
  ASSERT_EQ(true,  f[test_flags::C]);
  ASSERT_EQ(false, f[test_flags::D]);
}

TEST(test_enum_flags, clear_test)
{
  esl::flag_enum<test_flags> f;

  f.set(test_flags::A);
  f.set(test_flags::B, test_flags::C);
  f.clear(test_flags::A, test_flags::C);

  ASSERT_EQ(false, f[test_flags::A]);
  ASSERT_EQ(true,  f[test_flags::B]);
  ASSERT_EQ(false, f[test_flags::C]);
  ASSERT_EQ(false, f[test_flags::D]);
}

TEST(test_enum_flags, clear_all_test)
{
  esl::flag_enum<test_flags> f;

  f.set(test_flags::B, test_flags::C);

  f.clear();

  ASSERT_EQ(false, f[test_flags::A]);
  ASSERT_EQ(false, f[test_flags::B]);
  ASSERT_EQ(false, f[test_flags::C]);
  ASSERT_EQ(false, f[test_flags::D]);
}


TEST(test_enum_flags, any_test)
{
  esl::flag_enum<test_flags> f;

  f.set(test_flags::B, test_flags::C);

  ASSERT_EQ(true,  f.any(test_flags::A, test_flags::B));
  ASSERT_EQ(true,  f.any(test_flags::B, test_flags::A));

  ASSERT_EQ(false, f.any(test_flags::A, test_flags::D));
  ASSERT_EQ(true,  f.any(test_flags::C, test_flags::D));

  ASSERT_EQ(true,  f.any(test_flags::B, test_flags::C));

  ASSERT_EQ(true,  f.any(test_flags::B));
  ASSERT_EQ(false, f.any(test_flags::D));
}

TEST(test_enum_flags, all_test)
{
  esl::flag_enum<test_flags> f;

  f.set(test_flags::B, test_flags::C);

  ASSERT_EQ(false, f.all(test_flags::A, test_flags::B));
  ASSERT_EQ(false, f.all(test_flags::B, test_flags::A));

  ASSERT_EQ(false, f.all(test_flags::A, test_flags::D));
  ASSERT_EQ(false, f.all(test_flags::C, test_flags::D));

  ASSERT_EQ(true,  f.all(test_flags::B, test_flags::C));

  ASSERT_EQ(true,  f.all(test_flags::B));
  ASSERT_EQ(false, f.all(test_flags::D));
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
