cmake_minimum_required(VERSION 3.17)
project(rich)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(rich
        cmdline.cpp
        cmdline.h
        main.cpp
        map.cpp
        map.h
        player.cpp
        player.h)
