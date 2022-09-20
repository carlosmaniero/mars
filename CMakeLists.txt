cmake_minimum_required(VERSION 3.14)

set(MARS_PROJECT_NAME mars)
set(MARS_PROJECT_SOURCE_DIR src)

project(${MARS_PROJECT_NAME})

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

set(MARS_LIBRARIES_DIR
    libs
)
set(MARS_LIBRARIES
    mclexer
)

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
  hello_test
  hello_test.cc
)

target_link_libraries(
  hello_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)

foreach(LIBRARY ${MARS_LIBRARIES})
    add_subdirectory("${MARS_LIBRARIES_DIR}/${LIBRARY}")
endforeach(LIBRARY)

# target_link_libraries(${MARS_PROJECT_NAME} ${MARS_LIBRARIES})
