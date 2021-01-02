#ifndef INPUT_OPENGL_INPUT_H_
#define INPUT_OPENGL_INPUT_H_

// clang-format off
#include <glad/glad.h>  // must go before glfw.h
// clang-format on
#include <GLFW/glfw3.h>

#include "src/input/input.h"

class OpenGLInput : public Input {
 public:
  OpenGLInput(GLFWwindow* window);
  OpenGLInput(OpenGLInput const&) = delete;
  OpenGLInput& operator=(OpenGLInput const&) = delete;
  ~OpenGLInput() override;

  // Input implementation:
  Input::State GetButtonState() override;

 private:
  void OnKey(int key, int action);

  Input::State state_;
  GLFWwindow* window_;
};

#endif  // INPUT_OPENGL_INPUT_H_
