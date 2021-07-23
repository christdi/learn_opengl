#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <filesystem>

#include <glad/glad.h>

#include "myopengl/shader.h"
#include "myopengl/shader_exception.h"

namespace myopengl {

// Construct an instance of a shader.
//
// Parameters
// vertex_shader_path - path on the filesystem to a file containing a GLSL vertex shader
// fragment_shader_path - path on the filesystem to a file containing a GLSL fragment shader
//
// Throws
// shader_exception - thrown if the shader could not be configured
shader::shader(const char* vertex_shader_path, const char* fragment_shader_path)
    : _id(0) {
  assert(vertex_shader_path != NULL);
  assert(fragment_shader_path != NULL);

  load(vertex_shader_path, fragment_shader_path);
}

// Deconstructors an instance of a shader.
shader::~shader() {
  if (_id != 0) {
    glDeleteProgram(_id);
  }
}

// Loads vertex and fragment shader from disk, compiles and links them.
//
// Parameters
// vertex_shader_path - path on the filesystem to a file containing a GLSL vertex shader
// fragment_shader_path - path on the filesystem to a file containing a GLSL fragment shader
//
// Throws
// shader_exception - thrown if the shader could not be configured
void shader::load(const char* vertex_shader_path, const char* fragment_shader_path) {
  assert(vertex_shader_path != NULL);
  assert(fragment_shader_path != NULL);
  assert(_id == 0);

  unsigned int vertex_shader_id = 0;
  unsigned int fragment_shader_id = 0;

  try {
    std::string vertex_src = read_file_content(vertex_shader_path);
    std::string fragment_src = read_file_content(fragment_shader_path);

    vertex_shader_id = compile(GL_VERTEX_SHADER, vertex_src.c_str());
    fragment_shader_id = compile(GL_FRAGMENT_SHADER, fragment_src.c_str());

    _id = link(vertex_shader_id, fragment_shader_id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
  } catch (shader_exception& e) {
    if (vertex_shader_id != 0) {
      glDeleteShader(vertex_shader_id);
    }

    if (fragment_shader_id != 0) {
      glDeleteShader(fragment_shader_id);
    }

    throw e;
  }
}

// Sets an integer uniform value in the shaders.
//
// Parameters
// name - the name of the uniform to set
// value - the value to be set
void shader::set_int(const std::string& name, int value) const noexcept {
  assert(_id != 0);

  glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

// Sets a float uniform value in the shaders.
//
// Parameters
// name - the name of the uniform to set
// value - the value to be set
void shader::set_float(const std::string& name, float value) const noexcept {
  assert(_id != 0);

  glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}

// Instructs OpenGL to use this shader.
void shader::use() {
  assert(_id != 0);

  glUseProgram(_id);
}

// Reads the contents of a supplied file path.
//
// Parameters
// path - the path to the file to be read
//
// Throws
// shader_exception - if the contents of the file could not be read
//
// Returns the contents of the file as a string
std::string shader::read_file_content(const char* path) {
  assert(path != NULL);

  std::ifstream file;
  file.exceptions(std::ifstream::badbit | std::ifstream::failbit);

  try {
    std::stringstream buffer;

    file.open(path);
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
  } catch (std::ifstream::failure& e) {
    std::cout << "Error reading [" << path << "], "
              << "[" << e.what() << "]" << std::endl;

    if (file.is_open()) {
      file.close();
    }

    throw shader_exception(e.what());
  }

  return std::string();
}

// Compiles the source of a shader with a given type
//
// Parameters
// type - OpenGL shader type, i.e. GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
// src - Source code of the shader, probably loaded from a file
//
// Throws
// shader_exception - if the shader could not be compiled
//
// Returns the id of the shader from OpenGL.
unsigned int shader::compile(unsigned int type, const char* src) {
  assert(src != NULL);

  unsigned int shader = 0;

  shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);

  int success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (success != GL_TRUE) {
    char info[512];
    glGetShaderInfoLog(shader, 512, NULL, info);
    std::cout << "Error compiling shader: "
              << "[" << info << "]" << std::endl;

    throw shader_exception(info);
  }

  return shader;
}

// Attempts to link a vertex and fragment shader into an OpenGL shader program.
//
// Parameters
// vertex_shader_id - ID of the vertex shader provided by OpenGL
// fragment_shader_id - ID of the fragment shader provided by OpenGL
//
// Throws
// shader_exception - If the shaders could not be linked
//
// Returns the id of the shader program from OpenGL
unsigned int shader::link(unsigned int vertex_shader_id, unsigned int fragment_shader_id) {
  assert(vertex_shader_id != 0);
  assert(fragment_shader_id != 0);

  unsigned int program_id = 0;

  program_id = glCreateProgram();
  glAttachShader(program_id, vertex_shader_id);
  glAttachShader(program_id, fragment_shader_id);
  glLinkProgram(program_id);

  int success = 0;
  glGetProgramiv(program_id, GL_LINK_STATUS, &success);

  if (success != GL_TRUE) {
    char info[512];
    glGetProgramInfoLog(program_id, 512, NULL, info);

    std::cout << "Error linking shaders: "
              << "[" << info << "]" << std::endl;

    throw shader_exception(info);
  }

  return program_id;
}

}
