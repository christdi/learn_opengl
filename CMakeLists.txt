cmake_minimum_required(VERSION 3.1)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

project("learnopengl" VERSION 1.0 DESCRIPTION "OpenGL learning project" LANGUAGES CXX)

find_package(glfw3)

add_subdirectory(src)
add_subdirectory(example)