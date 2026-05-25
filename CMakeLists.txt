cmake_minimum_required(VERSION 3.10)

# Name your project
project(PhysarumEngine)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Tell CMake to build an executable from main.cpp
add_executable(PhysarumEngine main.cpp)