#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "myopengl/shader.h"
#include "myopengl/shader_exception.h"

int run_application();
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
  try {
    return run_application();
  } catch (myopengl::shader_exception& e) {
    std::cout << "Error loading shaders: [" << e.what() << "]" << std::endl;
  }
}

// Runs the application
//
// Returns a status code which should be returned to the OS
int run_application() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);

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

  myopengl::shader default_shader("./shader/vertex.glsl", "./shader/fragment.glsl");

  unsigned int vao = 0;
  unsigned int vbo = 0;

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  float vertices[] = {
    0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  vbo = create_vertex_buffer(vertices, sizeof(vertices));

  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    default_shader.use();
    default_shader.setFloat("offset", 0.0f);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);

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
  
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return vertex_buffer_id;
}