cmake_minimum_required(VERSION 3.15)
project(Lab1)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(Lab1 main.cpp Bank.cpp Bank.h Account.cpp Account.h Transaction.cpp Transaction.h)