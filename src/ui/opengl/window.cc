#include "src/ui/opengl/window.h"

#include <cassert>
#include <iostream>

#include "src/ui/opengl/gl_init.h"

namespace {

const int kInitialWindowWidth = 680;
const int kInitialWindowHeight = 600;

// Ensure that only a single Window was created on this thread, because GLFW
// and Window uses thread-local storage.
thread_local bool window_created_ = false;

};  // namespace

OpenGLWindow::OpenGLWindow() {
  if (window_created_) {
    assert("Cannot create multiple windows on the same thread.");
  }
  window_created_ = true;

  // Ensure this stays up-to-date with the version of GLAD.
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a window.
  glfw_window_ = glfwCreateWindow(kInitialWindowWidth, kInitialWindowHeight,
                                  "glfw", NULL, NULL);
  if (glfw_window_ == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    CloseWindow();
    return;
  }
  glfwMakeContextCurrent(glfw_window_);
  glfwSwapInterval(0);

  InitGLIfNeeded();

  // Set the window size.
  // Note: some platforms (retina macs) require this extra step is required
  // because the pixel dimensions do not match the viewport dimensions. See:
  // https://www.glfw.org/docs/latest/window.html#window_fbsize
  int width, height;
  glfwGetFramebufferSize(glfw_window_, &width, &height);
  glViewport(0, 0, width, height);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

OpenGLWindow::~OpenGLWindow() { CloseWindow(); }

void OpenGLWindow::Refresh() {
  glfwSwapBuffers(glfw_window_);
  glfwPollEvents();
  if (glfwWindowShouldClose(glfw_window_)) {
    CloseWindow();
  }
}

void OpenGLWindow::SetTitle(std::string title) {
  glfwSetWindowTitle(glfw_window_, title.c_str());
}

void OpenGLWindow::CloseWindow() {
  if (!is_closed_) {
    is_closed_ = true;
    glfwTerminate();
  }
}
