cmake_minimum_required(VERSION 3.15)
project(GaussianBlur)

set (CMAKE_CXX_FLAGS "-lpng")
set(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_CXX_STANDARD 14)

add_executable(GaussianBlur main.cpp Picture.hpp lib/jpglib.hpp lib/stb_image.h lib/stb_image_write.h GaussianBlur.hpp lib/threadPool.hpp)