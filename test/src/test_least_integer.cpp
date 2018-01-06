//          Copyright Emil Fresk 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <gtest/gtest.h>
#include <esl/helpers/utils.hpp>

TEST(test_least_integer, all_test)
{
  static_assert(std::is_same<esl::uint_least_t<0>, std::uint8_t>::value, "uint8_t failed");
  static_assert(std::is_same<esl::uint_least_t<255>, std::uint8_t>::value, "uint8_t failed");
  static_assert(std::is_same<esl::uint_least_t<256>, std::uint16_t>::value, "uint16_t failed");
  static_assert(std::is_same<esl::uint_least_t<65535>, std::uint16_t>::value, "uint16_t failed");
  static_assert(std::is_same<esl::uint_least_t<65536>, std::uint32_t>::value, "uint32_t failed");
  static_assert(std::is_same<esl::uint_least_t<4294967295>, std::uint32_t>::value, "uint32_t failed");
  static_assert(std::is_same<esl::uint_least_t<4294967296>, std::uint64_t>::value, "uint64_t failed");

  static_assert(std::is_same<esl::int_least_t<0>, std::int8_t>::value, "int8_t failed");
  static_assert(std::is_same<esl::int_least_t<127>, std::int8_t>::value, "int8_t failed");
  static_assert(std::is_same<esl::int_least_t<128>, std::int16_t>::value, "int16_t failed");
  static_assert(std::is_same<esl::int_least_t<32767>, std::int16_t>::value, "int16_t failed");
  static_assert(std::is_same<esl::int_least_t<32768>, std::int32_t>::value, "int32_t failed");
  static_assert(std::is_same<esl::int_least_t<2147483647>, std::int32_t>::value, "int32_t failed");
  static_assert(std::is_same<esl::int_least_t<2147483648>, std::int64_t>::value, "int64_t failed");
}

int main(int argc, char *argv[])
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
