##          Copyright Emil Fresk 2017-2018.
## Distributed under the Boost Software License, Version 1.0.
##    (See accompanying file LICENSE.md or copy at
##          http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.1)
project(esl)

########################################
# C++ flags
########################################
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

option(ENABLE_COVERAGE "Enable coverage reporting" FALSE)
option(ENABLE_CPP17 "Enable C++17" FALSE)

set(Extra_Link_Flags "")

######################################
# Find packages needed
######################################
find_package(GTest REQUIRED)

######################################
# Set up the testing
######################################
include_directories(
  ${GTEST_INCLUDE_DIRS}
  src
)

########################################
# Testing
########################################

enable_testing()

#
# Macro for tests
#
macro(perform_test str)

  # UBSAN / ASAN
  add_executable(test_${str} test/src/test_${str}.cpp)
  set_target_properties(test_${str} PROPERTIES COMPILE_FLAGS "-fsanitize=undefined,address -std=c++14 -g")
  target_link_libraries(test_${str} ${GTEST_BOTH_LIBRARIES} -fsanitize=undefined,address -fuse-ld=gold)
  add_test(NAME ${str} COMMAND test_${str})

if (ENABLE_COVERAGE)
  add_executable(test_${str}_cov test/src/test_${str}.cpp)
  set_target_properties(test_${str}_cov PROPERTIES COMPILE_FLAGS "-std=c++14 -O0 --coverage")
  target_link_libraries(test_${str}_cov ${GTEST_BOTH_LIBRARIES} --coverage -fuse-ld=gold)
  add_test(NAME ${str}_cov COMMAND test_${str}_cov)
endif()

  if (ENABLE_CPP17)
    # UBSAN / ASAN
    add_executable(test_${str}_cpp17 test/src/test_${str}.cpp)
    set_target_properties(test_${str}_cpp17 PROPERTIES COMPILE_FLAGS "-fsanitize=undefined,address -std=c++17 -g")
    target_link_libraries(test_${str}_cpp17 ${GTEST_BOTH_LIBRARIES} -fsanitize=undefined,address -fuse-ld=gold)
    add_test(NAME ${str}_cpp17 COMMAND test_${str}_cpp17)

    if (ENABLE_COVERAGE)
      add_executable(test_${str}_cov_cpp17 test/src/test_${str}.cpp)
      set_target_properties(test_${str}_cov_cpp17 PROPERTIES COMPILE_FLAGS "-std=c++17 -O0 --coverage")
      target_link_libraries(test_${str}_cov_cpp17 ${GTEST_BOTH_LIBRARIES} --coverage -fuse-ld=gold)
      add_test(NAME ${str}_cov_cpp17 COMMAND test_${str}_cov_cpp17)
    endif()
  endif()

endmacro(perform_test)

#
# Unit Tests
#
perform_test(flag_enum)
perform_test(function)
perform_test(function_view)
perform_test(least_integer)
perform_test(repeat)
perform_test(ring_buffer)
perform_test(singleton)
perform_test(static_vector)
perform_test(unsafe_flag)
perform_test(vector)
perform_test(quaternion)
