cmake_minimum_required(VERSION 3.10)

project(hw3)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(net SHARED src/net.cpp)
target_include_directories(net PUBLIC src include)

add_subdirectory(hw_example)
