#include <memory>

#include "program.h"
#include "shader.h"
#include "window.h"

// Suppress Apple's annoying deprecation warnings.
// OpenGL is going away anytime soon.
#define GL_SILENCE_DEPRECATION

int main(int argc, char** argv) {
  Window window;

  std::vector<std::unique_ptr<Shader>> shaders;
  shaders.push_back(std::unique_ptr<Shader>(
      new Shader(GL_VERTEX_SHADER, "src/ui/vertex.glsl")));
  shaders.push_back(std::unique_ptr<Shader>(
      new Shader(GL_FRAGMENT_SHADER, "src/ui/fragment.glsl")));

  Program program(std::move(shaders));

  bool window_should_close = false;
  while (!window_should_close) {
    program.Render();
    window_should_close = window.Update();
  }
  return 0;
}
