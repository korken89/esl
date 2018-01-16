//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>
#include <cstdint>
#include <gtest/gtest.h>
#include <esl/helpers/utils.hpp>


TEST(test_repeat, test_repeat_normal)
{
  std::size_t i = 0;
  esl::repeat< 10 >([&i](auto idx) { i += idx; });

  ASSERT_EQ(45, i);
}

template < std::size_t N >
struct test
{
  auto val() const
  {
    return N;
  }
};

TEST(test_repeat, test_repeat_compile_time)
{
  std::size_t i = 0;
  esl::repeat< 10 >([&i](const auto idx) {
    auto s = test< idx >{};
    i += s.val();
  });

  ASSERT_EQ(45, i);
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
