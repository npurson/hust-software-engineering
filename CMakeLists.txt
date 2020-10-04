cmake_minimum_required(VERSION 3.17)
project(rich)

set(CMAKE_CXX_STANDARD 14)

add_executable(rich main.cpp map.cpp map.h player.cpp player.h cmdline.h cmdline.cpp)