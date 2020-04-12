cmake_minimum_required(VERSION 3.10)

project(hw2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(log SHARED src/Logger.cpp)
target_include_directories(log PUBLIC src include)

add_subdirectory(hw_example)