#include "src/input/opengl/input.h"

#include <iostream>

namespace {

// A singleton is required, so that the C callbacks can fetch the OpenGLInput
// object.
thread_local OpenGLInput* instance;

// static
void SetInstance(OpenGLInput* input) {
  if (instance) {
    std::cout << "Input cannot be created multiple times." << std::endl;
    return;
  }
  instance = input;
}

// static
OpenGLInput* GetInstance() { return instance; }

}  // namespace

OpenGLInput::OpenGLInput(GLFWwindow* window) : window_(window) {
  SetInstance(this);
  auto key_callback = [](GLFWwindow* window, int key, int, int action, int) {
    GetInstance()->OnKey(key, action);
  };
  glfwSetKeyCallback(window_, key_callback);

  state_ = {
      .a = false,
      .b = false,
      .select = false,
      .start = false,
      .up = false,
      .down = false,
      .left = false,
      .right = false,
  };
}

OpenGLInput::~OpenGLInput() = default;

Input::State OpenGLInput::GetButtonState() { return state_; }

void OpenGLInput::OnKey(int key, int action) {
  // Only handle presses and releases.
  if (action != GLFW_PRESS && action != GLFW_RELEASE) {
    return;
  }

  bool pressed = (action == GLFW_PRESS);
  switch (key) {
    case GLFW_KEY_J:
      state_.a = pressed;
      break;
    case GLFW_KEY_K:
      state_.b = pressed;
      break;
    case GLFW_KEY_ENTER:
      state_.start = pressed;
      break;
    case GLFW_KEY_TAB:
      state_.select = pressed;
      break;
    case GLFW_KEY_E:
      state_.up = pressed;
      break;
    case GLFW_KEY_D:
      state_.down = pressed;
      break;
    case GLFW_KEY_S:
      state_.left = pressed;
      break;
    case GLFW_KEY_F:
      state_.right = pressed;
      break;
  }
}
