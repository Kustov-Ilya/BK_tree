cmake_minimum_required(VERSION 3.13)
project(BK_tree)

set(CMAKE_CXX_STANDARD 17)

add_executable(BK_tree main.cpp BK_tree_class.cpp BK_tree_class.h correct_file.cpp correct_file.h)