cmake_minimum_required(VERSION 3.17)
project(rich)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(rich
        .idea/misc.xml
        .idea/modules.xml
        .idea/rich.iml
        .idea/vcs.xml
        .idea/workspace.xml
        cmake-build-debug/cmake_install.cmake
        cmake-build-debug/CMakeCache.txt
        cmake-build-debug/Makefile
        cmake-build-debug/rich.cbp
        .gitignore
        CMakeLists.txt
        cmdline.cpp
        cmdline.h
        main.cpp
        map.cpp
        map.h
        player.cpp
        player.h)
