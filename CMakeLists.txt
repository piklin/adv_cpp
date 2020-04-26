cmake_minimum_required(VERSION 3.10)

project(hw4)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MP_SOURCE_DIR src/)
set(MP_INCLUDE_DIR include/)
file(GLOB SOURCES ${MP_SOURCE_DIR}*.cpp)

add_library(mp SHARED ${SOURCES} src/FileDescriptor.cpp include/FileDescriptor.h)
target_include_directories(mp PUBLIC ${MP_INCLUDE_DIR})

add_subdirectory(example)