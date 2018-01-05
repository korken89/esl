cmake_minimum_required(VERSION 3.1)
project(esl)

########################################
# Enable C++14 and other C++ flags
########################################
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
add_compile_options(-Wall -Wextra -Wpedantic -Werror)

option(ENABLE_COVERAGE "Enable coverage reporting" FALSE)
option(ENABLE_CPP17 "Enable C++17" FALSE)
option(ENABLE_UBSAN "Enable UB Sanitizer" FALSE)
option(ENABLE_ASAN "Enable Address Sanitizer" FALSE)

set(Extra_Link_Flags "")

if (ENABLE_COVERAGE)
  add_compile_options(--coverage -O0)
  set(Extra_Link_Flags ${Extra_Link_Flags} "--coverage")
endif()

if (ENABLE_CPP17)
  add_compile_options(-std=c++17)
else()
  set(CMAKE_CXX_STANDARD 14)
endif()

if (ENABLE_UBSAN)
  add_compile_options(-fsanitize=undefined)
  set(Extra_Link_Flags ${Extra_Link_Flags} "-fsanitize=undefined")
endif()

if (ENABLE_ASAN)
  add_compile_options(-fsanitize=address)
  set(Extra_Link_Flags ${Extra_Link_Flags} "-fsanitize=address")
endif()

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
  add_executable(test_${str} test/src/test_${str}.cpp)
  target_link_libraries(test_${str} ${GTEST_BOTH_LIBRARIES} ${Extra_Link_Flags})
  add_test(NAME ${str} COMMAND test_${str})
endmacro(perform_test)

#
# Unit Tests
#
perform_test(static_vector)
perform_test(ring_buffer)
perform_test(delegate)
perform_test(function_view)
perform_test(singleton)
perform_test(unsafe_flag)
perform_test(flag_enum)
perform_test(vector)
