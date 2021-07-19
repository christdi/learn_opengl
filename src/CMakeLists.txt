set(LIBRARY_NAME myopengl)

add_library(${LIBRARY_NAME} glad.cpp)

target_include_directories(${LIBRARY_NAME} PUBLIC ../include)