cmake_minimum_required(VERSION 3.1)

set(PROJECT_NAME opengl_triangle)

project(${PROJECT_NAME} VERSION 1.0 DESCRIPTION "Render a triangle in OpenGL" LANGUAGES CXX)

add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION "C:/dev/opengl/lib/glfw3.lib" INTERFACE_INCLUDE_DIRECTORIES "C:/dev/opengl/include")

add_executable(${PROJECT_NAME} src/main.cpp src/glad.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw)