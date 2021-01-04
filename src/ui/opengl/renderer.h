#ifndef UI_OPENGL_RENDERER_H_
#define UI_OPENGL_RENDERER_H_

#include "src/ui/opengl/background.h"
#include "src/ui/opengl/sprites.h"
#include "src/ui/renderer.h"

class OpenGLRenderer : public Renderer {
 public:
  OpenGLRenderer();
  ~OpenGLRenderer() override;

  // Renderer implementation:
  void Render() override;
  void SetMirroringMode(MirroringMode mode) override;
  void SetMask(uint8_t mask) override;
  void SetScroll(int x, int y) override;
  void SetNametable(int num, std::vector<uint8_t>& nametable) override;
  void SetAttributeTable(int num, std::vector<uint8_t>& table) override;
  void SetFramePalette(std::vector<uint8_t>& palette) override;
  void SetPalette(std::vector<uint8_t>& palette) override;
  void SetSprites(std::vector<Sprite>& sprites) override;

 private:
  void PrepareTextures();
  void LoadElements();

  bool show_background = true;
  bool show_sprites = true;

  // Textures.
  unsigned int palette_;

  // TODO: move to optional
  std::unique_ptr<Background> background_;
  std::unique_ptr<Sprites> sprites_;
};

#endif  // UI_OPENGL_RENDERER_H_
