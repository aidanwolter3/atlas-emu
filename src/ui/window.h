#ifndef WINDOW_H_
#define WINDOW_H_

#include <functional>

// clang-format off
#include <glad/glad.h>  // must go before glfw.h
// clang-format on
#include <GLFW/glfw3.h>

using AspectRatioCallback = std::function<void(float)>;

class Window {
 public:
  Window();
  ~Window();

  // Delete copy and assign.
  Window operator=(const Window&) = delete;
  Window(const Window&) = delete;

  void SetAspectRatioCallback(AspectRatioCallback aspect_ratio_callback);

  // Refreshes the view, and returns whether the window should close.
  bool Update();

 private:
  GLFWwindow* glfw_window_;
};

#endif  // WINDOW_H_
