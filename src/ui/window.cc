// Suppress Apple's deprecation warnings.
#define GL_SILENCE_DEPRECATION

#include "window.h"

#include <cassert>
#include <iostream>

namespace {

const int kInitialWindowWidth = 680;
const int kInitialWindowHeight = 600;

// Ensure that only a single Window was created on this thread, because GLFW
// and Window uses thread-local storage.
thread_local bool window_created_ = false;

// The current callback for window resizes.
thread_local AspectRatioCallback aspect_ratio_callback_ = nullptr;

void OnResize(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
  if (aspect_ratio_callback_) {
    aspect_ratio_callback_((float)height / (float)width);
  }
}

}  // namespace

Window::Window() {
  if (window_created_) {
    assert("Cannot create multiple windows on the same thread.");
  }
  window_created_ = true;

  // Ensure this stays up-to-date with the version of GLAD in //glad.
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a window.
  glfw_window_ = glfwCreateWindow(kInitialWindowWidth, kInitialWindowHeight,
                                  "Windowing", NULL, NULL);
  if (glfw_window_ == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
  }
  glfwMakeContextCurrent(glfw_window_);

  // Load GL.
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
  }

  // Set the window size and callback.
  // Note: some platforms (retina macs) require this extra step is required
  // because the pixel dimensions do not match the viewport dimensions. See:
  // https://www.glfw.org/docs/latest/window.html#window_fbsize
  int width, height;
  glfwGetFramebufferSize(glfw_window_, &width, &height);
  glfwSetFramebufferSizeCallback(glfw_window_, OnResize);
  glViewport(0, 0, width, height);
}

Window::~Window() { glfwTerminate(); }

void Window::SetAspectRatioCallback(AspectRatioCallback aspect_ratio_callback) {
  aspect_ratio_callback_ = aspect_ratio_callback;
}

bool Window::Update() {
  glfwSwapBuffers(glfw_window_);
  glfwPollEvents();
  return glfwWindowShouldClose(glfw_window_);
}
