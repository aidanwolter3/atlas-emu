#ifndef UI_OPENGL_WINDOW_H_
#define UI_OPENGL_WINDOW_H_

// Suppress Apple's deprecation warnings.
#define GL_SILENCE_DEPRECATION

#include <functional>
// clang-format off
#include <glad/glad.h>  // must go before glfw.h
// clang-format on
#include <GLFW/glfw3.h>

#include "src/ui/opengl/program.h"
#include "src/ui/window.h"

using AspectRatioCallback = std::function<void(float)>;

class OpenGLWindow : public Window {
 public:
  OpenGLWindow();
  ~OpenGLWindow();

  // Window implementation:
  bool IsClosed() override { return is_closed_; }
  void SetTile(int num, std::vector<uint8_t>& tile) override;
  void Update() override;

 private:
  void SetAspectRatioCallback(AspectRatioCallback aspect_ratio_callback);
  void CloseWindow();

  GLFWwindow* glfw_window_;
  std::unique_ptr<Program> program_;

  bool is_closed_ = false;
  unsigned int tile_textures_;
};

#endif  // UI_OPENGL_WINDOW_H_
