#ifndef UI_OPENGL_RENDERER2_H_
#define UI_OPENGL_RENDERER2_H_

#include <memory>

#include "src/ui/opengl/program.h"
#include "src/ui/renderer2.h"

class OpenGLRenderer2 : public Renderer2 {
 public:
  OpenGLRenderer2();
  ~OpenGLRenderer2() override;

  // Renderer2 implementation:
  void Render(std::vector<uint8_t>& frame) override;

 private:
  std::unique_ptr<Program> program_;

  unsigned int frame_;
};

#endif  // UI_OPENGL_RENDERER2_H_
