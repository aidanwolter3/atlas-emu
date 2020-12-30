#include "src/ui/opengl/gl_init.h"

// clang-format off
#include <glad/glad.h>  // must go before glfw.h
// clang-format oni
#include <GLFW/glfw3.h>
#include <iostream>

namespace {

thread_local bool gl_initialized_ = false;

};  // namespace

void InitGLIfNeeded() {
  if (!gl_initialized_) {
    gl_initialized_ = true;
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
    }
  }
}
