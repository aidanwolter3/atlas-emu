#include "src/ui/opengl/window.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "src/ui/opengl/shader.h"

// Suppress Apple's annoying deprecation warnings.
// OpenGL is not going away anytime soon.
#define GL_SILENCE_DEPRECATION

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
  glfwSwapInterval(0);

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

  // Declare the tile textures.
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &nametable_);
  glBindTexture(GL_TEXTURE_2D, nametable_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Declare the attribute table.
  glActiveTexture(GL_TEXTURE1);
  glGenTextures(1, &attribute_table_);
  glBindTexture(GL_TEXTURE_1D, attribute_table_);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Declare the frame palette.
  glActiveTexture(GL_TEXTURE2);
  glGenTextures(1, &frame_palette_);
  glBindTexture(GL_TEXTURE_1D, frame_palette_);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Declare the palette.
  glActiveTexture(GL_TEXTURE3);
  glGenTextures(1, &palette_);
  glBindTexture(GL_TEXTURE_1D, palette_);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glActiveTexture(0);
}

OpenGLWindow::~OpenGLWindow() { CloseWindow(); }

void OpenGLWindow::SetTitle(std::string title) {
  glfwSetWindowTitle(glfw_window_, title.c_str());
}

void OpenGLWindow::SetScroll(uint8_t x, uint8_t y) {
  glm::mat4 transform = glm::mat4(1.0f);
  float scroll_x = 2.0f * x / float(0xFF);
  float scroll_y = 2.0f * y / float(0xF0);
  transform = glm::translate(transform, glm::vec3(scroll_x, scroll_y, 0.0f));
  auto transform_loc =
      glGetUniformLocation(program_->gl_program(), "transform");
  glUniformMatrix4fv(transform_loc, 1, false, &transform[0][0]);
}

void OpenGLWindow::SetNametable(int num, std::vector<uint8_t>& nametable) {
  if (nametable.size() != (32 * 8 * 30 * 8)) {
    std::cout << "Nametable is the wrong size" << std::endl;
    return;
  }
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, nametable_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, 32 * 8, 30 * 8, 0, GL_RED_INTEGER,
               GL_UNSIGNED_BYTE, nametable.data());
}

void OpenGLWindow::SetAttributeTable(int num, std::vector<uint8_t>& table) {
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_1D, attribute_table_);
  glTexImage1D(GL_TEXTURE_1D, 0, GL_R8UI, table.size(), 0, GL_RED_INTEGER,
               GL_UNSIGNED_BYTE, table.data());
}

void OpenGLWindow::SetFramePalette(std::vector<uint8_t>& palette) {
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_1D, frame_palette_);
  glTexImage1D(GL_TEXTURE_1D, 0, GL_R8UI, palette.size(), 0, GL_RED_INTEGER,
               GL_UNSIGNED_BYTE, palette.data());
}

void OpenGLWindow::SetPalette(std::vector<uint8_t>& palette) {
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_1D, palette_);
  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB8, palette.size() / 3, 0, GL_RGB,
               GL_UNSIGNED_BYTE, palette.data());
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
