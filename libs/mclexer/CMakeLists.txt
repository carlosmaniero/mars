cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME
    McLexerLibrary
)
set(LIBRARY_NAME
    mclexer
)
set(LIBRARY_HEADERS_DIR
    include/${LIBRARY_NAME}
)
set(LIBRARY_HEADERS
    ${LIBRARY_HEADERS_DIR}/token.h
    ${LIBRARY_HEADERS_DIR}/token_factory.h
    ${LIBRARY_HEADERS_DIR}/lexer.h
)
set(LIBRARY_SOURCE_DIR
    src
)
set(LIBRARY_SOURCE
    ${LIBRARY_SOURCE_DIR}/token.cc
    ${LIBRARY_SOURCE_DIR}/token_factory.cc
    ${LIBRARY_SOURCE_DIR}/lexer.cc
)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

project(${PROJECT_NAME})

add_library(${LIBRARY_NAME} STATIC
    ${LIBRARY_HEADERS}
    ${LIBRARY_SOURCE}
)

target_include_directories(${LIBRARY_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/${LIBRARY_NAME}>
    $<INSTALL_INTERFACE:include/${LIBRARY_NAME}>
)

target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_subdirectory(test)
