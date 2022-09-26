cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME
    McParserLibrary
)
set(LIBRARY_NAME
    mcparser
)
set(LIBRARY_HEADERS_DIR
    include/${LIBRARY_NAME}
)
set(LIBRARY_HEADERS
    ${LIBRARY_HEADERS_DIR}/context.h
    ${LIBRARY_HEADERS_DIR}/parser.h
)
set(LIBRARY_SOURCE_DIR
    src
)
set(LIBRARY_SOURCE
    ${LIBRARY_SOURCE_DIR}/context.cc
    ${LIBRARY_SOURCE_DIR}/parser.cc
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
    ${MARS_LIBRARIES}
)

target_include_directories(${LIBRARY_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${MARS_LIBRARIES}
)

target_link_libraries(
    ${LIBRARY_NAME}
    mclexer
)

add_subdirectory(test)
