# Minimum CMake version required
cmake_minimum_required(VERSION 3.28)

# Project name and version
project(Asolto VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Option for Wayland support
option(USE_WAYLAND "Enable Wayland support" ON)
if(USE_WAYLAND)
    add_definitions(-DUSE_WAYLAND=ON)
endif()

# Add external library
add_subdirectory("/home/jarivanderkolk/Engines/engines/magique/" magique)

# Define the executable
add_executable(MyGame "Application.h" main.cpp)

# Link the library to the executable
target_link_libraries(MyGame PRIVATE magique)

# Additional warnings and debugging flags (optional)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(MyGame PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Print summary
message(STATUS "USE_WAYLAND: ${USE_WAYLAND}")
option(USE_WAYLAND "Enable Wayland support" ON)
