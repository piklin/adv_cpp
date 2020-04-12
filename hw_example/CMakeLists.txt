cmake_minimum_required(VERSION 3.10)

project(hw_example)

add_executable(example main.cpp)
target_link_libraries(example log)
