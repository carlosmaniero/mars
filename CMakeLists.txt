cmake_minimum_required(VERSION 3.14)

set(MARS_PROJECT_NAME mc)
set(MARS_PROJECT_SOURCE_DIR src)

project(${MARS_PROJECT_NAME})

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

set(MARS_LIBRARIES_DIR
    libs
)

set(MARS_SOURCE
    ${MARS_PROJECT_SOURCE_DIR}/mc.cc
)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.
include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Find the libraries that correspond to the LLVM components
# that we wish to use
# llvm_map_components_to_libnames(llvm_libs support core irreader)
llvm_map_components_to_libnames(llvm_libs ${LLVM_TARGETS_TO_BUILD} support core irreader codegen mc mcparser option)

# Link against LLVM libraries
target_link_libraries(${LIBRARY_NAME} ${llvm_libs})


include(FetchContent)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.10.0
)

FetchContent_MakeAvailable(spdlog)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # does not produce the json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works

add_executable(${MARS_PROJECT_NAME} ${MARS_SOURCE})

set(MARS_LIBRARIES
    mclexer
    mcparser
    mcllvm
)

foreach(LIBRARY ${MARS_LIBRARIES})
    add_subdirectory("${MARS_LIBRARIES_DIR}/${LIBRARY}")
endforeach(LIBRARY)

foreach(LIBRARY ${MARS_LIBRARIES})
    target_link_libraries(${MARS_PROJECT_NAME} ${LIBRARY})
endforeach(LIBRARY)

target_link_libraries(${MARS_PROJECT_NAME} spdlog::spdlog)
