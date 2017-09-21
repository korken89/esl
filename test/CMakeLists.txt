cmake_minimum_required(VERSION 3.1)
project(esl_test)

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
  ../src/
)

########################################
# Testing
########################################

enable_testing()

#
# Unit Tests
#
add_executable(test_static_vector src/test_static_vector.cpp)
target_link_libraries(test_static_vector ${GTEST_BOTH_LIBRARIES})

add_executable(test_ring_buffer src/test_ring_buffer.cpp)
target_link_libraries(test_ring_buffer ${GTEST_BOTH_LIBRARIES})

add_executable(test_delegate src/test_delegate.cpp)
target_link_libraries(test_delegate  ${GTEST_BOTH_LIBRARIES})

add_test(NAME static_vector COMMAND test_static_vector)
add_test(NAME ring_buffer COMMAND test_delegate)
add_test(NAME delegate COMMAND test_delegate)
