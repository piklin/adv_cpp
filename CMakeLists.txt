cmake_minimum_required(VERSION 3.0)

project(hw1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(process STATIC src/Process.cpp)
target_include_directories(process PUBLIC src include)

add_subdirectory(example)