cmake_minimum_required(VERSION 3.1)
project(esl)

########################################
# Enable C++14 and other C++ flags
########################################
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)


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
  target_link_libraries(test_${str} ${GTEST_BOTH_LIBRARIES})
  add_test(NAME ${str} COMMAND test_${str})
endmacro(perform_test)

#
# Unit Tests
#
perform_test(static_vector)
perform_test(ring_buffer)
perform_test(delegate)
perform_test(singleton)
perform_test(unsafe_flag)
perform_test(flag_enum)
perform_test(vector)
