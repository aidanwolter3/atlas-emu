#include "src/ui/opengl/window.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "src/ui/opengl/shader.h"

// Suppress Apple's annoying deprecation warnings.
// OpenGL is not going away anytime soon.
#define GL_SILENCE_DEPRECATION

namespace {

const int kInitialWindowWidth = 680;
const int kInitialWindowHeight = 600;
// TODO: We should probably not be using so many textures. Also, this doesn't
// scale to multiple nametables.
const int kNumTextures = 960;

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

};  // namespace

OpenGLWindow::OpenGLWindow() {
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
                                  "Atlas", NULL, NULL);
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

  // Create the Shaders and Program.
  std::vector<std::unique_ptr<Shader>> shaders;
  shaders.push_back(std::unique_ptr<Shader>(
      new Shader(GL_VERTEX_SHADER, "src/ui/opengl/vertex.glsl")));
  shaders.push_back(std::unique_ptr<Shader>(
      new Shader(GL_FRAGMENT_SHADER, "src/ui/opengl/fragment.glsl")));
  program_ = std::make_unique<Program>(std::move(shaders));

  // Declare the texture.
  glGenTextures(1, &tile_textures_);
  glBindTexture(GL_TEXTURE_2D_ARRAY, tile_textures_);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, 8, 8,
               /*count=*/kNumTextures, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}

OpenGLWindow::~OpenGLWindow() { CloseWindow(); }

void OpenGLWindow::SetTile(int num, std::vector<uint8_t>& tile) {
  if (num > kNumTextures) {
    std::cout << "Tile number is too large: " << num << std::endl;
    return;
  }
  glBindTexture(GL_TEXTURE_2D_ARRAY, tile_textures_);
  glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
                  /*x=*/0, /*y=*/0, /*z=*/num, 8, 8, 1, GL_RGBA,
                  GL_UNSIGNED_BYTE, tile.data());
}

void OpenGLWindow::Update() {
  program_->Render();
  glfwSwapBuffers(glfw_window_);
  glfwPollEvents();
  if (glfwWindowShouldClose(glfw_window_)) {
    CloseWindow();
  }
}

void OpenGLWindow::SetAspectRatioCallback(
    AspectRatioCallback aspect_ratio_callback) {
  aspect_ratio_callback_ = aspect_ratio_callback;
}

void OpenGLWindow::CloseWindow() {
  if (!is_closed_) {
    is_closed_ = true;
    glfwTerminate();
  }
}