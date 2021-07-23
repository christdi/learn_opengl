set(PROJECT_NAME textures)

file(GLOB_RECURSE SHADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/shader/${PROJECT_NAME}/*")
file(GLOB_RECURSE TEXTURE_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/texture/${PROJECT_NAME}/*")

add_executable(${PROJECT_NAME} main.cpp ${SHADER_LIST})

target_include_directories(${PROJECT_NAME} PUBLIC ../../include)
target_link_libraries(${PROJECT_NAME} PUBLIC myopengl glfw::glfw)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

message(${SHADER_LIST})
source_group("Shader Files" FILES ${SHADER_LIST})

file(COPY ${SHADER_LIST} DESTINATION shader)
file(COPY ${TEXTURE_LIST} DESTINATION texture)