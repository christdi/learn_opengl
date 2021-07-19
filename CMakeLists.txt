cmake_minimum_required(VERSION 3.1)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
message(${CMAKE_BINARY_DIR})
set(PROJECT_NAME opengl_triangle)

project(${PROJECT_NAME} VERSION 1.0 DESCRIPTION "Render a triangle in OpenGL" LANGUAGES CXX)

add_executable(${PROJECT_NAME} src/main.cpp src/glad.cpp)

find_package(glfw3)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw::glfw)