if(BUILD_TRIANGLE OR BUILD_ALL)
add_subdirectory(triangle)
endif()

if(BUILD_SHADERS OR BUILD_ALL)
add_subdirectory(shaders)
endif()

if(BUILD_TEXTURES OR BUILD_ALL)
add_subdirectory(textures)
endif()

if(BUILD_TRANSFORMATION OR BUILD_ALL)
add_subdirectory(transformation)
endif()