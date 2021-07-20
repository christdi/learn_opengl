set(PROJECT_NAME triangle)

file(GLOB_RECURSE SHADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/shader/triangle/*.glsl")

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ../../include)
target_link_libraries(${PROJECT_NAME} PUBLIC myopengl glfw::glfw)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

file(COPY ${SHADER_LIST} DESTINATION shader)