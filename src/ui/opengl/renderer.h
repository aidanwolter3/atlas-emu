#ifndef UI_OPENGL_RENDERER_H_
#define UI_OPENGL_RENDERER_H_

#include <memory>

#include "src/ui/opengl/program.h"
#include "src/ui/renderer.h"

class OpenGLRenderer : public Renderer {
 public:
  OpenGLRenderer();
  ~OpenGLRenderer() override;

  // Renderer implementation:
  void Render(std::vector<uint8_t>& frame) override;

 private:
  std::unique_ptr<Program> program_;

  unsigned int frame_;
};

#endif  // UI_OPENGL_RENDERER_H_
