set(PROJECT_NAME triangle)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ../../include)
target_link_libraries(${PROJECT_NAME} PUBLIC myopengl glfw::glfw)