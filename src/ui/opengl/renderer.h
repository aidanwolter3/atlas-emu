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
  void Render() override;
  void SetScroll(int x, int y) override;
  void SetNametable(int num, std::vector<uint8_t>& nametable) override;
  void SetAttributeTable(int num, std::vector<uint8_t>& table) override;
  void SetFramePalette(std::vector<uint8_t>& palette) override;
  void SetPalette(std::vector<uint8_t>& palette) override;

 private:
  void PrepareUniforms();
  void LoadElements();

  std::unique_ptr<Program> program_;

  // Uniforms.
  unsigned int nametable_;
  unsigned int attribute_table_;
  unsigned int frame_palette_;
  unsigned int palette_;
};

#endif  // UI_OPENGL_RENDERER_H_
