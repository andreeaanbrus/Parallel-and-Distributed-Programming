cmake_minimum_required(VERSION 3.15)
project(Lab4)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)
add_executable(Lab4 main.cpp Matrix.cpp Matrix.h)