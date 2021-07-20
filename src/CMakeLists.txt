set(LIBRARY_NAME myopengl)

file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE SOURCE_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(${LIBRARY_NAME} ${SOURCE_LIST} ${HEADER_LIST})

target_include_directories(${LIBRARY_NAME} PUBLIC ../include)
target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_17)
set_target_properties(${LIBRARY_NAME} PROPERTIES CXX_EXTENSIONS OFF)

source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
