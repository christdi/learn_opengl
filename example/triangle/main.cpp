#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

void process_input(GLFWwindow* window);
void on_window_change(GLFWwindow* window, int width, int height);
unsigned int compile_shader(unsigned int type, const char* source);
unsigned int link_shaders(const std::vector<unsigned int>& shaders);
unsigned int create_vertex_buffer(float* vertices, size_t n);

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

  static const char* yellow_fragment_shader_source = "#version 330 core\n"
                                                     "out vec4 FragColor;\n"
                                                     "void main()\n"
                                                     "{\n"
                                                     "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
                                                     "}\n\0";

  static const char* green_fragment_shader_source = "#version 330 core\n"
                                                     "out vec4 FragColor;\n"
                                                     "void main()\n"
                                                     "{\n"
                                                     "   FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
                                                     "}\n\0";

  std::vector<unsigned int> shaders;
  shaders.push_back(compile_shader(GL_VERTEX_SHADER, vertex_shader_source));
  shaders.push_back(compile_shader(GL_FRAGMENT_SHADER, yellow_fragment_shader_source));
  unsigned int yellow_shader_program = link_shaders(shaders);

  shaders.clear();
  shaders.push_back(compile_shader(GL_VERTEX_SHADER, vertex_shader_source));
  shaders.push_back(compile_shader(GL_FRAGMENT_SHADER, green_fragment_shader_source));
  unsigned int green_shader_program = link_shaders(shaders);

  unsigned int vao[3];
  unsigned int vbo[3];

  glGenVertexArrays(3, vao);
  glBindVertexArray(vao[0]);

  float t1_vertices[] = {
    -0.8f, -0.8f, 0.0f,
    0.0f, -0.8f, 0.0f,
    -0.4f, 0.0f, 0.0f
  };

  vbo[0] = create_vertex_buffer(t1_vertices, sizeof(t1_vertices));

  glBindVertexArray(vao[1]);

  float t2_vertices[] = {
    0.0f, -0.8f, 0.0f,
    0.8f, -0.8f, 0.0f,
    0.4f, 0.0f, 0.0f
  };

  vbo[1] = create_vertex_buffer(t2_vertices, sizeof(t2_vertices));

  glBindVertexArray(vao[2]);

  float t3_vertices[] = {
    -0.4f, 0.0f, 0.0f,
    0.4f, 0.0f, 0.0f,
    0.0f, 0.8f, 0.0f
  };

  vbo[2] = create_vertex_buffer(t3_vertices, sizeof(t3_vertices));

  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(yellow_shader_program);
    glBindVertexArray(vao[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(vao[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(green_shader_program);
    glBindVertexArray(vao[2]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(3, vao);
  glDeleteBuffers(3, vbo);

  glDeleteProgram(yellow_shader_program);
  glDeleteProgram(green_shader_program);

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

// Creates and returns a vertex buffer.
//
// Parameters
// vertices - the vertices to be copied to the buffer
// n - the amount of vertices 
unsigned int create_vertex_buffer(float* vertices, size_t n) {
  unsigned int vertex_buffer_id = 0;

  glGenBuffers(1, &vertex_buffer_id);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

  glBufferData(GL_ARRAY_BUFFER, n, vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return vertex_buffer_id;
}