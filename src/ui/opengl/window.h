#ifndef UI_OPENGL_WINDOW_H_
#define UI_OPENGL_WINDOW_H_

// clang-format off
#include <glad/glad.h>  // must go before glfw.h
// clang-format on
#include <GLFW/glfw3.h>

#include "src/ui/window.h"

class OpenGLWindow : public Window {
 public:
  OpenGLWindow();
  ~OpenGLWindow();

  // Window implementation:
  void Refresh() override;
  bool IsClosed() override { return is_closed_; }
  void SetTitle(std::string title) override;

 private:
  void CloseWindow();

  GLFWwindow* glfw_window_;
  bool is_closed_ = false;
};

#endif  // UI_OPENGL_WINDOW_H_
