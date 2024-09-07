# Minimum CMake version required
cmake_minimum_required(VERSION 3.28)

# Project name and version
project(MyProject VERSION 1.0)

# Specify the C++ standard (you can change this to any version, e.g., C++11, C++14, C++20)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory("C:/Dev/Magique-Engine" magique)
add_executable(MyGame "Application.h"  main.cpp)
target_link_libraries(MyGame PRIVATE magique)