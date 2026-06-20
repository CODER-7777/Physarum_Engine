cmake_minimum_required(VERSION 3.10)

# Name your project
project(PhysarumEngine)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Tell CMake to build an executable from main.cpp
add_executable(PhysarumEngine main.cpp extern/glad/src/glad.c)
target_include_directories(PhysarumEngine PRIVATE
        ${CMAKE_SOURCE_DIR}/extern/glad/include        
)
target_link_libraries(PhysarumEngine PRIVATE OpenGL::GL glfw)