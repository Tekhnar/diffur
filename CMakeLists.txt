cmake_minimum_required(VERSION 3.15)
project(diffur)

set(CMAKE_CXX_STANDARD 14)

add_executable(diffur main.cpp MyTree.cpp MyTree.h my_stack.h my_stack.cpp)