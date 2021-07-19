#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

void process_input(GLFWwindow* window);
void on_window_change(GLFWwindow* window, int width, int height);
unsigned int compile_shader(unsigned int type, const char* source);
unsigned int link_shaders(const std::vector<unsigned int>& shaders);
void configure_buffer(unsigned int& vertex_buffer_object, unsigned int& vertex_array_object);

// Entry method for the applications
//
// Parameters
// argc - count of command line argumnets
// argv - array of command line arguments
int main(int argc, char* argv[]) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "opengl_triangle", NULL, NULL);

  if (window == NULL) {
    std::cout << "Failed to create window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, on_window_change);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialise GLAD" << std::endl;
    glfwTerminate();
    return -1;
  }

  static const char* vertex_shader_source = "#version 330 core\n"
                                            "layout (location = 0) in vec3 aPos;\n"
                                            "void main()\n"
                                            "{\n"
                                            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                            "}\0";

  static const char* fragment_shader_source = "#version 330 core\n"
                                              "out vec4 FragColor;\n"
                                              "void main()\n"
                                              "{\n"
                                              "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                              "}\n\0";

  std::vector<unsigned int> shaders;
  shaders.push_back(compile_shader(GL_VERTEX_SHADER, vertex_shader_source));
  shaders.push_back(compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source));
  unsigned int shader_program = link_shaders(shaders);

  unsigned int vertex_buffer_object = 0;
  unsigned int vertex_array_object = 0;
  configure_buffer(vertex_buffer_object, vertex_array_object);

  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program);
    glBindVertexArray(vertex_array_object);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vertex_array_object);
  glDeleteBuffers(1, &vertex_buffer_object);
  glDeleteProgram(shader_program);

  glfwTerminate();

  return 0;
}

// Function callback registered with GLFW called when window dimensions change.  Allows us to reset the OpenGL viewport.
//
// Parameters
// window - the window whose dimensions have changed
// width - the new width of the window
// height - the new height of the window
void on_window_change(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

// Handles input events on the GLFW window.  Called during rendering loop.
//
// Parameters
// window - the window which has received input.
void process_input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

// Compiles a shader from given source
//
// Parameters
// source - OpenGL source to be compiled
//
// Returns an ID representing the shader
unsigned int compile_shader(unsigned int type, const char* source) {
  unsigned int shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  int success = 0;

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (success != GL_TRUE) {
    char info_log[512];

    glGetShaderInfoLog(shader, 512, NULL, info_log);
    std::cout << "Shader creation failed: [" << info_log << "]" << std::endl;

    // TODO(Chris I) Throw an exception if the shader isn't compiled successfully
  }

  return shader;
}

// Links compiled shaders to an OpenGL program
//
// Parameters
//  shaders - contains IDs of compiled shaders
//
// Returns the ID of the shader program.
unsigned int link_shaders(const std::vector<unsigned int>& shaders) {
  unsigned int shader_program = glCreateProgram();

  for (auto shader : shaders) {
    glAttachShader(shader_program, shader);
  }

  glLinkProgram(shader_program);

  int success = 0;

  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

  if (success != GL_TRUE) {
    char info_log[512];
    glGetProgramInfoLog(shader_program, 512, NULL, info_log);
    std::cout << "Shader linking failed: [" << info_log << "]" << std::endl;
  }

  for (auto shader : shaders) {
    glDeleteShader(shader);
  }

  return shader_program;
}

// Configure the various buffers required to make this run
//
// Parameters
// vertex_buffer_object - reference to our vertex buffer object
// vertex_array_object - reference to our vertex array object
void configure_buffer(unsigned int& vertex_buffer_object, unsigned int& vertex_array_object) {
  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
  };

  // TODO(Chris I) Understand everything beneath this point.  Currently black magic.
  glGenVertexArrays(1, &vertex_array_object);
  glGenBuffers(1, &vertex_buffer_object);
  glBindVertexArray(vertex_array_object);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
}