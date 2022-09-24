cmake_minimum_required(VERSION 3.14.0)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

set(PROJECT_NAME LexerTest)

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
  lexer_test
  lexer_test.cc
)

add_executable(
  token_factory_test
  token_factory_test.cc
)

target_link_libraries(
  lexer_test
  GTest::gtest_main
  ${LIBRARY_NAME}
  ${MARS_LIBRARIES}
)

target_link_libraries(
  token_factory_test
  GTest::gtest_main
  ${LIBRARY_NAME}
  ${MARS_LIBRARIES}
)

include(GoogleTest)

gtest_discover_tests(lexer_test)
gtest_discover_tests(token_factory_test)
