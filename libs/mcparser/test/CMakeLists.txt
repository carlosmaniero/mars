cmake_minimum_required(VERSION 3.14.0)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

set(PROJECT_NAME ParserTest)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # does not produce the json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  parser_test
  parser_test.cc
)

target_link_libraries(
  parser_test
  GTest::gtest_main
  ${LIBRARY_NAME}
  ${MARS_LIBRARIES}
)

include(GoogleTest)

gtest_discover_tests(parser_test)
