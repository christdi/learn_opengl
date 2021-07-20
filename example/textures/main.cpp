#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

#include <iostream>
#include <vector>

#include "myopengl/shader.h"
#include "myopengl/shader_exception.h"

int run_application();
void process_input(GLFWwindow* window);
void on_window_change(GLFWwindow* window, int width, int height);
unsigned int create_vertex_buffer(float* vertices, size_t n);
unsigned int create_element_buffer(unsigned int* indices, size_t n);
unsigned int create_texture(const char* path, unsigned int format);

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

  float vertices[] = {
    0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
  };

  unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
  };

  unsigned int vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  unsigned int vbo = create_vertex_buffer(vertices, sizeof(vertices));
  unsigned int ebo = create_element_buffer(indices, sizeof(indices));
  unsigned int texture = create_texture("./texture/container.jpg", GL_RGB);

  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, texture);    
    default_shader.use();
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);

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
//
// Returns the OpenGL generated id for the vertex buffer.
unsigned int create_vertex_buffer(float* vertices, size_t n) {
  unsigned int vertex_buffer_id = 0;

  glGenBuffers(1, &vertex_buffer_id);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

  glBufferData(GL_ARRAY_BUFFER, n, vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return vertex_buffer_id;
}

// Creates and returns an element buffer.
//
// Parameters
// indices - indices of the vertices to render
// n - the amount of indices
//
// Returns the OpenGL generated id for the object buffer
unsigned int create_element_buffer(unsigned int* indices, size_t n) {
  unsigned int ebo = 0;

  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, n, indices, GL_STATIC_DRAW);

  return ebo;
}

unsigned int create_texture(const char* path, unsigned int format) {
  unsigned int texture = 0;

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width = 0;
  int height = 0;
  int channels = 0;

  unsigned char* data = stbi_load(path, &width, &height, &channels, 0);

  if (data != NULL) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture [" << path << "]" << std::endl;  
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  stbi_image_free(data);

  return texture;
}